#ifndef CSPACEHIST_H
#define CSPACEHIST_H

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include "CTimer.h"
#include "CSpatial.h"

#define MAX_ID_LENGTH 100
	
using namespace std;

class CSpaceHist: public CSpatial
{
	public:
		CSpaceHist(const char* idd);
		~CSpaceHist();

		void init(int numRooms,int numActivities);

		//adds a serie of measurements to the data
		int add(int room,float state);


		//estimates the probability for the given times - using stored histogram 
		float estimate(int time);

		//predicts the probability for the given times - using updated histogram 
		float predict(int time);

		void update(int modelOrder);
		void print(bool verbose=true);

		char id[MAX_ID_LENGTH];
		int save(FILE* file,bool lossy = false);
		int load(FILE* file);
		int save(char* name,bool lossy = false);
		int load(char* name);
		
		float *storedHistogram;
		float *predictHistogram;

};

#endif
