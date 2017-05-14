#pragma once
#include <opencv2/opencv.hpp>
#include <QThread>
#include <stdlib.h>
#include<algorithm> 
#include "config.h"

using namespace std;
using namespace cv;

class faceDetect :public QThread
{
public:
	faceDetect();
public:
	Rect face;
	
public:
	void detectSingleFace();
	Mat getFace();
	void markFace();
	void setImage(Mat image);
	Mat getImage();
	void run();
private:
	Mat image;
	//Mat markface;
	CascadeClassifier haar_faces;
	Mat resizeFace(Mat image);
};

