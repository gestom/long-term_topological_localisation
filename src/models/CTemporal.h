#ifndef CTEMPORAL_H
#define CTEMPORAL_H

#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include "CTimer.h"

#define MAX_ID_LENGTH 100
	
using namespace std;

typedef enum{
	TT_FREMEN = 0,
	TT_PERGAM,
	TT_MEAN,
	TT_HISTOGRAM,
	TT_NONE	
}ETemporalType;

class CTemporal
{
	public:

		//adds a serie of measurements to the data
		virtual int add(uint32_t time,float state) = 0;

		//initialize
		virtual void init(int maxPeriod,int elements,int numActivities) = 0;

		//estimates the probability for the given times 
		virtual float estimate(uint32_t time) = 0;
		virtual float predict(uint32_t time) = 0;

		virtual void update(int modelOrder) = 0;
		virtual void print(bool verbose=true) = 0;

		virtual int save(FILE* file,bool lossy = false) = 0;
		virtual int load(FILE* file) = 0;
		virtual int save(char* name,bool lossy = false) = 0;
		virtual int load(char* name) = 0;

		int measurements;
		int64_t firstTime;
		int64_t  lastTime;
		int numElements;
		int maxPeriod;
		ETemporalType type;
};

#endif
