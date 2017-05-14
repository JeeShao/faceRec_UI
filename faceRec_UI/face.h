#pragma once
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include<algorithm> 
#include "config.h"

using namespace std;
using namespace cv;


//检测单个人脸
Rect detectSingleFace(Mat image)
{
	CascadeClassifier haar_faces;
	haar_faces.load(CASCADE_FILE);
	vector<Rect> faces(0);
	Rect nullRect(0,0,0,0);
	haar_faces.detectMultiScale(image, faces, 1.2, 6, 0, Size(0, 0));
	if (faces.size()>0)
	{
		return faces[0];
	}
	return nullRect;

}

Mat resize(Mat image) {
	Mat resize_image;
	resize(image, resize_image, Size(FACE_WIDTH, FACE_HEIGHT), (0, 0), (0, 0), cv::INTER_LINEAR);
	return resize_image;
}

Mat getFace(Mat image)
{
	Rect face;
	Rect key;
	Mat res;
	face = detectSingleFace(image);
	if ((face | key) != key) {
		return resize(image(face));
	}
	return res;
}

//Mat crop(Mat image, int x, int y, int w, int h)
//{
//	int crop_height,midy, y1, y2;
//	crop_height = FACE_WIDTH / FACE_HEIGHT * w;
//	midy = y + h / 2;
//	y1 = max(0, midy - crop_height / 2);
//	y2 = min(sizeof(image) / sizeof(image[0]) - 1, midy + crop_height / 2);
//	return image(range(y1, y2), range(x, x + w));
//}
//格式化人脸

//标记人脸
Mat markFace(Mat image)
{
	Mat nullMat(0,0,CV_64F);
	Rect res = detectSingleFace(image);
	if (res.area()==0) {
		return nullMat;
	}
	rectangle(image, Point(res.x, res.y), Point(res.x + res.width, res.y + res.height), Scalar(0, 255, 0), 3, 8);
	return image;
}