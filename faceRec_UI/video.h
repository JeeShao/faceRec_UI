#pragma once
#include <opencv2/opencv.hpp>
#include <QtWidgets/QMainWindow>
using namespace cv;
class Video
{
public:
	Mat frame; //cvÍ¼Ïñ
	Mat currentFrame;
	QImage image; //QtÍ¼Ïñ
	VideoCapture capture;//ÉãÏñÍ·
public:
	Video();
	~Video();
public:
	Mat getCVImage();
	void captureNextFrame();
	QImage getQImageFrame();
	Mat getGrayCVImage();
	QImage Mat2QImage(cv::Mat cvImg);

private:
	Mat read();
};

