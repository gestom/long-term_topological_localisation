#include "CFeatureMap.h" 

bool compareResponse(const KeyPoint &a,const KeyPoint &b)
{
	return ((float)a.response > (float)b.response);
}

bool compareAngle(const KeyPoint &a,const KeyPoint &b)
{
	return ((float)a.angle > (float)b.angle);
}

CFeatureMap::CFeatureMap(int numImages)
{
	debug = false;
	totalPics = numImages;
	numPics = 0;
	frelementArray = NULL;
	times = Mat::zeros(1,totalPics,CV_32SC1);
}

CFeatureMap::~CFeatureMap()
{
	if (frelementArray != NULL)
	{
		for (int i = 0;i<visibility.rows;i++) delete frelementArray[i];
		free(frelementArray);
	}
}

void CFeatureMap::distinctiveMatch(Mat& descriptors1,Mat& descriptors2, vector<DMatch>& matches,float distanceFactor,bool crosscheck)
{
   Ptr<DescriptorMatcher> descriptorMatcher;
   vector<vector<DMatch> > allMatches1to2, allMatches2to1;
   matches.clear();
   if (descriptors1.rows == 0 || descriptors2.rows == 0) return;
   if (L2_NORM)
      descriptorMatcher = new BFMatcher(cv::NORM_L2,  false);
   else 
      descriptorMatcher = new BFMatcher(cv::NORM_HAMMING, false);
   descriptorMatcher->knnMatch(descriptors1, descriptors2, allMatches1to2, 2);


   if (!crosscheck)
   {
      for(unsigned int i=0; i < allMatches1to2.size(); i++)
      {
         if (allMatches1to2[i].size() == 2)
         { 
            if (allMatches1to2[i][0].distance < allMatches1to2[i][1].distance * distanceFactor)
            {
               DMatch match = DMatch(allMatches1to2[i][0].queryIdx, allMatches1to2[i][0].trainIdx, allMatches1to2[i][0].distance);
               matches.push_back(match);
            }
         }         
         else if (allMatches1to2[i].size() == 1)
         {
            DMatch match = DMatch(allMatches1to2[i][0].queryIdx, allMatches1to2[i][0].trainIdx, allMatches1to2[i][0].distance);
            matches.push_back(match);
            cerr << "ERROR" << endl;
         }
         
      }
   }
   else
   {
      descriptorMatcher->knnMatch(descriptors2, descriptors1, allMatches2to1, 2);
      for(unsigned int i=0; i < allMatches1to2.size(); i++)
      {
         if (allMatches1to2[i].size() == 2)
         { 
            if (allMatches2to1[allMatches1to2[i][0].trainIdx].size() == 2)
	    {
		    if (allMatches1to2[i][0].distance < allMatches1to2[i][1].distance * distanceFactor && allMatches2to1[allMatches1to2[i][0].trainIdx][0].distance < allMatches2to1[allMatches1to2[i][0].trainIdx][1].distance * distanceFactor && allMatches1to2[i][0].trainIdx == allMatches2to1[allMatches1to2[i][0].trainIdx][0].queryIdx)
		    {
			    DMatch match = DMatch(allMatches1to2[i][0].queryIdx, allMatches1to2[i][0].trainIdx, allMatches1to2[i][0].distance);
			    matches.push_back(match);
		    }
		    //else if ((allMatches1to2[i][0].distance > allMatches1to2[i][1].distance * (1+distanceFactor)*0.5) && (allMatches2to1[allMatches1to2[i][0].trainIdx][0].distance > allMatches2to1[allMatches1to2[i][0].trainIdx][1].distance*(1+distanceFactor)*0.5))
		    else if ((allMatches1to2[i][0].distance > allMatches1to2[i][1].distance * distanceFactor) && (allMatches2to1[allMatches1to2[i][0].trainIdx][0].distance > allMatches2to1[allMatches1to2[i][0].trainIdx][1].distance*distanceFactor))
		    {
			    DMatch match = DMatch(allMatches1to2[i][0].queryIdx, allMatches1to2[i][0].trainIdx, -10000);
			    matches.push_back(match);
		    }
	    }
            else if (allMatches2to1[allMatches1to2[i][0].trainIdx].size() == 1)
               if (allMatches1to2[i][0].distance  < allMatches1to2[i][1].distance * distanceFactor && allMatches1to2[i][0].trainIdx == allMatches2to1[allMatches1to2[i][0].trainIdx][0].queryIdx)
               {
                  DMatch match = DMatch(allMatches1to2[i][0].queryIdx, allMatches1to2[i][0].trainIdx, allMatches1to2[i][0].distance);
                  matches.push_back(match);
                  cerr << "ERROR" << endl;
               } 
         }
         else if (allMatches2to1[allMatches1to2[i][0].trainIdx].size() == 2)
         {
            if (allMatches2to1[allMatches1to2[i][0].trainIdx][0].distance < allMatches2to1[allMatches1to2[i][0].trainIdx][1].distance * distanceFactor && allMatches1to2[i][0].trainIdx == allMatches2to1[allMatches1to2[i][0].trainIdx][0].queryIdx)
            {
                  DMatch match = DMatch(allMatches1to2[i][0].queryIdx, allMatches1to2[i][0].trainIdx, allMatches1to2[i][0].distance);
                  matches.push_back(match);
                  cerr << "ERROR" << endl;
            }
         }
         else if (allMatches1to2[i][0].trainIdx == allMatches2to1[allMatches1to2[i][0].trainIdx][0].queryIdx)
         {
               DMatch match = DMatch(allMatches1to2[i][0].queryIdx, allMatches1to2[i][0].trainIdx, allMatches1to2[i][0].distance);
               matches.push_back(match);
               cerr << "ERROR" << endl;
         } 

      }

   }
}

int CFeatureMap::drawPredicted(Mat img)
{
	//namedWindow("Extracted Features", 1);
	Mat out;
	drawKeypoints(img,currentPositions, out, Scalar(0,0,255),4 );
	imshow("ExtractedFeatures", out);
	waitKey(0);
	return 0;
}

int CFeatureMap::drawAllFeatures(Mat img)
{
	//if (imagePositions.size() != 0 && globalPositions.size() != 0){
		namedWindow("Extracted features", 1);
		Mat out;
		drawMatches(img,imagePositions,img,globalPositions,matches,out,Scalar(0,0,255),Scalar(0,0,255),vector<char>(),0);
		imshow("Extracted features", out);
		waitKey(0);
	//}
	return 0;
}

int CFeatureMap::drawReidentified(Mat img1,Mat img2)
{
	namedWindow("Extracted features", 1);
	Mat out;
	drawMatches(img1,globalPositions,img2,imagePositions,matches,out,Scalar(0,0,255),Scalar(0,0,255),vector<char>(),0);
	imshow("Extracted features", out);
	waitKey(0);
	return 0;
}

int CFeatureMap::drawCurrentMatches(Mat img1,Mat img2)
{
	namedWindow("Extracted features", 1);
	Mat out;
	drawMatches(img1,imagePositions,img2,currentPositions,matches,out,Scalar(0,0,255),Scalar(0,0,255),vector<char>(),0);
	imshow("Extracted features", out);
	waitKey(0);
	return 0;
}

void CFeatureMap::fremenTest(int order)
{
	for (int i=0;i<visibility.cols;i++)printf("%i %f %i\n",visibility.at<char>(0,i),frelementArray[0]->estimate(i*600,order),frelementArray[0]->estimate(i*600,order)>0.5);
}

int CFeatureMap::extract(Mat img,int number)
{
	StarFeatureDetector detector(45,0,10,8,5);
	BriefDescriptorExtractor extractor;
	//SIFT detector(200,3,0,10,1.6);

	imagePositions.clear();
	detector.detect(img,  imagePositions);
	if (imagePositions.size() > number)
	{
		std::sort(imagePositions.begin(),imagePositions.end(),compareResponse);
		imagePositions.resize(number);
	}
	else
	{
		std::sort(imagePositions.begin(),imagePositions.end(),compareResponse);
	}
	extractor.compute(img,  imagePositions, imageDescriptors);
	//detector.compute(img,  imagePositions, imageDescriptors);
	//printf("Features: %ld\n",imagePositions.size());
	return imagePositions.size();
}

void CFeatureMap::addToMap(unsigned int currentTime,float distanceThreshold,bool crosscheck)
{
	addToMap(globalDescriptors,imageDescriptors,matches,imagePositions,distanceThreshold,crosscheck);
//	for (int i=0;i<globalPositions.size();i++)printf("Fotures: %f %i\n",imagePositions[i].response,imagePositions.size());
	times.at<uint32_t>(0,numPics) = currentTime;
	numPics++;
}

void CFeatureMap::reidentify(unsigned int currentTime,float distanceThreshold,bool crosscheck)
{
	matches.clear();
	distinctiveMatch(globalDescriptors,imageDescriptors, matches,distanceThreshold,crosscheck);
	for(unsigned int j=0; j < globalDescriptors.rows; j++)visibility.at<char>(j,numPics) = 0;
	for(unsigned int i=0; i<matches.size() ; i++)
	{
		//fprintf(stdout,"R:%f\n",allMatches1to2[i][0].distance);
		int qI=matches[i].queryIdx;
		int tI=matches[i].trainIdx;
		int hD = fabs(globalPositions[qI].pt.x-imagePositions[tI].pt.x);
		int vD = fabs(globalPositions[qI].pt.y-imagePositions[tI].pt.y);
		if (vD < VERTICAL_LIMIT && hD < HORIZONTAL_LIMIT && matches[i].distance!=-10000)
		{ 
			visibility.at<char>(qI,numPics) = 1;
		}else{
			visibility.at<char>(qI,numPics) = 0;
			matches.erase(matches.begin()+i--);
		}
	}
	times.at<uint32_t>(0,numPics) = currentTime;
	numPics++;
}

void CFeatureMap::fremenize(int order)
{
	unsigned char *signal = (unsigned char*)calloc(visibility.cols,sizeof(unsigned char));
	unsigned int *timeArray = (unsigned int*)calloc(visibility.cols,sizeof(unsigned int));

	frelementArray = (ELEMENT**) calloc(visibility.rows,sizeof(ELEMENT*));
	for (int j=0;j<visibility.cols;j++) timeArray[j] = times.at<uint32_t>(0,j);
	for (int i=0;i<visibility.rows;i++)
	{
		for (int j=0;j<visibility.cols;j++) signal[j] = visibility.at<char>(i,j);
		frelementArray[i] = new ELEMENT();
		frelementArray[i]->build(timeArray,signal,totalPics,order);
		if (debug) frelementArray[i]->print();
	}
	free(signal);
	free(timeArray);
}

void CFeatureMap::sortAndReduce(float threshold)
{
	for (int i = 0;i<globalPositions.size();i++){
		globalPositions[i].class_id = i;
		globalPositions[i].angle = 0; 
		for (int j=0;j<totalPics;j++) globalPositions[i].angle+=visibility.at<char>(i,j);
		globalPositions[i].angle/=totalPics;
	}

	std::sort(globalPositions.begin(),globalPositions.end(),compareAngle);
	int number = 0;
	for (number=0;number<globalPositions.size() && globalPositions[number].angle > threshold;number++) {}
	if (debug) printf("Number of retained features at %.4f is %i out of %ld.\n",threshold,number,globalPositions.size());
	globalPositions.resize(number);
 
	cv::Mat vis;
	currentDescriptors = globalDescriptors;
	globalDescriptors.resize(0,0);
	int index = 0;
	for (int i = 0;i<globalPositions.size();i++){
		globalPositions[i].angle=-1;
		index = globalPositions[i].class_id;
		globalDescriptors.push_back(currentDescriptors.row(index));
		vis.push_back(visibility.row(index));
	}
	visibility = vis;
}

float CFeatureMap::predictThreshold(unsigned int time,float threshold,int order)
{
	currentPositions.clear();
	currentDescriptors.resize(0,0);
	for (int i = 0;i<globalPositions.size();i++)
	{
		if (frelementArray[i]->estimate(time,order) > threshold)
		{
			currentPositions.push_back(globalPositions[i]);
			currentDescriptors.push_back(globalDescriptors.row(i));
		}
	}
	return currentPositions.size();
}


float CFeatureMap::predictNumber(unsigned int time,int number,int order)
{
	currentPositions = globalPositions;
	for (int i = 0;i<currentPositions.size();i++){
		currentPositions[i].class_id = i;
		currentPositions[i].angle = frelementArray[i]->estimate(time,order);
	}
	if (currentPositions.size()<number) number = currentPositions.size();
	std::sort(currentPositions.begin(),currentPositions.end(),compareAngle);
	currentPositions.resize(number);
	currentDescriptors.resize(0,0);
	float cumProb = 0;
	for (int i = 0;i<number;i++)cumProb += currentPositions[i].angle;
	for (int i = 0;i<number;i++) currentPositions[i].angle=-1;
	for (int i = 0;i<number;i++)currentDescriptors.push_back(globalDescriptors.row(currentPositions[i].class_id));
	return cumProb;
}

void CFeatureMap::print()
{
	for (int i=0;i<visibility.rows;i++) frelementArray[i]->print();
}


void CFeatureMap::saveReadable(const char* name)
{
	FILE* file = fopen(name,"w+");
	unsigned char dummyBin[globalDescriptors.cols*8];	
	unsigned char dummyDes[globalDescriptors.cols];
	for (int i=0;i<visibility.rows;i++)
	{
		for (int j=0;j<globalDescriptors.cols;j++) dummyDes[j] = globalDescriptors.at<unsigned char>(i,j);
		binaryDescriptor(dummyBin,dummyDes,globalDescriptors.cols);

		//keypoint
		cv::KeyPoint kp = globalPositions[i];
		fprintf(file,"Position: %f %f %f %f %f %i %i\n",kp.pt.x,kp.pt.y,kp.size,kp.angle,kp.response,kp.octave,kp.class_id);

		//descriptor
		fprintf(file,"Descriptor: %02x",globalDescriptors.at<char>(i,0));
		for (int j=1;j<globalDescriptors.cols;j++) fprintf(file,",%02x",globalDescriptors.at<unsigned char>(i,j));
		fprintf(file,"\n");

		//descriptor
		fprintf(file,"Binary: %x",dummyBin[0]);
		for (int j=1;j<globalDescriptors.cols*8;j++) fprintf(file,",%x",dummyBin[j]);
		fprintf(file,"\n");

		//visibility
		fprintf(file,"Visibility: %i",visibility.at<char>(i,0));
		for (int j=1;j<visibility.cols;j++) fprintf(file,",%i",visibility.at<char>(i,j));
		fprintf(file,"\n");
	}
	fclose(file);	
}

void CFeatureMap::savePredictions(const char* baseName,int timeQuantum)
{
	char name[1000];
	sprintf(name,"%s.vis",baseName);
	FILE* file = fopen(name,"w+");
	for (int t=0;t<visibility.cols;t++){
		for (int f=0;f<visibility.rows;f++) fprintf(file,"%i ",visibility.at<char>(f,t));
		fprintf(file,"\n");
	}
	fclose(file);
	
	for (int o=0;o<7;o++)
	{
		sprintf(name,"%s.fremen_%i",baseName,o);
		file = fopen(name,"w+");
		for (int t=0;t<visibility.cols;t++){
			for (int f=0;f<visibility.rows;f++) fprintf(file,"%.3f ",frelementArray[f]->estimate(t*timeQuantum,o));
			fprintf(file,"\n");
		}	
		fclose(file);
	} 	
}
void CFeatureMap::save(const char* name)
{
	cv::FileStorage storage(name, cv::FileStorage::WRITE);
	//converting keypoints to matrices
	cv::Mat gP;
	for (int i=0;i<globalPositions.size();i++)
	{
		cv::KeyPoint kp = globalPositions[i];
		cv::Mat tM = (Mat_<float>(1,7) << (float)kp.pt.x,(float)kp.pt.y,(float)kp.size,(float)kp.angle,(float)kp.response,(float)kp.octave,(float)kp.class_id);
		gP.push_back(tM);
	}
	storage << "Keypoints" << gP;
	storage << "Descriptors" << globalDescriptors;
	storage << "Visibility" << visibility;
	storage << "Times" << times;
	if (frelementArray!= NULL)
	{
		cv::Mat fremen;
		for (int i = 0;i< globalPositions.size();i++)
		{
			if (debug) frelementArray[i]->print();
			cv::Mat fM = (Mat_<float>(1,5) << (float)0,(float)0,(float)i,(float)frelementArray[i]->order,(float)frelementArray[i]->gain);
			fremen.push_back(fM);
			for (int j = 0;j<frelementArray[i]->order ;j++){
				SFrelement f = frelementArray[i]->frelements[j];
				cv::Mat fM = (Mat_<float>(1,5) << (float)i,(float)j,f.amplitude,f.phase,f.frequency);
				fremen.push_back(fM);
			}
		}
		storage << "Fremen" << fremen;
	}
	storage.release();
}

bool CFeatureMap::load(const char* name)
{
	cv::FileStorage storage(name, cv::FileStorage::READ);
	if (storage.isOpened() == false) return -1;
	globalPositions.clear();
	cv::Mat gP,fremen;
	storage["Keypoints"] >> gP;
	for (int i=0;i<gP.rows;i++)
	{
		cv::KeyPoint kp(gP.at<float>(i,0),gP.at<float>(i,1),gP.at<float>(i,2),gP.at<float>(i,3),gP.at<float>(i,4),gP.at<float>(i,5),gP.at<float>(i,6));
		globalPositions.push_back(kp);
	}

	storage["Descriptors"] >> globalDescriptors;
	storage["Visibility"] >> visibility;
	storage["Times"] >> times;
	storage["Fremen"] >> fremen;
	if (debug) printf("Loaded %ld features from %i images.\n",globalPositions.size(),visibility.cols);
	int line = 0;
	if (fremen.empty() == false){
		frelementArray = (ELEMENT**) calloc(visibility.rows,sizeof(ELEMENT*));
		for (int i = 0;i< globalDescriptors.rows;i++)
		{
			if ((fremen.at<float>(line,0) != 0) || (fremen.at<float>(line,1) != 0) || (fremen.at<float>(line,2) != i)) printf("Error when loading FreMEn feature visibility.\n"); 
			frelementArray[i] = new ELEMENT(fremen.at<float>(line,3),fremen.at<float>(line,4),visibility.cols);
			line++;
			for (int j = 0;j< frelementArray[i]->order;j++){
				if ((fremen.at<float>(line,0) != i) || (fremen.at<float>(line,1) != j)) printf("Error when loading FreMEn feature visibility.\n"); 
				frelementArray[i]->frelements[j].amplitude = fremen.at<float>(line,2);
				frelementArray[i]->frelements[j].phase = fremen.at<float>(line,3);
				frelementArray[i]->frelements[j].frequency = fremen.at<float>(line,4);
				line++;
			}
		}
	}
	storage.release(); 
	totalPics = visibility.cols;
	return 0;
}

void CFeatureMap::binaryDescriptor(unsigned char *dst,unsigned char* in,int length)
{
	int cnt = 0;
	for (int i=0;i<length;i++){
		for (int j=0;j<8;j++) dst[cnt++] = ((in[i] & (128>>j)) != 0);
	}	
}

void CFeatureMap::addToMap(Mat& base,Mat& view, vector<DMatch>& matches,vector<KeyPoint> keypoints,float distanceFactor,bool crosscheck)
{
	int newInBag = 0;
	int k = numPics; 
	matches.clear();

	//no features detected in the image
	if (keypoints.size() == 0) return;

	//feature database is empty - populate it with image features 
	if (base.rows==0){
		for(unsigned int i=0; i < view.rows; i++)
		{
			cv::Mat newMat = view.row(i);
			base.push_back(newMat);
			globalPositions.push_back(keypoints[i]);
			newInBag++;
		}
		visibility = Mat::zeros(newInBag,totalPics,CV_8UC1);
		for(unsigned int j=0; j < base.rows; j++)visibility.at<char>(j,k) = 1;
		if (debug) fprintf(stdout,"Image %i Features %i Base %i Matched %ld New %i\n",k,view.rows,base.rows,matches.size(),newInBag);
		return;
	}

	//view=query,base=train
	distinctiveMatch(view,base, matches,distanceFactor,crosscheck);

	//perform matching 
	for(unsigned int j=0; j < base.rows; j++)visibility.at<char>(j,k) = 0;
	for(unsigned int j=0; j < view.rows; j++) keypoints[j].class_id = 0;  
	for(unsigned int i=0; i<matches.size() ; i++)
	{
		int qI=matches[i].queryIdx;
		if (matches[i].distance == -10000)
		{
			matches.erase(matches.begin()+i--);
			Mat newMat = view.row(qI);
			base.push_back(newMat);
			globalPositions.push_back(keypoints[qI]);
			Mat visMat = Mat::zeros(1,totalPics,CV_8UC1);
			visMat.at<char>(0,k) = 1;
			visibility.push_back(visMat);
			newInBag++;
		}
		else
		{
			if (matches[i].distance < 0) fprintf(stdout,"WARNING: %f\n",matches[i].distance);
			int tI=matches[i].trainIdx;
			int hD = fabs(keypoints[qI].pt.x-globalPositions[tI].pt.x);
			int vD = fabs(keypoints[qI].pt.y-globalPositions[tI].pt.y);
			if (vD < VERTICAL_LIMIT && hD < HORIZONTAL_LIMIT)
			{
				visibility.at<char>(tI,k) = 1;
			}else{
				visibility.at<char>(tI,k) = 0;
				matches.erase(matches.begin()+i--);
			}
		}
	}
	/*for(unsigned int j=0; j < view.rows; j++){
		if (keypoints[j].class_id == -10)
		{
			Mat newMat = view.row(j);
			base.push_back(newMat);
			globalPositions.push_back(keypoints[j]);
			Mat visMat = Mat::zeros(1,totalPics,CV_8UC1);
			visMat.at<char>(0,k) = 1;
			visibility.push_back(visMat);
			newInBag++;
		} 
	}*/
	if (debug) fprintf(stdout,"Image %i Features %i Base %i Matched %ld New %i\n",k,view.rows,base.rows,matches.size(),newInBag);
}

int CFeatureMap::match(CFeatureMap* map,bool geometry,int *tentative)
{
	imageDescriptors = map->imageDescriptors;
	imagePositions = map->imagePositions;
	return match(currentDescriptors,imageDescriptors,matches,imagePositions,geometry,tentative);
}

int CFeatureMap::match(Mat& base,Mat& view, vector<DMatch>& matches,vector<KeyPoint> keypoints,bool geometry,int *tentative)
{
	//no features detected in the image
	matches.clear();
	if (keypoints.size() == 0) return 0;
	int matched = 0;

	//view=query,base=train
	distinctiveMatch(view,base, matches,0.70,false);
	*tentative = matches.size();
	if (geometry){
		for(unsigned int i=0; i<matches.size() ; i++)
		{
			//fprintf(stdout,"R:%f\n",allMatches1to2[i][0].distance);
			int qI=matches[i].queryIdx;
			int tI=matches[i].trainIdx;
			int hD = fabs(keypoints[qI].pt.x-currentPositions[tI].pt.x);
			int vD = fabs(keypoints[qI].pt.y-currentPositions[tI].pt.y);
			if (vD < VERTICAL_LIMIT && hD < HORIZONTAL_LIMIT && matches[i].distance!=-10000) matched++; else matches.erase(matches.begin()+i--);
		}
	}
	else
	{
		for(unsigned int i=0; i<matches.size() ; i++)
		{
			if (matches[i].distance!=-10000) matched++; else matches.erase(matches.begin()+i--);
		}
	}
	matched = matches.size();
	if (debug) fprintf(stdout,"Matched %i %ld\n",matched,matches.size());
	return matched;
}
