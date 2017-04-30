#include "CSpaceHist.h"

using namespace std;

CSpaceHist::CSpaceHist(const char* idd)
{
	strcpy(id,idd);
	measurements = 0;
	numElements = 0;
	type = ST_HISTOGRAM;
}

void CSpaceHist::init(int elements,int numActivities)
{
	numElements = elements;
	predictHistogram = (float*) malloc(sizeof(float)*numElements);	
	storedHistogram = (float*) malloc(sizeof(float)*numElements);	
	for (int i=0;i<numElements;i++) predictHistogram[i] = storedHistogram[i] = 1.0/numActivities; 
}

CSpaceHist::~CSpaceHist()
{
	free(predictHistogram);
	free(storedHistogram);
}

// adds new state observations at given times
int CSpaceHist::add(int room,float state)
{
	storedHistogram[room] = state;
	measurements++;

	return 0; 
}

/*not required in incremental version*/
void CSpaceHist::update(int modelOrder)
{
	for (int i=0;i<numElements;i++) predictHistogram[i] = storedHistogram[i];
}

/*text representation of the fremen model*/
void CSpaceHist::print(bool verbose)
{
	std::cout << "Model " << id << " Size: " << measurements << " ";
	if (verbose){
		printf("Bin values: "); 
		for (int i = 0;i<numElements;i++) printf("%.3f ",storedHistogram[i]);
	}
	printf("\n"); 
}

float CSpaceHist::estimate(int room)
{
	float estimate = storedHistogram[room];
	float saturation = 0.01;
	if (estimate > 1.0-saturation) estimate =  1.0-saturation;
	if (estimate < 0.0+saturation) estimate =  0.0+saturation;
	return estimate;
}

float CSpaceHist::predict(int room)
{
	float estimate = predictHistogram[room];
	float saturation = 0.01;
	if (estimate > 1.0-saturation) estimate =  1.0-saturation;
	if (estimate < 0.0+saturation) estimate =  0.0+saturation;
	return estimate;
}

int CSpaceHist::save(char* name,bool lossy)
{
	FILE* file = fopen(name,"w");
	save(file);
	fclose(file);
	return 0;
}

int CSpaceHist::load(char* name)
{
	FILE* file = fopen(name,"r");
	load(file);
	fclose(file);
	return 0;
}


int CSpaceHist::save(FILE* file,bool lossy)
{
	return -1;
}

int CSpaceHist::load(FILE* file)
{
	return -1;
}


