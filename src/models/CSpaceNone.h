#ifndef CSPACENONE_H
#define CSPACENONE_H

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include "CTimer.h"
#include "CSpatial.h"

using namespace std;

class CSpaceNone: public CSpatial
{
	public:
		CSpaceNone(const char* idd);
		~CSpaceNone();

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
		float estimation;		
};

#endif
