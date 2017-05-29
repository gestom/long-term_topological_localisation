#ifndef CFEATUREMAP_H
#define CFEATUREMAP_H

/**
@author Tom Krajnik
*/
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "CFrelement.h" 

using namespace std;
using namespace cv;

//matching restrictions
#define VERTICAL_LIMIT 50
#define HORIZONTAL_LIMIT 400
#define L2_NORM true 
class CFeatureMap
{
	public:
		CFeatureMap(int numImages = 0);
		~CFeatureMap();

		void save(const char* name);
		void loadReadable(const char* name);
		bool load(const char* name);
		void saveReadable(const char* name);
		void savePredictions(const char* baseName,int timeQuantum);
		int extract(Mat img,int number);
		int drawPredicted(Mat img);
		int drawCurrentMatches(Mat img1,Mat img2);
		void addToMap(Mat& base,Mat& view, vector<DMatch>& matches,vector<KeyPoint> keypoints,float  distanceFactor,bool crosscheck);
		void addToMap(unsigned int time,float distanceThreshold,bool crosscheck);
		void reidentify(unsigned int time,float distanceThreshold,bool crosscheck);
		int drawAllFeatures(Mat img);
		int drawReidentified(Mat img1,Mat img2);
		void distinctiveMatch(Mat& descriptors1,Mat& descriptors2, vector<DMatch>& matches,float distanceFactor,bool crosscheck);
		void fremenize(int order);
		void print();
		void binaryDescriptor(unsigned char *dst,unsigned char *in,int length);
		float predictThreshold(unsigned int time,float threshold,int order);
		float predictNumber(unsigned int time,int number,int order);
		int match(CFeatureMap* map,bool geomtery,int *tentative);
		int match(Mat& base,Mat& view, vector<DMatch>& matches,vector<KeyPoint> keypoints,bool geometry,int *tentative);
		void sortAndReduce(float threshold);
		void fremenTest(int order);
		bool debug;	

		vector<KeyPoint> globalPositions;
		Mat globalDescriptors;
		Mat visibility;
		Mat times; 

		vector<KeyPoint> currentPositions;
		Mat currentDescriptors;

		vector<KeyPoint> imagePositions;
		Mat imageDescriptors;
		vector<DMatch> matches;

		CFrelement** frelementArray;
		int numFeatures;
		int numPics;	
		int totalPics;
		unsigned int estimTime;
};

#endif
