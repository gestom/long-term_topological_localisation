#ifndef CTIMENONE_H
#define CTIMENONE_H

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include "CTimer.h"
#include "CTemporal.h"

#define MAX_ID_LENGTH 100
	
using namespace std;

class CTimeNone: public CTemporal
{
	public:
		CTimeNone(const char* idd);
		~CTimeNone();

		void init(int iMaxPeriod,int elements,int numActivities);

		//adds a serie of measurements to the data
		int add(uint32_t time,float state);

		float estimate(uint32_t time);

		float predict(uint32_t time);

		void update(int modelOrder);
		void print(bool verbose=true);

		char id[MAX_ID_LENGTH];
		int save(FILE* file,bool lossy = false);
		int load(FILE* file);
		int save(char* name,bool lossy = false);
		int load(char* name);
		
		float estimation;

};

#endif
