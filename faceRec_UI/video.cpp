#include "video.h"

Video::Video()
{
	frame = NULL;
	currentFrame = NULL;
}

Video::~Video()
{
}

void Video::captureNextFrame()
{
	read();
	if (!frame.empty()) {
		cvtColor(frame, currentFrame, CV_BGR2RGB);
		//flip(currentFrame, currentFrame, 1);
	}
}

Mat Video::getCVImage()
{
	Mat cvImage;
	read();
	cvtColor(frame, cvImage, CV_BGR2RGB);
	//flip(cvImage, cvImage, 1);
	return cvImage;
}

Mat Video::read()
{
	if (!capture.isOpened())
		capture.open(0);
	bool ret;
	ret = capture.read(frame);
	if(ret)
		flip(frame, frame, 1);
	return frame;
}

Mat Video::getGrayCVImage() {
	Mat grayFrame;
	read();
	if (!frame.empty())
		cvtColor(frame, grayFrame, CV_BGR2GRAY);
	return grayFrame;
}

//cvÍ¼Ïñ×ªQtÍ¼Ïñ
QImage Video::Mat2QImage(cv::Mat cvImg)
{
	QImage qImg;
	if (cvImg.channels() == 3)  //3 channels color image
	{
		//cv::cvtColor(cvImg, cvImg, CV_BGR2RGB);
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_RGB888);
	}
	else if (cvImg.channels() == 1) //grayscale image
	{
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_Indexed8);
	}
	else
	{
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_RGB888);
	}
	return qImg;
}

QImage Video::getQImageFrame() {
	captureNextFrame();
	return Mat2QImage(currentFrame);
}