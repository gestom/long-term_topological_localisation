#include "CFrelement.h"

using namespace std;

int fremenSort(const void* i,const void* j) 
{
	 if (((SFrelement*)i)->amplitude < ((SFrelement*)j)->amplitude) return +1;
	 return -1;
}

CFrelement::CFrelement(int idi)
{
	id = idi;
	//initialization of the frequency set
	storedGain = 0.5;
	predictGain = 0.5;
	order = 0;
	firstTime = -1;
	lastTime = -1;
	measurements = 0;
	type = TT_FREMEN;
}

void CFrelement::init(int iMaxPeriod,int elements,int numActivities)
{
	maxPeriod = 86400;
	numElements = 7*24;
	storedFrelements = (SFrelement*)malloc(sizeof(SFrelement)*numElements);
	predictFrelements = (SFrelement*)malloc(sizeof(SFrelement)*numElements);
	for (int i=0;i<numElements;i++) storedFrelements[i].amplitude = storedFrelements[i].phase = 0; 
	for (int i=0;i<numElements;i++) storedFrelements[i].period = (maxPeriod)/(i+1); 
}

CFrelement::~CFrelement()
{
	free(storedFrelements);
	free(predictFrelements);
}

// adds new state observations at given times
int CFrelement::add(uint32_t time,float state)
{
	if (measurements == 0)
	{
		for (int i = 0;i<numElements;i++){
			storedFrelements[i].realStates  = 0;
			storedFrelements[i].imagStates  = 0;
			storedFrelements[i].realBalance = 0; 
			storedFrelements[i].imagBalance = 0; 
		}
		firstTime = time;
	}else{
		if (time - lastTime < shortestTime) shortestTime = time-lastTime;
	}
	lastTime = time;

	//update the gains accordingly 
	float oldGain=storedGain;
	storedGain = (storedGain*measurements+state)/(measurements+1);

	//recalculate spectral balance - this is beneficial is the process period does not match the length of the data
	if (oldGain > 0){
		for (int i = 0;i<numElements;i++)
		{
			storedFrelements[i].realBalance  = storedGain*storedFrelements[i].realBalance/oldGain;
			storedFrelements[i].imagBalance  = storedGain*storedFrelements[i].imagBalance/oldGain;
		}
	}else{
		for (int i = 0;i<numElements;i++)
		{
			storedFrelements[i].realBalance  = 0;
			storedFrelements[i].imagBalance  = 0;
		}
	}


	float angle = 0;
	//recalculate the spectral components
	for (int i = 0;i<numElements;i++)
	{
		angle = 2*M_PI*(float)time/storedFrelements[i].period;
		storedFrelements[i].realStates   += state*cos(angle);
		storedFrelements[i].imagStates   += state*sin(angle);
		storedFrelements[i].realBalance  += storedGain*cos(angle);
		storedFrelements[i].imagBalance  += storedGain*sin(angle);
	}
	measurements++;

	return 0; 
}

/*not required in incremental version*/
void CFrelement::update(int modelOrder,unsigned int* times,float* signal,int length)
{
	//establish amplitudes and phase shifts
	float re,im;
	int duration = lastTime - firstTime;
	predictGain = storedGain;
	for (int i = 0;i<numElements;i++)
	{
		re = storedFrelements[i].realStates-storedFrelements[i].realBalance;
		im = storedFrelements[i].imagStates-storedFrelements[i].imagBalance;
		if (1.5*storedFrelements[i].period <= duration && storedFrelements[i].period > shortestTime*2) storedFrelements[i].amplitude = sqrt(re*re+im*im)/measurements; else storedFrelements[i].amplitude = 0;
		if (storedFrelements[i].amplitude < FREMEN_AMPLITUDE_THRESHOLD) storedFrelements[i].amplitude = 0;
		storedFrelements[i].phase = atan2(im,re);
	}

	//sort the spectral components
	qsort(storedFrelements,numElements,sizeof(SFrelement),fremenSort);

	if (modelOrder > numElements) modelOrder = numElements;
	order = modelOrder;
	for (int i = 0;i<order;i++) predictFrelements[i] = storedFrelements[i];

	/*if given this info, the model tries to determine optimal model order to prevent overfit*/	
	if (times != NULL && signal != NULL && length > 0){
		int bestOrder = 0;
		for (int o = 0;o<modelOrder;o++){
			float error = 0;
			float minError = length*10;
			order = 0;
			for (int i = 0;i<length;i++)
			{
				order = o;	
				error += fabs(estimate(times[i])-signal[i]);
			}
			if (error < minError)
			{
				minError = error;
				bestOrder = o;
			}
		}
		order = bestOrder;
	}
}

/*text representation of the fremen model*/
void CFrelement::print(bool verbose)
{
	std::cout << "Model: " << id << " Prior: " << predictGain << " Size: " << measurements << " ";
	if (verbose){
		for (int i = 0;i<order;i++){
			std::cout << "Frelement " << i << " " << predictFrelements[i].amplitude << " " << predictFrelements[i].phase << " " << predictFrelements[i].period << " ";
		}
	}
	std::cout << endl; 
}

float CFrelement::estimate(uint32_t time)
{
	float saturation = 0.01;
	float estimate =  predictGain;
	for (int i = 0;i<order;i++) estimate+=2*predictFrelements[i].amplitude*cos(time/predictFrelements[i].period*2*M_PI-predictFrelements[i].phase);
	if (estimate > 1.0-saturation) estimate =  1.0-saturation;
	if (estimate < 0.0+saturation) estimate =  0.0+saturation;
	return estimate;
}


float CFrelement::predict(uint32_t time)
{
	float saturation = 0.01;
	float estimate =  predictGain;
	for (int i = 0;i<order;i++) estimate+=2*predictFrelements[i].amplitude*cos(time/predictFrelements[i].period*2*M_PI-predictFrelements[i].phase);
	if (estimate > 1.0-saturation) estimate =  1.0-saturation;
	if (estimate < 0.0+saturation) estimate =  0.0+saturation;
	return estimate;
}

int CFrelement::save(char* name,bool lossy)
{
	FILE* file = fopen(name,"w");
	save(file);
	fclose(file);
	return 0;
}

int CFrelement::importFromArray(double* array,int len)
{
	int pos = 0;
	type = (ETemporalType)array[pos++];
	if (type != TT_FREMEN) fprintf(stderr,"Error loading the model, type mismatch.\n");
	order = array[pos++];        
	id = array[pos++];
	storedGain = array[pos++];
	predictGain = array[pos++];  
	numElements = array[pos++];  
	measurements = array[pos++]; 
	shortestTime = array[pos++];
	memcpy(&firstTime,&array[pos++],sizeof(double));
	memcpy(&lastTime,&array[pos++],sizeof(double));
	for (int i = 0;i<numElements;i++){
		storedFrelements[i].realStates = array[pos++];
		storedFrelements[i].imagStates = array[pos++];
		storedFrelements[i].realBalance = array[pos++];
		storedFrelements[i].imagBalance = array[pos++];
		storedFrelements[i].period = array[pos++];	
	}
	update(order);
	return 0;
}

int CFrelement::exportToArray(double* array,int maxLen)
{
	int pos = 0;
	array[pos++] = type;
	array[pos++] = order;
	array[pos++] = id;
	array[pos++] = storedGain;
	array[pos++] = predictGain;
	array[pos++] = numElements;
	array[pos++] = measurements;
	array[pos++] = shortestTime;
	memcpy(&array[pos++],&firstTime,sizeof(double));
	memcpy(&array[pos++],&lastTime,sizeof(double));
	for (int i = 0;i<numElements;i++){
		array[pos++] = storedFrelements[i].realStates;
		array[pos++] = storedFrelements[i].imagStates;
		array[pos++] = storedFrelements[i].realBalance;
		array[pos++] = storedFrelements[i].imagBalance;
		array[pos++] = storedFrelements[i].period;	
	}
	//printf("POPOPO: %ld %ld %i %i\n",sizeof(double),sizeof(int64_t),pos,8+5*numElements);
	return pos;
}

int CFrelement::load(char* name)
{
	FILE* file = fopen(name,"r");
	load(file);
	fclose(file);
	return 0;
}


int CFrelement::save(FILE* file,bool lossy)
{
	int frk = numElements;
	fwrite(&frk,sizeof(uint32_t),1,file);
	fwrite(&storedGain,sizeof(float),1,file);
	fwrite(storedFrelements,sizeof(SFrelement),numElements,file);
	return 0;
}

int CFrelement::load(FILE* file)
{
	int frk = numElements;
	fwrite(&frk,sizeof(uint32_t),1,file);
	fwrite(&storedGain,sizeof(float),1,file);
	fwrite(storedFrelements,sizeof(SFrelement),numElements,file);
	return 0;
}

