#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/face/facerec.hpp>
#include <iostream>
#include "config.h"
#include "faceDetect.h"

using namespace std;
using namespace cv;

class Recognize
{
public:
	Ptr<face::FaceRecognizer> model;
	int predictedLabel;
	double confidence;
	string csv_addr;
	string names[200];
	int times;
	int recRse[200] = {};//识别结果集
	vector<Rect> faces;
	faceDetect *facedetece;
public:
	Recognize();
public:
	void getNames();
	void startRec(Mat face);
private:
	string getNamefromPath(string path);
};

