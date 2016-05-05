#ifndef CIMAGE_H
#define CIMAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#define THICK_CROSS

/**
@author Tom Krajnik
*/
class CRawImage
{
public:

  CRawImage();
  ~CRawImage();
  void saveBmp(const char* name);
  void saveBmp();
  bool loadBmp(const char* name);
  void swap();
  void merge(CRawImage* first,CRawImage* second,int offsetX,int offsetY);
  void plotLine(int x,int y);
  void plotCenter();
  int average();

  int  getSaveNumber();

  double getOverallBrightness(bool upperHalf);
  
	  
  int width;
  int height;
  int palette;
  int size;
  int bpp;

  unsigned char* data;
  int numSaved;
};

#endif
