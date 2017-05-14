#include "faceDetect.h"

faceDetect::faceDetect()
{
	Rect face(0, 0, 0, 0);
	//markface=(0, 0, CV_64F);
	haar_faces.load(CASCADE_FILE);
}

void faceDetect::setImage(Mat image)
{
	this->image = image;
}

Mat faceDetect::getImage()
{
	return this->image;
}

void faceDetect::detectSingleFace()
{
	
	vector<Rect> faces(0);
	Rect nullRect(0, 0, 0, 0);
	Mat frame;
	frame = getImage();
	haar_faces.detectMultiScale(frame, faces, 1.2, 6, 0, Size(0, 0));
	if (faces.size()>0)
	{
		face = faces[0];
	}
	else
	{
		face = nullRect;
	}
}

Mat faceDetect::getFace()
{
	//Rect face(0,0,0,0);
	Rect key(0,0,0,0);
	Mat res;
	//detectSingleFace();
	if ((face | key) != key) {
		return resizeFace(image(face));
	}
	return res;
}

void faceDetect::markFace()
{
	//Mat nullMat(0, 0, CV_64F);
	//detectSingleFace();
	if (face.area() != 0) {
		//return nullMat;
		rectangle(image, Point(face.x, face.y),
			Point(face.x + face.width, face.y + face.height), Scalar(0, 255, 0), 3, 8);
	}
	//return image;
}

Mat faceDetect::resizeFace(Mat image) {
	Mat resize_image;
	resize(image, resize_image, Size(FACE_WIDTH, FACE_HEIGHT), (0, 0), (0, 0), cv::INTER_LINEAR);
	return resize_image;
}

void faceDetect::run()
{
	detectSingleFace();
}