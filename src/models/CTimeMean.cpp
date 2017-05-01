#include "CTimeMean.h"

using namespace std;

CTimeMean::CTimeMean(int idd)
{
	id=idd;
	firstTime = -1;
	lastTime = -1;
	measurements = 0;
	maxPeriod = 0;
	numElements = 0;
	positive = 0;
	type = TT_MEAN;
}

void CTimeMean::init(int iMaxPeriod,int elements,int numActivities)
{
	maxPeriod = iMaxPeriod;
	numElements = 1;
	estimation = 1.0/numActivities; 
}

CTimeMean::~CTimeMean()
{
}


// adds new state observations at given times
int CTimeMean::add(uint32_t time,float state)
{
	if (measurements == 0) firstTime = time;
	lastTime = time;
	positive=positive+state;
	measurements++;
	return 0; 
}

/*not required in incremental version*/
void CTimeMean::update(int modelOrder,unsigned int* times,float* signal,int length)
{
	if (measurements  > 0) estimation = positive/measurements;
}

/*text representation of the fremen model*/
void CTimeMean::print(bool verbose)
{
	std::cout << "Model " << id << " Size: " << measurements << " ";
	if (verbose){
		printf("Mean: "); 
		printf("%.3f ",positive/measurements);
	}
	printf("\n"); 
}

float CTimeMean::estimate(uint32_t time)
{
	float estimate = estimation;//positive/measurements; 
	float saturation = 0.001;
	if (estimate > 1.0-saturation) estimate =  1.0-saturation;
	if (estimate < 0.0+saturation) estimate =  0.0+saturation;
	return estimate;
}

float CTimeMean::predict(uint32_t time)
{
	float estimate = estimation; 
	float saturation = 0.001;
	if (estimate > 1.0-saturation) estimate =  1.0-saturation;
	if (estimate < 0.0+saturation) estimate =  0.0+saturation;
	return estimate;
}
int CTimeMean::save(char* name,bool lossy)
{
	FILE* file = fopen(name,"w");
	save(file);
	fclose(file);
	return 0;
}

int CTimeMean::load(char* name)
{
	FILE* file = fopen(name,"r");
	load(file);
	fclose(file);
	return 0;
}


int CTimeMean::save(FILE* file,bool lossy)
{
	return -1;
}

int CTimeMean::load(FILE* file)
{
	return -1;
}

int CTimeMean::exportToArray(double* array,int maxLen)
{
	int pos = 0;
	array[pos++] = type;
	array[pos++] = positive;
	array[pos++] = id;
	array[pos++] = measurements;
	return pos;
}

int CTimeMean::importFromArray(double* array,int len)
{
	int pos = 0;
	type = (ETemporalType)array[pos++];
	if (type != TT_MEAN) fprintf(stderr,"Error loading the model, type mismatch.\n");
	positive = array[pos++];
	id = array[pos++];  
	measurements = array[pos++]; 
	return pos;
}
