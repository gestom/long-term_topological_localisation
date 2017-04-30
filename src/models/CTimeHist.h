#ifndef CTIMEHIST_H
#define CTIMEHIST_H

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

class CTimeHist: public CTemporal
{
	public:
		CTimeHist(const char* idd);
		~CTimeHist();

		void init(int iMaxPeriod,int elements,int numActivities);

		//adds a serie of measurements to the data
		int add(uint32_t time,float state);

		//estimates the probability for the given times - using stored histogram 
		float estimate(uint32_t time);

		//predicts the probability for the given times - using updated histogram 
		float predict(uint32_t time);

		void update(int modelOrder);
		void print(bool verbose=true);

		char id[MAX_ID_LENGTH];
		int save(FILE* file,bool lossy = false);
		int load(FILE* file);
		int save(char* name,bool lossy = false);
		int load(char* name);
		
		float *storedHistogram;
		float *predictHistogram;
		int *measurementsHistogram;

		int numModels;
};

#endif
