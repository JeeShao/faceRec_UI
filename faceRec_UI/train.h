#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/face/facerec.hpp>
#include <QThread>
#include "config.h"

using namespace std;
using namespace cv;

class Train :public QThread
{

public:
	Ptr<face::FaceRecognizer> model;
	string csv_addr;
	string path;
public:
	Train();
private:
	int facetoCsv();
public:
	void trainFace();
	void run();
};

