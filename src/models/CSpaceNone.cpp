#include "CSpaceNone.h"

using namespace std;

CSpaceNone::CSpaceNone(const char* idd)
{
	strcpy(id,idd);
	measurements = 0;
	numElements = 0;
	type = ST_NONE;
}

void CSpaceNone::init(int elements,int numActivities)
{
	numElements = elements;
	estimation = 1.0/numActivities;
}

CSpaceNone::~CSpaceNone()
{
}

// adds new state observations at given times
int CSpaceNone::add(int room,float state)
{
	measurements++;
	return 0; 
}

/*not required in incremental version*/
void CSpaceNone::update(int modelOrder)
{
}

/*text representation of the fremen model*/
void CSpaceNone::print(bool verbose)
{
	std::cout << "Model " << id << " Size: " << measurements << " ";
	if (verbose) printf("Value: %.3f \n",estimation); 
}

float CSpaceNone::estimate(int room)
{
	return estimation;
}

float CSpaceNone::predict(int room)
{
	return estimation;
}

int CSpaceNone::save(char* name,bool lossy)
{
	FILE* file = fopen(name,"w");
	save(file);
	fclose(file);
	return 0;
}

int CSpaceNone::load(char* name)
{
	FILE* file = fopen(name,"r");
	load(file);
	fclose(file);
	return 0;
}


int CSpaceNone::save(FILE* file,bool lossy)
{
	return -1;
}

int CSpaceNone::load(FILE* file)
{
	return -1;
}


