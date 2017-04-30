#include "CTimeHist.h"

using namespace std;

CTimeHist::CTimeHist(const char* idd)
{
	strcpy(id,idd);
	firstTime = -1;
	lastTime = -1;
	measurements = 0;
	maxPeriod = 0;
	numElements = 0;
	type = TT_HISTOGRAM;
}

void CTimeHist::init(int iMaxPeriod,int elements,int models)
{
	maxPeriod = iMaxPeriod;
	numElements = elements;
	numModels = models;
	predictHistogram = (float*) malloc(sizeof(float)*numElements);	
	storedHistogram = (float*) malloc(sizeof(float)*numElements);	
	measurementsHistogram = (int*) malloc(sizeof(int)*numElements);	
	for (int i=0;i<numElements;i++){
		predictHistogram[i] = 1.0/numModels; 
		measurementsHistogram[i] = 0;
	}
}

CTimeHist::~CTimeHist()
{
	free(predictHistogram);
	free(measurementsHistogram);
	free(storedHistogram);
}

// adds new state observations at given times
int CTimeHist::add(uint32_t time,float state)
{
	if (measurements == 0) firstTime = time;
	lastTime = time;
	int index = ((time%maxPeriod)*numElements/maxPeriod)%numElements;
	storedHistogram[index] += state;
	measurementsHistogram[index] ++;
	measurements++;

	return 0; 
}

/*not required in incremental version*/
void CTimeHist::update(int modelOrder)
{
	for (int i=0;i<numElements;i++){
		 if (measurementsHistogram[i] > 0) predictHistogram[i] = storedHistogram[i]/measurementsHistogram[i];
	}
}

/*text representation of the fremen model*/
void CTimeHist::print(bool verbose)
{
	std::cout << "Model " << id << " Size: " << measurements << " ";
	if (verbose){
		printf("Bin values: "); 
		for (int i = 0;i<numElements;i++) printf("%.3f ",predictHistogram[i]);
	}
	printf("\n"); 
}

float CTimeHist::estimate(uint32_t time)
{
	int index = ((time%maxPeriod)*numElements/maxPeriod)%numElements;
	float estimate = 1.0/numModels;
	if (measurementsHistogram[index] > 0) estimate = storedHistogram[index]/measurementsHistogram[index];
	float saturation = 0.001;
	if (estimate > 1.0-saturation) estimate =  1.0-saturation;
	if (estimate < 0.0+saturation) estimate =  0.0+saturation;
	return estimate;
}

float CTimeHist::predict(uint32_t time)
{
	float estimate = predictHistogram[((time%maxPeriod)*numElements/maxPeriod)%numElements];
	float saturation = 0.001;
	if (estimate > 1.0-saturation) estimate =  1.0-saturation;
	if (estimate < 0.0+saturation) estimate =  0.0+saturation;
	return estimate;
}
int CTimeHist::save(char* name,bool lossy)
{
	FILE* file = fopen(name,"w");
	save(file);
	fclose(file);
	return 0;
}

int CTimeHist::load(char* name)
{
	FILE* file = fopen(name,"r");
	load(file);
	fclose(file);
	return 0;
}


int CTimeHist::save(FILE* file,bool lossy)
{
	return -1;
}

int CTimeHist::load(FILE* file)
{
	return -1;
}


