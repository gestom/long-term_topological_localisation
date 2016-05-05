#include <iostream>
#include <fstream>	
#include <cstdlib>	
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "CRawImage.h" 
#include "CFrelement.h" 
#include "CFeatureMap.h" 

unsigned int times[10000];
unsigned int realTimes[10000];
unsigned int averages[10000];
unsigned char day[10000];
int numFiles = 0;
char filename[100];
bool draw = false;
int dummy =0;
Mat imgColor;
Mat imgGray;

int featuresToExtract = 500;
float confVec[8];

int compare (const void * a, const void * b)
{
  if ( *(int*)a <  *(int*)b ) return -1;
  if ( *(int*)a == *(int*)b ) return 0;
  if ( *(int*)a >  *(int*)b ) return 1;
  return 0;
}

void listFiles(const char* name)
{
	struct dirent *entry;
	DIR *dir = opendir(name);		
	while ((entry = readdir(dir)) != NULL){
		times[numFiles] = atoi(entry->d_name);
		sprintf(filename,"%05i.bmp",times[numFiles]);
		if (strcmp(entry->d_name,filename)==0) numFiles++;
	}	
	qsort(times,numFiles,sizeof(int),compare);
	closedir(dir);
	//for (int i = 0;i<numFiles;i++) printf("%05i.bmp\n",times[i]);
}

int main(int argc,char *argv[])
{
	/*Mat imgMap;
	CFeatureMap map;
	int numMaps = 8;
	sprintf(filename,"%s/place_%i.map",argv[3],atoi(argv[2]));
	map.load(filename);
	for (int j=0;j<map.visibility.cols;j++){
		for (int o=0;o<10;o++){
			{
				for (int k=0;k<map.visibility.rows;k++) printf("%i ",map.visibility.at<char>(k,j));
			}
			printf("\n");	
		}
	}
	return 0;*/


	//map.load("place.map");
	if (strcmp(argv[1],"build")==0)
	{
		listFiles(argv[2]);
		CFeatureMap map(numFiles);
		printf("NumFiles: %i\n",numFiles);
		for (int i=0;i<numFiles;i++)
		{
			sprintf(filename,"%s/%05i.bmp",argv[2],times[i]);
			imgColor = imread(filename, CV_LOAD_IMAGE_COLOR);
			imgGray  = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
			map.extract(imgGray,featuresToExtract);
			map.addToMap(i*7200,0.7,true);
			if (draw) map.drawAllFeatures(imgColor);
		}
		map.save(argv[3]);
		//sprintf(filename,"%s.readable",argv[3]);
		//map.saveReadable(filename);
	}
	//make readable
	if (strcmp(argv[1],"readable")==0)
	{
		CFeatureMap map(numFiles);
		map.load(argv[2]);
		map.saveReadable(argv[3]);
	}
	//reduce input_map threshold output_map 
	if (strcmp(argv[1],"reduce")==0)
	{
		CFeatureMap map(numFiles);
		map.load(argv[2]);
		map.sortAndReduce(atof(argv[3]));
		map.save(argv[4]);
	}

	//recalculate image_dir input_map output_map 
	if (strcmp(argv[1],"recalculate")==0)
	{
		listFiles(argv[2]);
		CFeatureMap map(numFiles);
		map.load(argv[3]);
		for (int i=0;i<numFiles;i++)
		{
			sprintf(filename,"%s/%05i.bmp",argv[2],times[i]);
			imgColor = imread(filename, CV_LOAD_IMAGE_COLOR);
			imgGray  = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
			map.extract(imgGray,featuresToExtract);
			map.reidentify(i*7200,0.7,false);
			//map.drawReidentified(imgColor,imgColor);
		}
		map.fremenize(MAX_ADAPTIVE_ORDER+1);
		map.save(argv[4]);
		//map.fremenTest(atoi(argv[5]));
	}

	//match image_dir map_file num_features fremen_order
	if (strcmp(argv[1],"match")==0)
	{
		listFiles(argv[2]);
		CFeatureMap map(numFiles);
		CFeatureMap imageMap(numFiles);
		map.load(argv[3]);
		//map.print();
		for (int i=0;i<numFiles;i++)
		{
			sprintf(filename,"%s/%05i.bmp",argv[2],times[i]);
			imgColor = imread(filename, CV_LOAD_IMAGE_COLOR);
			imgGray  = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
			printf("%i\n",imageMap.extract(imgGray,featuresToExtract));
			map.predictThreshold(0,0,0);
			map.match(&imageMap,true,&dummy);
			//map.drawPredicted(imgColor);
			map.drawCurrentMatches(imgColor,imgColor);
		}
	}
	if (strcmp(argv[1],"test")==0)
	{
		Mat imgMap;
		listFiles(argv[2]);
		CFeatureMap maps[8];
		CFeatureMap imageMap;
		int numMaps = 5;
		for (int i=0;i<numMaps;i++)
		{
			sprintf(filename,"%s/place_%i.map",argv[3],i);
			maps[i].load(filename);
		}
		bool debug = true;
		float matching[8];
		float predicted[8];
		float extracted;
		float success,unknown,failures,globalSuccess,globalUnknown,globalFailures;
		globalSuccess = globalUnknown = globalFailures = 0;
		numFiles = 3; 
		int step = 1;
		float sumPredicted = 0;
		float sumMatched = 0;
		float sumCorrect = 0;
		float sumExtracted = 0;
		int matches = 0;
		//number of places*/
		for (int im=0;im<numMaps;im++)
		{
			//int im = indi[imt];
			memset(confVec,0,8*sizeof(float));	
			failures=success=unknown=0; 
			for (int i=0;i<numFiles;i+=step)
			{
				int indi[4] = {1,5,8};
				int image = indi[i];
				for (int j=0;j<numMaps;j++)
				{
					if (debug) printf("Image: %i %04i : ",im,j);
					if (atof(argv[4]) > 1){
						predicted[j] = maps[j].predictNumber(image*7200,atoi(argv[4]),atoi(argv[5]));
						//featuresToExtract = atoi(argv[4]); 
					}else{
						predicted[j] = maps[j].predictThreshold(image*7200,atof(argv[4]),atoi(argv[5]));
						//featuresToExtract = predicted[j]; 
					}
					if (debug) printf("%02.0f ",matching[j]);
				}
				sprintf(filename,"%s/place_%i/%05i.bmp",argv[2],im,image);
				imgGray  = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
				imgColor  = imread(filename, CV_LOAD_IMAGE_COLOR);
				extracted = imageMap.extract(imgGray,featuresToExtract);
				//if (extracted == 0) printf("Extracted 0 features for place %i and image %i.\n",im,i); 
				for (int j=0;j<numMaps;j++){
					matching[j] = maps[j].match(&imageMap,strcmp(argv[6],"geometry")==0,&matches);
					
					if (j == im){
						sumExtracted += extracted;
						sumPredicted += predicted[j];
						sumMatched += matches;
						sumCorrect += matching[j];
					}
					matching[j] = matching[j];//;/(predicted[j]+1);
					//if (j == im) sumCorrect += matching[j]; else sumMatched += matching[j];
			
					if (extracted == 0){
						if (atof(argv[4]) > 1){
							matching[j] = 1-predicted[j]/maps[j].predictNumber(image*7200,1000,0)/10.0;
						}else{
							matching[j] = 1-predicted[j]/maps[j].predictThreshold(image*7200,0.0,0)/10.0;
						}
					}
					//if (extracted == 0) printf("Predicted %i %i %.0f %.3f\n",i,j,predicted[j],matching[j]); 
					if (draw)
					{
						sprintf(filename,"training/place_%i/%05i.bmp",j,i);
						imgMap = imread(filename, CV_LOAD_IMAGE_COLOR);
						maps[j].drawCurrentMatches(imgColor,imgMap);
					}
				}
				float maxMatch = 0;
				int index = -1;
				for (int k = 0;k<numMaps;k++){
					if (maxMatch < matching[k])
					{
						index = k;
						maxMatch = matching[k];
					}
				} 
				if (debug) printf(": %i\n",index);
				if (index == im){
					success++;
					confVec[index]++;
				}else if (index == -1){
					unknown++;
				}else{
					failures++;
					confVec[index]++;
				}
				if (index != im && false){
					for (int j=0;j<numMaps;j++)
					{
						maps[j].debug = true;
						predicted[j] = maps[j].predictThreshold(i*7200,atof(argv[4]),atoi(argv[5]));
						maps[j].debug = false;
						matching[j] = maps[j].match(&imageMap,strcmp(argv[6],"geometry")==0,&dummy);
						sprintf(filename,"testing_Nov/place_%i/%05i.bmp",j,i);
						imgMap = imread(filename, CV_LOAD_IMAGE_COLOR);
						printf("Testing: %i Training: %i Sequence: %i\n",im,j,i);
						maps[j].drawCurrentMatches(imgColor,imgMap);
					}
				}
			}
			globalSuccess += success;
			globalUnknown += unknown;
			globalFailures += failures;
			printf("Results: %f %f %f ",success/numFiles*step,unknown/numFiles*step,failures/numFiles*step);
			for (int i=0;i<8;i++) printf("%.3f ",confVec[i]/numFiles*step);
			printf("\n");
		}
		printf("ROC: %f %f %.0f %.0f %.0f %.0f\n",sumCorrect/sumMatched,(sumMatched-sumCorrect)/(sumPredicted-sumCorrect),sumCorrect,sumMatched,sumPredicted,sumExtracted);
		printf("Overall: %f %f %f %i\n",globalSuccess/numFiles/numMaps*step,globalUnknown/numFiles/numMaps*step,globalFailures/numFiles/numMaps*step,numFiles);
		printf("Debug: %.0lf %i %i\n",globalSuccess,numFiles,numMaps);
	}
	return 0;
}
