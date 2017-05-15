#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/face/facerec.hpp>
#include <stdio.h>
#include "config.h"

using namespace cv;
using namespace std;

//CascadeClassifier faceCascade; //����������

Mat toGrayImg(Mat img) {
	Mat imgGray;//�Ҷ�ͼ��
	if (img.channels() == 3){   //��ɫͼ����3ͨ����RGB��,�Ҷ�ͼ���ǵ�ͨ��
		cvtColor(img, imgGray, CV_RGB2GRAY);
		return imgGray;
	}else {
		return img;
	}
}

//�������
vector<Rect> markFace(Mat img, CascadeClassifier faceCascade) {
	Mat imgGray;
	vector<Rect> faces;  //��������ͼ������
	imgGray = toGrayImg(img);
	faceCascade.detectMultiScale(imgGray, faces, 1.2, 6, 0, Size(0, 0));   //������� ������������Լ��ڲ����˽�� 
	if (faces.size()>0) {
		for (int i = 0; i < faces.size(); i++) //����������ж�����������ѭ���þ��α��ÿ���������������ֻ��һ��������������û��Χ����Ļ���
		{
			rectangle(img, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 255, 0), 3, 8);    //�������λ��  
		}
	}
	return faces;
}

//��������ͼƬ�ļ�·����ȡ�û���
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

//��ȡcsv�ļ� ��ȡ�û�������  ����ʶ��ʱ��ʾ�û���
static void getNames(const string& csv_addr, string(&names)[100], char separator = ';') {
	string str;
	std::ifstream file(csv_addr.c_str(), ifstream::in);
	if (!file) {
		string error_message = "CSV�ļ�������!";
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


//ʶ��
void faceRec(CascadeClassifier faceCascade) {
	char key = 0;
	string names[100];
	Point text_xy;
	Mat imgGray,frame,myFace;
	vector<Rect> faces;
	string csv_addr = CSV_FILE;
	Ptr<face::FaceRecognizer> model = RECOGNIZE_LBPH;
	model->load(TRAINMODEL_FILE);//����ѵ���õ�ģ��
	int predictedLabel = -1;  //Ԥ��label
	double confidence = 0.0;  //����ֵ
	namedWindow("frame"); //��������ͷ����
	VideoCapture cap(0);  //��������ͷ
	if (!cap.isOpened()) {
		cout << "����ͷδ׼������\n";
		exit(1);
	}
	getNames(csv_addr, names);
	while (key != 'q')  //ѭ��  ���δ����q�� һֱѭ�� ������q�����˳�����
	{
		cap >> frame;  //��ȡ����ͷһ֡ͼ��
		flip(frame, frame, 1);//����תͼ��
		if (frame.empty()) {
			cout << "û������ͷͼ�񡭡�\n";
			break;
		}
		faces = markFace(frame,faceCascade);  //�������
		if (faces.size() > 0) {
			text_xy = Point(faces[0].x, faces[0].y-8);  //��Ļ��ʾ�û���������
			imgGray = toGrayImg(frame);
			resize(imgGray(faces[0]), myFace, Size(92, 112)); //ͼ���ʽ��
			//equalizeHist(Myface,Myface);//����ֱ��ͼ
			model->predict(myFace, predictedLabel, confidence);
			cout << "Ԥ��label:" << predictedLabel << "  " << "����ֵ:" << confidence << endl;
			putText(frame, names[predictedLabel], text_xy, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255),2);
		}
		imshow("frame", frame);  //��ʾͼ��
		key = waitKey(20);  //�ȴ�20ms ���������� ѭ����ȡ��Ƶ֡
	}
	destroyAllWindows(); //����p�� �������д���
	cap.release();  //�ͷ�����ͷ
}
