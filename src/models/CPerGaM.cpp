#include "CPerGaM.h"

using namespace std;

CPerGaM::CPerGaM(const char* name)
{
	numBins = 1440;
	strcpy(id,name);
	firstTime = -1;
	lastTime = -1;
	measurements = 0;
	maxPeriod = 0;
	numElements = 0;
	type = TT_PERGAM;
	gaussian = NULL;
	storedHistogram = NULL;
	offset = 0;
	gain = 1.0/12.0; 
}

void CPerGaM::init(int iMaxPeriod,int elements,int numActivities)
{
	maxPeriod = iMaxPeriod;
	numElements = elements;
	gaussian = (SPerGaM*) malloc(sizeof(SPerGaM)*numElements);	
	storedHistogram = (float*) malloc(sizeof(float)*maxPeriod);	
	for (int i=0;i<numElements;i++)
	{
		gaussian[i].mean = (i+0.5)*maxPeriod/elements;
		gaussian[i].sigma = maxPeriod;
		gaussian[i].weight = 1.0/12.0/elements;
		gaussian[i].frequency = maxPeriod;
	}
	for (int i=0;i<numElements;i++) storedHistogram[i] = 1.0/numActivities;	
}

CPerGaM::~CPerGaM()
{
	free(storedHistogram);
}

// adds new state observations at given times
int CPerGaM::add(uint32_t time,float state)
{
	if (measurements == 0) firstTime = time;
	lastTime = time;
	storedHistogram[((time%maxPeriod)*numBins/maxPeriod)%numBins] += state;
	measurements++;
	return 0; 
}

void CPerGaM::update(int order)
{
	numElements = order;
	float *input = (float*)calloc(numBins,sizeof(float));
	float *refined = (float*)calloc(numBins,sizeof(float));
	float *output = (float*)calloc(numBins,sizeof(float));
	float *smooth = (float*)calloc(numBins,sizeof(float));
	offset = 0;
	gain = 0;
	//summarize over all days
	for (int i=0;i<numBins;i++){
	       	smooth[i]=0;
		input[i]=(float)storedHistogram[i]/measurements*numBins;
		gain+=input[i];
	}

	//smooth it and find a global minimum
	float sum = 0;
	int smoothLength = numBins/2;
	for (int i=numBins-smoothLength;i<numBins;i++) sum+=input[i]; 
	for (int i=numBins;i<numBins*2;i++){
		 sum = sum - input[(i-smoothLength)%numBins] + input[i%numBins];
		 smooth[(i-smoothLength/2)%numBins] = sum; 
	}

	float minimum = 10.0;
	for (int i=0;i<numBins;i++){
		 smooth[i]=smooth[i]/smoothLength;
		 if (minimum > smooth[i])
		 {
			minimum = smooth[i];
			offset = i;
		 }
	}
	for (int i=0;i<numBins;i++) refined[i]=input[(i+offset)%numBins];
	sum = 0;
	for (int i=0;i<numBins;i++) sum+=input[i];

	//initialize Gaussianu

	free(gaussian); 
	int meanDistance = numBins/(numElements+1);
	gaussian = (SPerGaM*) calloc(numElements,sizeof(SPerGaM));
	for (int i = 0;i<order;i++)
	{
	 	gaussian[i].mean = meanDistance*(i+1);
	 	gaussian[i].weight = 1.0/numElements;
	 	gaussian[i].sigma = numBins/numElements/2;
	 	gaussian[i].estimate = (float*)calloc(numBins,sizeof(float));
	}

	//expectation step - calculate Gaussians
	float *y;
	float w,m,s;
	for (int steps = 0;steps<100;steps++){
		memset(output,0,numBins*sizeof(float));
		for (int i = 0;i<order;i++)
		{
			y = gaussian[i].estimate;
			w = gaussian[i].weight;
			m = gaussian[i].mean;
			s = gaussian[i].sigma;
			for (int t=0;t<numBins;t++){
				y[t] = w*exp(-(t-m)*(t-m)/(2*s*s))/(sqrt(2*M_PI)*s);
				output[t]+=y[t];
			}
		}	
		//maximization step
		for (int i = 0;i<order;i++)
		{
			w = 0;
			m = 0;
			s = 0;
			y = gaussian[i].estimate;
			for (int t=0;t<numBins;t++)
			{
				y[t]=y[t]/output[t]*refined[t]/sum;
				m+=t*y[t];	
				w+=y[t];	
			}
			m = m/w;
			for (int t=0;t<numBins;t++) s+= (t-m)*(t-m)*y[t];
			gaussian[i].weight = w;
			gaussian[i].mean = m;
			gaussian[i].sigma = sqrt(s/w);
			if (gaussian[i].sigma == 0) gaussian[i].sigma = 1;
		}
	}
	memset(output,0,numBins*sizeof(float));
	for (int i = 0;i<numElements;i++)
	{
		y = gaussian[i].estimate;
		w = gaussian[i].weight;
		m = gaussian[i].mean;
		s = gaussian[i].sigma;
		for (int t=0;t<numBins;t++){
			y[t] = w*exp(-(t-m)*(t-m)/(2*s*s))/(sqrt(2*M_PI)*s)*sum;
			output[t]+=y[t];
		}
	}

	//print the summary
//	for (int i=0;i<numBins;i++) printf("SUMAPIC: %s %f %f %f\n",id,output[i]*measurements,refined[i],smooth[i]);
//	printf("OFFSET %s %i %f %f\n",id,offset,gaussian[0].mean,gaussian[0].sigma);
		
//	for (int i=0;i<numBins;i++) printf("SUMAPIC: %s %f %f\n",id,input[i],estimate(i*60));
	free(input);
	free(refined);
	free(smooth);
	free(output);
//	print(false);
	return;
}

float CPerGaM::estimate(uint32_t time)
{
	CTimer timer;
	timer.start();
	time = ((time/60+numBins-offset)%numBins);
	int timi;
	float probability = 0; 
	for (int i=0;i<numElements;i++)
	{
		float scale = gaussian[i].weight/(sqrt(2*M_PI)*gaussian[i].sigma);
		timi = fabs(time-gaussian[i].mean);
		float exponent = (timi)/gaussian[i].sigma;
		probability += scale*exp(-exponent*exponent/2);
	}
	probability = probability*gain;//*measurements;
	if (fpclassify(probability) != FP_NORMAL) return 0;
	/*for (int i = 0;i<phaseShift;i++) probability[i] = signal[signalLength-phaseShift+i]; 
	for (int i = 0;i<signalLength-phaseShift;i++) probability[i+phaseShift] = signal[i]; 
	for (int i=0;i<repeats;i++) memcpy(&signal[signalLength*i],probability,signalLength*sizeof(float));*/

	float saturation = 0.01;
	if (probability > 1.0-saturation) probability =  1.0-saturation;
	if (probability < 0.0+saturation) probability =  0.0+saturation;
	return probability;
}

float CPerGaM::predict(uint32_t time)
{
	return estimate(time);
}

void CPerGaM::print(bool verbose)
{
	for (int i=0;i<numElements;i++) printf("%.3f ",gaussian[i].mean);
	printf("\n");
	for (int i=0;i<numElements;i++) printf("%.3f ",gaussian[i].sigma);
	printf("\n");
	for (int i=0;i<numElements;i++) printf("%.3f ",gaussian[i].weight);
	printf("\n");
}

int CPerGaM::save(char* name,bool lossy)
{
	FILE* file = fopen(name,"w");
	save(file);
	fclose(file);
	return 0;
}

int CPerGaM::load(char* name)
{
	FILE* file = fopen(name,"r");
	load(file);
	fclose(file);
	return 0;
}

int CPerGaM::save(FILE* file,bool lossy)
{
	return -1;
}

int CPerGaM::load(FILE* file)
{
	return -1;
}
