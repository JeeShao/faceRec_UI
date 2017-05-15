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
QImage Video::Mat2QImage(Mat cvImg)
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

QImage Video::mat2QImage(Mat mat)
{
	// 8-bits unsigned, NO. OF CHANNELS=1
	if (mat.type() == CV_8UC1)
	{
		// Set the color table (used to translate colour indexes to qRgb values)
		QVector<QRgb> colorTable;
		for (int i = 0; i<256; i++)
			colorTable.push_back(qRgb(i, i, i));
		// Copy input Mat
		const uchar *qImageBuffer = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
		img.setColorTable(colorTable);
		return img;
	}
	// 8-bits unsigned, NO. OF CHANNELS=3
	if (mat.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar *qImageBuffer = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return img.rgbSwapped();
	}
	else
	{
		return QImage();
	}
}

QImage Video::getQImageFrame() {
	captureNextFrame();
	return Mat2QImage(currentFrame);
}