#include "CTimeAdaptiveHist.h"
#define BINS_PER_DAY 1440 //the number of minutes of a day
#define MEASURE_DEF_THRES 100
using namespace std;

CTimeAdaptiveHist::CTimeAdaptiveHist(int idd)
{
    id = idd;
    firstTime = -1;
    lastTime = -1;
    measurements = 0;
    maxPeriod = 0;
    numElements = 0;
    type = TT_ADAPTIVE;
}

void CTimeAdaptiveHist::init(int imaxPeriod,int elements,int numActivities)
{
    maxPeriod = imaxPeriod;
    numElements = BINS_PER_DAY;
    def_sample_threshold=elements;
    predictHistogram = (float*) malloc(sizeof(float)*numElements);
    storedHistogram = (float*) malloc(sizeof(float)*numElements);
    measurementHistogram = (uint32_t*) malloc(sizeof(uint32_t)*numElements);
    for (int i=0;i<numElements;i++)
    {
        measurementHistogram[i]=0;
        predictHistogram[i] = storedHistogram[i] = 0.5;
    }
}

CTimeAdaptiveHist::~CTimeAdaptiveHist()
{
    free(predictHistogram);
    free(storedHistogram);
}

// adds new state observations at given times
int CTimeAdaptiveHist::add(uint32_t time,float state)
{
    if (measurements == 0) firstTime = time;
    lastTime = time;

    storedHistogram[((time%maxPeriod)*numElements/maxPeriod)%numElements] += state;
    measurementHistogram[((time%maxPeriod)*numElements/maxPeriod)%numElements]++;
    measurements++;

    return 0;
}

/*not required in incremental version*/
void CTimeAdaptiveHist::update(int modelOrder,unsigned int* times,float* signal,int length)
{
    for (int i=0;i<numElements;i++) predictHistogram[i] = storedHistogram[i];
}

/*text representation of the fremen model*/
void CTimeAdaptiveHist::print(bool verbose)
{
    std::cout << "Model " << id << " Size: " << measurements << " ";
    if (verbose){
        printf("Bin values: ");
        for (int i = 0;i<numElements;i++) printf("%.3f ",storedHistogram[i]);
    }
    printf("\n");
}

float CTimeAdaptiveHist::estimate(uint32_t time)
{
    float estimate = storedHistogram[((time%maxPeriod)*numElements/maxPeriod)%numElements];
    float saturation = 0.001;
    if (estimate > 1.0-saturation) estimate =  1.0-saturation;
    if (estimate < 0.0+saturation) estimate =  0.0+saturation;
    return estimate;
}

float CTimeAdaptiveHist::predict(uint32_t time,uint32_t sample_thres)
{
    int center=((time%maxPeriod)*numElements/maxPeriod)%numElements;
    uint32_t sum=measurementHistogram[center];
    float estimate = predictHistogram[center]*(float)measurementHistogram[center];
    int i=0;
    while(sum<sample_thres)
    {
        i++;
        sum+=measurementHistogram[(center+i)%numElements];
        sum+=measurementHistogram[(center-i)%numElements];
        estimate+=predictHistogram[(center+i)%numElements]*(float)measurementHistogram[(center+i)%numElements];
        estimate+=predictHistogram[(center-i)%numElements]*(float)measurementHistogram[(center-i)%numElements];
    }
    estimate=estimate/(float)sum;
    float saturation = 0.001;
    if (estimate > 1.0-saturation) estimate =  1.0-saturation;
    if (estimate < 0.0+saturation) estimate =  0.0+saturation;
    return estimate;
}
float CTimeAdaptiveHist::predict(uint32_t time)
{
    return CTimeAdaptiveHist::predict(time,(uint32_t)def_sample_threshold);
}
int CTimeAdaptiveHist::save(char* name,bool lossy)
{
    FILE* file = fopen(name,"w");
    save(file);
    fclose(file);
    return 0;
}

int CTimeAdaptiveHist::load(char* name)
{
    FILE* file = fopen(name,"r");
    load(file);
    fclose(file);
    return 0;
}


int CTimeAdaptiveHist::save(FILE* file,bool lossy)
{
    return -1;
}

int CTimeAdaptiveHist::load(FILE* file)
{
    return -1;
}

int CTimeAdaptiveHist::exportToArray(double* array,int maxLen)
{
	int pos = 0;
	array[pos++] = type;
	array[pos++] = numElements;
	array[pos++] = id;
	array[pos++] = def_sample_threshold;
	array[pos++] = measurements;

	for (int i = 0;i<numElements && pos < MAX_TEMPORAL_MODEL_SIZE;i++) array[pos++] = storedHistogram[i];
	for (int i = 0;i<numElements && pos < MAX_TEMPORAL_MODEL_SIZE;i++) array[pos++] = measurementHistogram[i];

	if (pos == MAX_TEMPORAL_MODEL_SIZE) fprintf(stdout,"Could not save the model dur to its size\n");
	return pos;
}

int CTimeAdaptiveHist::importFromArray(double* array,int len)
{
	int pos = 0;
	type = (ETemporalType)array[pos++];
	numElements = array[pos++];
	if (type != TT_ADAPTIVE) fprintf(stderr,"Error loading the model, type mismatch.\n");
	id = array[pos++];  
	def_sample_threshold = array[pos++];
	measurements = array[pos++]; 
 
	for (int i = 0;i<numElements && pos < MAX_TEMPORAL_MODEL_SIZE;i++)storedHistogram[i]=array[pos++]; 
	for (int i = 0;i<numElements && pos < MAX_TEMPORAL_MODEL_SIZE;i++)measurementHistogram[i]=array[pos++]; 

	if (pos == MAX_TEMPORAL_MODEL_SIZE) fprintf(stdout,"Model was not properly saved before.\n");
	return pos;
}
