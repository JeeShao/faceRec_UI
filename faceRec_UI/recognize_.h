#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/face/facerec.hpp>
#include <stdio.h>
#include "config.h"

using namespace cv;
using namespace std;

//CascadeClassifier faceCascade; //声明分类器

Mat toGrayImg(Mat img) {
	Mat imgGray;//灰度图形
	if (img.channels() == 3){   //彩色图像是3通道（RGB）,灰度图像是单通道
		cvtColor(img, imgGray, CV_RGB2GRAY);
		return imgGray;
	}else {
		return img;
	}
}

//标记人脸
vector<Rect> markFace(Mat img, CascadeClassifier faceCascade) {
	Mat imgGray;
	vector<Rect> faces;  //声明人脸图像容器
	imgGray = toGrayImg(img);
	faceCascade.detectMultiScale(imgGray, faces, 1.2, 6, 0, Size(0, 0));   //检测人脸 ，这个函数你自己在博客了解过 
	if (faces.size()>0) {
		for (int i = 0; i < faces.size(); i++) //这里是如果有多张人脸，则循环用矩形标出每个人脸，正常情况只有一个人脸（如果你姐没有围观你的话）
		{
			rectangle(img, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 255, 0), 3, 8);    //框出人脸位置  
		}
	}
	return faces;
}

//根据人脸图片文件路径获取用户名
string getNamefromPath(string path) {
	string str;
	//char *p;
	int left, right, len;
	left = path.find("\\", 2) + 1;
	right = path.find_last_of('\\');
	len = right - left;
	str = path.substr(left, len);
	return str;
}

//读取csv文件 获取用户名数组  用于识别时显示用户名
static void getNames(const string& csv_addr, string(&names)[100], char separator = ';') {
	string str;
	std::ifstream file(csv_addr.c_str(), ifstream::in);
	if (!file) {
		string error_message = "CSV文件不存在!";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, label, name;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, label);
		if (!path.empty() && !label.empty()) {
			//try {
			name = getNamefromPath(path);
			names[atoi(label.c_str())] = name;
		}
	}
	file.close();
}


//识别
void faceRec(CascadeClassifier faceCascade) {
	char key = 0;
	string names[100];
	Point text_xy;
	Mat imgGray,frame,myFace;
	vector<Rect> faces;
	string csv_addr = CSV_FILE;
	Ptr<face::FaceRecognizer> model = RECOGNIZE_LBPH;
	model->load(TRAINMODEL_FILE);//加载训练好的模型
	int predictedLabel = -1;  //预测label
	double confidence = 0.0;  //置信值
	namedWindow("frame"); //声明摄像头窗口
	VideoCapture cap(0);  //调用摄像头
	if (!cap.isOpened()) {
		cout << "摄像头未准备……\n";
		exit(1);
	}
	getNames(csv_addr, names);
	while (key != 'q')  //循环  如果未按下q键 一直循环 ；按下q键则退出程序
	{
		cap >> frame;  //获取摄像头一帧图像
		flip(frame, frame, 1);//镜像翻转图像
		if (frame.empty()) {
			cout << "没有摄像头图像……\n";
			break;
		}
		faces = markFace(frame,faceCascade);  //标记人脸
		if (faces.size() > 0) {
			text_xy = Point(faces[0].x, faces[0].y-8);  //屏幕显示用户名的坐标
			imgGray = toGrayImg(frame);
			resize(imgGray(faces[0]), myFace, Size(92, 112)); //图像格式化
			//equalizeHist(Myface,Myface);//均衡直方图
			model->predict(myFace, predictedLabel, confidence);
			cout << "预测label:" << predictedLabel << "  " << "置信值:" << confidence << endl;
			putText(frame, names[predictedLabel], text_xy, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255),2);
		}
		imshow("frame", frame);  //显示图像
		key = waitKey(20);  //等待20ms 检测键盘输入 循环获取视频帧
	}
	destroyAllWindows(); //按下p后 销毁所有窗口
	cap.release();  //释放摄像头
}
