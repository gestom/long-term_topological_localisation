#include "CTomasuvPokus.h"

CTomasuvPokus::CTomasuvPokus()
{
	for(int i = 0; i < 12; i++)
	{	
		probability[i] = 0;
	} 
}

CTomasuvPokus::CTomasuvPokus(int orderi,float gaini,int length)
{
	for(int i = 0; i < 12; i++)
	{	
		probability[i] = 0;
	}  
}

CTomasuvPokus::~CTomasuvPokus()
{
}

void CTomasuvPokus::build(unsigned int* times, unsigned char* signal, int length, int orderi)
{
	for (int i = 0;i<length;i++) 
	{
		probability[i] = signal[i];
	}
}

float CTomasuvPokus::estimate(int time, int maxOrder)
{
	int index = time / 7200;
	return probability[index];
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



