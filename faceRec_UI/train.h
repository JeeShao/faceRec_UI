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

//CascadeClassifier faceCascade; //����������


//ȥ���ַ�����β�ո����������û����Ǽ�⣩
string trim(string s)
{
	if( !s.empty() )
	{
	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	}
	return s;
}


//��������������ռ�
void detect() {
	CascadeClassifier faceCascade; //����������
	faceCascade.load(CASCADE_FILE);   //���ط������������������������������� 
	int num = 0;
	//faceCascade.load("./haar/haarcascade_frontalface_alt.xml");   //���ط�������������������������������  
	Mat frame;  //��ʼ������ͷԭͼ��
	Mat imgGray;  //�����Ҷ�ͼ��
	Mat Myface; //��������Ե�����
	vector<Rect> faces;  //��������ͼ������
	string name; //��������
	int i = 1;
	//cout << "�������û�����Enter������:";
	//getline(cin, name);
	name = "shaojian666";
	namedWindow("frame"); //��������ͷ����
	VideoCapture cap(0);  //��������ͷ
	if (!cap.isOpened()) {
		cout << "����ͷδ׼������\n";
		exit(1);
	}
	while (trim(name).empty()){
		cin.clear();//���������
		//cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		cout << "\n�û�������Ϊ��,����������!" << endl;
		cout << "�������û�����Enter������:";
		getline(cin, name);
	}
	string dirname_str = FACES_PIC_DIR + format("\\%s", name);
	judgefile(dirname_str); //�ж��ļ��Ƿ���� �������򴴽�
	while (num<=60)  //ѭ��  �Զ�����20��
	{
		cap >> frame;  //��ȡ����ͷһ֡ͼ��
		flip(frame, frame, 1);//����תͼ��
		if (frame.empty()) {
			cout << "û������ͷͼ�񡭡�\n";
			break;
		}
		if (frame.channels() == 3)  //��ɫͼ����3ͨ����RGB��,�Ҷ�ͼ���ǵ�ͨ��
		{                           //����� 3ͨ���Ĳ�ɫͼ��Ҳ��������ͷ��ȡ��ԭͼ����תΪ�Ҷ�
			cvtColor(frame, imgGray, CV_RGB2GRAY);
		}
		else
		{
			imgGray = frame;
		}
		faceCascade.detectMultiScale(imgGray, faces, 1.2, 6, 0, Size(0, 0));   //������� ������������Լ��ڲ����˽�� 
		string filename = dirname_str + format("/%d.jpg", i); //pic ͼ���ļ������ʽ
		if (faces.size() > 0) //faces.size()��ʾ��⵽��������
		{
			for (int i = 0; i < faces.size(); i++) //����������ж�����������ѭ���þ��α��ÿ���������������ֻ��һ��������������û��Χ����Ļ���
			{
				rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 255, 0), 3, 8);    //�������λ��  
			}
			if (num > 0 && num % 3 == 0) {  //num��Ϊ��=60  �˴���ʾnumΪ3�ı����Ǿ���һ�ţ�����20��
				cout << "���գ�" << i << endl;  //�ն���ʾ������
				i++;
				resize(imgGray(faces[0]), Myface, Size(92, 112)); //ͼ���ʽ��
				//equalizeHist(Myface,Myface);//����ֱ��ͼ
				imwrite(filename, Myface);  //����ͼ��Myyface��ʾ�ĵ��ĸ�ʽ��������
			}
			num++;
		}
		imshow("frame", frame);  //��ʾͼ��
		/*
		��������
		switch (key)
		{
		case'p':            //����p  ������
			if (faces.size() > 0) {
				cout << "���գ�" << i << endl;  //�ն���ʾ������
				i++;
				resize(imgGray(faces[0]), Myface, Size(92, 112)); //ͼ���ʽ��
				//equalizeHist(Myface,Myface);//����ֱ��ͼ
				imwrite(filename, Myface);  //����ͼ��Myyface��ʾ�ĵ��ĸ�ʽ��������
			}
			else {
				cout << "û�м�⵽����" << endl;
			}
			break;
		default:
			break;
		}
		*/
		if(waitKey(50)=='q')
			break;;  //�ȴ�50ms ���������� ѭ����ȡ��Ƶ֡
	}
	destroyAllWindows(); //����p�� �������д���
	cap.release();  //�ͷ�����ͷ
}

//ѵ������ ����ģ��
void trainFace() {
	trainFile();
	//��ȡ���CSV�ļ�·��.  
	string csv_addr = ".\\train\\faces.csv";
	// 2�����������ͼ�����ݺͶ�Ӧ�ı�ǩ  
	vector<Mat> images;
	vector<int> labels;
	// ��ȡ����. ����ļ����Ϸ��ͻ����  
	// ������ļ����Ѿ�����.  
	try
	{
		read_csv(csv_addr, images, labels);
	}
	catch (cv::Exception& e)
	{
		cerr << "Error opening file \"" << csv_addr << "\". Reason: " << e.msg << endl;
		// �ļ������⣬����ɶҲ�������ˣ��˳���  
		exit(1);
	}
	// ���û�ж�ȡ���㹻ͼƬ��Ҳ�˳�.  
	if (images.size() <= 1) {
		string error_message = "ͼƬ̫�٣�����Ӹ���������ѵ��!";
		CV_Error(CV_StsError, error_message);
		exit(1);
	}
	Ptr<face::FaceRecognizer> model = RECOGNIZE_LBPH;  //����ʶ���㷨����config.h�������������㷨�����Ը��������޸ģ�
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN); //�ı��ն�������ɫ
	cout << "ģ��ѵ���С���" << endl;
	model->train(images, labels);  //ѵ������
	model->save(TRAINMODEL_FILE);  //����ģ�ͣ�ʶ��ʱ���ظ�ģ��
	cout << "ģ��ѵ�����" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	Sleep(1000);
}


