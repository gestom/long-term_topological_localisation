#ifndef CTOMASUVPOKUS_H
#define CTOMASUVPOKUS_H

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <string.h>
#include "CFrelement.h"

#define MAX_ADAPTIVE_ORDER 5

using namespace std;

class CTomasuvPokus: public CFrelement
{
	public:
		CTomasuvPokus();
		CTomasuvPokus(int orderi,float gaini,int length);
		~CTomasuvPokus();
		void build(unsigned int* times, unsigned char* signal, int lenght, int orderi);
		float estimate(int time, int maxOrder);
		int save(FILE* file, bool lossy = false);
		int load(FILE* file);
		int probability[12];
};

#endif
		
