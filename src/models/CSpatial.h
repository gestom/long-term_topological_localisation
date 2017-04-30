#ifndef CSPATIAL_H
#define CSPATIAL_H

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include "CTimer.h"

#define MAX_ID_LENGTH 100

typedef enum{
	ST_HISTOGRAM = 0,
	ST_NONE
}ESpatialType;

using namespace std;

class CSpatial
{
	public:

		//adds a serie of measurements to the data
		virtual int add(int room,float state) = 0;

		//initialize
		virtual void init(int inumElements,int numActivities) = 0;

		//estimates the probability for the given times 
		virtual float estimate(int room) = 0;
		virtual float predict(int room) = 0;

		virtual void update(int modelOrder) = 0;
		virtual void print(bool verbose=true) = 0;

		virtual int save(FILE* file,bool lossy = false) = 0;
		virtual int load(FILE* file) = 0;
		virtual int save(char* name,bool lossy = false) = 0;
		virtual int load(char* name) = 0;

		int measurements;
		int numElements;
		ESpatialType type;
};

#endif
