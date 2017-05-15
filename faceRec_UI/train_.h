#pragma once
#include <iostream>
//#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/face/facerec.hpp>
#include <io.h>
#include <stdlib.h>
#include <direct.h>
#include "csv.h"
#include "config.h"
#include "video.h"



using namespace cv;
using namespace std;

//CascadeClassifier faceCascade; //声明分类器


//去除字符串首尾空格（用于输入用户名是检测）
string trim(string s)
{
	if( !s.empty() )
	{
	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	}
	return s;
}


//检测人脸并拍照收集
void detect() {
	CascadeClassifier faceCascade; //声明分类器
	faceCascade.load(CASCADE_FILE);   //加载分类器，这个分类器是用来检测人脸的 
	int num = 0;
	//faceCascade.load("./haar/haarcascade_frontalface_alt.xml");   //加载分类器，这个分类器是用来检测人脸的  
	Mat frame;  //初始化摄像头原图像
	Mat imgGray;  //声明灰度图像
	Mat Myface; //声明保存对的人脸
	vector<Rect> faces;  //声明人脸图像容器
	string name; //人脸名字
	int i = 1;
	//cout << "请输入用户名（Enter结束）:";
	//getline(cin, name);
	name = "shaojian666";
	namedWindow("frame"); //声明摄像头窗口
	VideoCapture cap(0);  //调用摄像头
	if (!cap.isOpened()) {
		cout << "摄像头未准备……\n";
		exit(1);
	}
	while (trim(name).empty()){
		cin.clear();//清空输入流
		//cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		cout << "\n用户名不能为空,请重新输入!" << endl;
		cout << "请输入用户名（Enter结束）:";
		getline(cin, name);
	}
	string dirname_str = FACES_PIC_DIR + format("\\%s", name);
	judgefile(dirname_str); //判断文件是否存在 不存在则创建
	while (num<=60)  //循环  自动拍照20张
	{
		cap >> frame;  //获取摄像头一帧图像
		flip(frame, frame, 1);//镜像翻转图像
		if (frame.empty()) {
			cout << "没有摄像头图像……\n";
			break;
		}
		if (frame.channels() == 3)  //彩色图像是3通道（RGB）,灰度图像是单通道
		{                           //如果是 3通道的彩色图像（也就是摄像头获取的原图像）则转为灰度
			cvtColor(frame, imgGray, CV_RGB2GRAY);
		}
		else
		{
			imgGray = frame;
		}
		faceCascade.detectMultiScale(imgGray, faces, 1.2, 6, 0, Size(0, 0));   //检测人脸 ，这个函数你自己在博客了解过 
		string filename = dirname_str + format("/%d.jpg", i); //pic 图像文件处理格式
		if (faces.size() > 0) //faces.size()表示检测到的人脸数
		{
			for (int i = 0; i < faces.size(); i++) //这里是如果有多张人脸，则循环用矩形标出每个人脸，正常情况只有一个人脸（如果你姐没有围观你的话）
			{
				rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 255, 0), 3, 8);    //框出人脸位置  
			}
			if (num > 0 && num % 3 == 0) {  //num设为《=60  此处表示num为3的倍数是就拍一张，共拍20张
				cout << "拍照：" << i << endl;  //终端显示拍照数
				i++;
				resize(imgGray(faces[0]), Myface, Size(92, 112)); //图像格式化
				//equalizeHist(Myface,Myface);//均衡直方图
				imwrite(filename, Myface);  //保存图像，Myyface表示拍到的格式化的人脸
			}
			num++;
		}
		imshow("frame", frame);  //显示图像
		/*
		按键拍照
		switch (key)
		{
		case'p':            //按下p  则拍照
			if (faces.size() > 0) {
				cout << "拍照：" << i << endl;  //终端显示拍照数
				i++;
				resize(imgGray(faces[0]), Myface, Size(92, 112)); //图像格式化
				//equalizeHist(Myface,Myface);//均衡直方图
				imwrite(filename, Myface);  //保存图像，Myyface表示拍到的格式化的人脸
			}
			else {
				cout << "没有检测到人脸" << endl;
			}
			break;
		default:
			break;
		}
		*/
		if(waitKey(50)=='q')
			break;;  //等待50ms 检测键盘输入 循环获取视频帧
	}
	destroyAllWindows(); //按下p后 销毁所有窗口
	cap.release();  //释放摄像头
}

//训练人脸 生成模型
void trainFace() {
	trainFile();
	//读取你的CSV文件路径.  
	string csv_addr = ".\\train\\faces.csv";
	// 2个容器来存放图像数据和对应的标签  
	vector<Mat> images;
	vector<int> labels;
	// 读取数据. 如果文件不合法就会出错  
	// 输入的文件名已经有了.  
	try
	{
		read_csv(csv_addr, images, labels);
	}
	catch (cv::Exception& e)
	{
		cerr << "Error opening file \"" << csv_addr << "\". Reason: " << e.msg << endl;
		// 文件有问题，我们啥也做不了了，退出了  
		exit(1);
	}
	// 如果没有读取到足够图片，也退出.  
	if (images.size() <= 1) {
		string error_message = "图片太少，请添加更多人脸以训练!";
		CV_Error(CV_StsError, error_message);
		exit(1);
	}
	Ptr<face::FaceRecognizer> model = RECOGNIZE_LBPH;  //人脸识别算法（在config.h中配置有三种算法，可以根据配置修改）
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN); //改变终端字体颜色
	cout << "模型训练中……" << endl;
	model->train(images, labels);  //训练人脸
	model->save(TRAINMODEL_FILE);  //保存模型，识别时加载该模型
	cout << "模型训练完毕" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	Sleep(1000);
}


