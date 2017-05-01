#ifndef CPERGAM_H
#define CPERGAM_H

#include <iostream>
#include <vector>
#include <complex>	
#include <algorithm> 
#include <iterator> 
#include <string.h>
#include "CTimer.h"
#include "CTemporal.h"
	
/**
@author Tom Krajnik
*/

#define MAX_ID_LENGTH 100

using namespace std;

typedef struct
{
	float mean;
	float sigma;
	float weight;
	float *estimate;
	unsigned int frequency;
}SPerGaM;

class CPerGaM: public CTemporal
{
public:
  CPerGaM(int id);
  ~CPerGaM();

  //adds a serie of measurements to the data
  int add(uint32_t time,float state);
  void init(int iMaxPeriod,int elements,int numActivities);

  //estimates the probability for the given times - using stored histogram 
  float estimate(uint32_t time);

  //predicts the probability for the given times - using updated histogram 
  float predict(uint32_t time);

  void update(int maxOrder,unsigned int* times = NULL,float* signal = NULL,int length = 0);
  void print(bool verbose=true);

  int id;
  int exportToArray(double* array,int maxLen);
  int importFromArray(double* array,int len);
  int save(FILE* file,bool lossy = false);
  int load(FILE* file);
  int save(char* name,bool lossy = false);
  int load(char* name);
  int loadTxt(FILE* file);

  SPerGaM *gaussian;
  float *storedHistogram;
  int offset;
  float gain;
  int numBins; 
};

#endif
