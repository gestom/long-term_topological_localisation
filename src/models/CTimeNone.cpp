#include "CTimeNone.h"

using namespace std;

CTimeNone::CTimeNone(int idd)
{
	id=idd;
	measurements = 0;
	numElements = 0;
	type = TT_NONE;
}

void CTimeNone::init(int iMaxPeriod,int elements,int numActivities)
{
	maxPeriod = iMaxPeriod;
	numElements = 1;
	estimation = 1.0/numActivities; 
}

CTimeNone::~CTimeNone()
{
}

// adds new state observations at given times
int CTimeNone::add(uint32_t time,float state)
{
	measurements++;
	return 0; 
}

void CTimeNone::update(int modelOrder,unsigned int* times,float* signal,int length)
{
}

/*text representation of the fremen model*/
void CTimeNone::print(bool verbose)
{
	std::cout << "Model " << id << " Size: " << measurements << " ";
	if (verbose) printf("Value: %.3f \n",estimation); 
}

float CTimeNone::estimate(uint32_t time)
{
	return estimation;
}

float CTimeNone::predict(uint32_t time)
{
	return estimation;
}
int CTimeNone::save(char* name,bool lossy)
{
	FILE* file = fopen(name,"w");
	save(file);
	fclose(file);
	return 0;
}

int CTimeNone::load(char* name)
{
	FILE* file = fopen(name,"r");
	load(file);
	fclose(file);
	return 0;
}


int CTimeNone::save(FILE* file,bool lossy)
{
	return -1;
}

int CTimeNone::load(FILE* file)
{
	return -1;
}


int CTimeNone::exportToArray(double* array,int maxLen)
{
	int pos = 0;
	array[pos++] = type;
	array[pos++] = estimation;
	array[pos++] = id;
	array[pos++] = measurements;
	return pos;
}

int CTimeNone::importFromArray(double* array,int len)
{
	int pos = 0;
	type = (ETemporalType)array[pos++];
	if (type != TT_NONE) fprintf(stderr,"Error loading the model, type mismatch.\n");
	estimation = array[pos++];
	id = array[pos++];  
	measurements = array[pos++]; 
	return pos;
}
