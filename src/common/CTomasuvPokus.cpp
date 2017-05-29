#include "CTomasuvPokus.h"

CTomasuvPokus::CTomasuvPokus()
{
	probability = 0.5; 
}

CTomasuvPokus::CTomasuvPokus(int orderi,float gaini,int length)
{
	probability = 0.5; 
}

CTomasuvPokus::~CTomasuvPokus()
{
}

void CTomasuvPokus::build(unsigned int* times, unsigned char* signal, int length, int orderi)
{
	int positive = 0;
	for (int i = 0;i<length;i++) positive += signal[i];
	probability = (float)(positive)/length;
}

float CTomasuvPokus::estimate(int time, int maxOrder)
{
	return probability;
}

int CTomasuvPokus::load(FILE* file)
{
	int ret =0;
	ret+=fread(&probability,sizeof(float),1,file);
	return ret;
}

int CTomasuvPokus::save(FILE* file,bool lossy)
{
	fwrite(&probability,sizeof(float),1,file);
	return 0;
}



