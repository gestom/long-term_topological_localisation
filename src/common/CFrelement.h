#ifndef CFRELEMENT_H
#define CFRELEMENT_H

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <string.h>
#include "CTimer.h"

#define MAX_ADAPTIVE_ORDER 5
	
/**
@author Tom Krajnik
*/

using namespace std;

typedef struct
{
	float amplitude;
	float phase;
	float frequency;
}SFrelement;

class CFrelement
{
	public:
		CFrelement();
		CFrelement(int orderi,float gaini,int length);
		~CFrelement();

		//void add(unsigned int time,unsigned char data);
		void build(unsigned int* times,unsigned char* signal,int length,int orderi);
		float evaluate(unsigned int* times,unsigned char* signal,int length,int orderi);
		unsigned char retrieve(unsigned int timeStamp);
		void update(int modelOrder);
		float estimate(int time,int maxOrder);
		void print(bool verbose=true);

		int save(FILE* file,bool lossy = false);
		int load(FILE* file);
		int save(char* name,bool lossy = false);
		int load(char* name);

		SFrelement *frelements;
		unsigned int *outlierSet;
		unsigned int outliers;
		unsigned char order;
		float gain;
		unsigned int timePeriod;
		unsigned int lastTime;
		int signalLength;
};

#endif
