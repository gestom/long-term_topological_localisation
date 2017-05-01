#ifndef CTIMEADAPTIVEHIST_H
#define CTIMEADAPTIVEHIST_H

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

class CTimeAdaptiveHist: public CTemporal
{

    public:
        CTimeAdaptiveHist(int idd);
        ~CTimeAdaptiveHist();

	void init(int iMaxPeriod,int elements,int numActivities);

        //adds a serie of measurements to the data
        int add(uint32_t time,float state);


        //estimates the probability for the given times - using stored histogram
        float estimate(uint32_t time);

        //predicts the probability for the given times - using updated histogram
        float predict(uint32_t time);//default threshold
        float predict(uint32_t time,uint32_t sample_thres);


	void update(int maxOrder,unsigned int* times = NULL,float* signal = NULL,int length = 0);
        void print(bool verbose=true);

	int exportToArray(double* array,int maxLen);
	int importFromArray(double* array,int len);
        int id;
        int save(FILE* file,bool lossy = false);
        int load(FILE* file);
        int save(char* name,bool lossy = false);
        int load(char* name);

        float *storedHistogram;
        float *predictHistogram;
        uint32_t *measurementHistogram;

    private:
        int def_sample_threshold;
};


#endif //CTIMEADAPTIVEHIST_H
