#pragma once
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <io.h>
#include <stdlib.h>
#include <direct.h>
#include <ctime>
#include "config.h"

using namespace cv;
using namespace std;

void judgefile(string dirname_str) {
	const char* dirname = dirname_str.c_str(); //字符串常量
	char c;
	if (_access(dirname, 0) != 0) {  //目录不存在则创建（0:存在 -1:不存在）
			if (_mkdir(dirname) == -1) { //创建失败则退出 （0:成功 -1:失败）
				cout << "文件目录创建失败！" << endl;
				exit(1);
			}
			else {
				cout << dirname << "目录创建成功!" << endl;
			}
	}
	else
	{
		cout <<"\n"<<dirname<< "目录已覆盖!"<<endl;
	}
}


//获取人脸文件路径(遍历pic下面的文件)
vector<string> dir(string path)
{
	vector<string> files;//存放文件名
	long hFile = 0;
	struct _finddata_t fileInfo;
	string pathName, subDir, filePath;
	if ((hFile = _findfirst(pathName.assign(path).append("\\*").c_str(), &fileInfo)) == -1) {
		cout << "木有文件" << endl;
	}
	do
	{
		if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0) {
			subDir = pathName.substr(0, pathName.length() - 1).append(fileInfo.name);
			//dir(subDir);
			filePath = pathName.substr(0, pathName.length() - 1);
			files.push_back(filePath.append(fileInfo.name));
			filePath.clear();
		}
		/*else
		{
		filePath = pathName.substr(0, pathName.length() - 1);
		files.push_back(filePath.append(fileInfo.name));
		cout << "文件：" <<files.capacity() << endl;
		filePath.clear();
		}*/
	} while (_findnext(hFile, &fileInfo) == 0);
	_findclose(hFile);
	/*for (int i = 0; i < files.size(); i++) {
		cout << files[i] << endl;  
	}*/
	return files;
}

//写入csv
bool write_csv(const vector<string> images, const vector<int> labels, const char *addr, char separator = ';')
{
	ofstream fout(addr);
	if (!fout.is_open())
	{
		cout << addr << " 无法打开 " << endl;
		return false;
	}
	for (int i = 0; i<(int)images.size(); i++)
	{
		fout << images[i];  //把人脸文件路径写入csv
		fout << separator;  //分隔符;
		fout << labels[i] << endl; //把人脸编号写入csv
	}
	fout.close();
	return true;
}



//使用CSV文件去读图像和标签，主要使用stringstream和getline方法  
static void read_csv(const string& csv_addr, vector<Mat>& images, vector<int>& labels, char separator = ';') {
	string str;
	std::ifstream file(csv_addr.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, label;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, label);
		if (!path.empty() && !label.empty()) {
			//try {
			images.push_back(imread(path, 0));
			labels.push_back(atoi(label.c_str()));
			//}
			//catch (cv::Exception& e)
			//{
			//	cerr << "文件不存在:"<< path <<"\n"<< e.msg << endl;
			//	// 文件有问题，我们啥也做不了了，退出了  
			//	exit(1);
			//}
		}
		
	}
}



//获取人脸图片，生成csv文件
int trainFile() {
	string path = FACES_PIC_DIR;
	string csv_addr = CSV_FILE;
	vector<string> cates = dir(path); //人脸文件目录路径容器./pic/1; ./pic/2……
	vector<string> images;
	//图片ID  
	vector<int> labels;
	for (int i = 0; i<(int)cates.size(); i++)
	{
		//获取类别绝对地址  
		string cate_dir = cates[i];
		//获取图片  
		vector<string> imgs = dir(cate_dir);
		//判断图片是否存在  
		if ((int)imgs.size() == 0)
		{
			cout << cate_dir << "里面木有图片" << endl;
			getchar();
			exit(1);
		}

		//设置绝对地址  
		for (int j = 0; j<(int)imgs.size(); j++)
		{
			string img_dir =  imgs[j];
			images.push_back(img_dir);//把所有人脸图片文件放入images
			labels.push_back(i); //放入ID  注意此处不能用++i或者i++
		}
	}
	time_t start = clock();
	//制作csv文件  
	if (!write_csv(images, labels, csv_addr.c_str())) {
		cout << "csv文件写入失败!";
			getchar();
			exit(1);
	};
	time_t end = clock();
	double time = (double)(end - start) / CLOCKS_PER_SEC; //计算函数使用时间，以秒计  
	cout << "csv文件写入耗时: " << time << " s" << endl;
	return 0;
}