#pragma once
#include <iostream>  
#include <string>  
#include <direct.h>
#include <io.h>

using namespace std;

/*
遍历文件夹
*/
bool traverseFiles(string path)
{
	_finddata_t file_info;
	string current_path = path + "/*.*"; 
	int handle = _findfirst(current_path.c_str(), &file_info);
	//返回值为-1则查找失败  
	if (-1 == handle)
		return false;
	do
	{
		//判断是否子目录  
		string attribute;
		if (file_info.attrib == _A_SUBDIR) //是目录  
		{

		}
		else //文件
		{

		}
		//cout << file_info.name << ' ' << file_info.time_write << ' ' << file_info.size << ' ' << attribute << endl; //获得的最后修改时间是time_t格式的长整型，需要用其他方法转成正常时间显示  
	} while (!_findnext(handle, &file_info));  //返回0则遍历完  
 //关闭文件句柄  
	_findclose(handle);
	return true;
}

/*
判断文件是否存在 不存在则创建 存在则覆盖
*/
bool judgefile(string dirname_str) {
	const char* dirname = dirname_str.c_str(); //字符串常量
	//char c;
	if (_access(dirname, 0)!=0 && _mkdir(dirname)==-1) {  //目录不存在则创建（0:存在 -1:不存在）
		//if (_mkdir(dirname) == -1) { //创建失败则退出 （0:成功 -1:失败）
		//	cout << "文件目录创建失败！" << endl;
		//	exit(1);
		//}
		//else {
		//	cout << dirname << "目录创建成功!" << endl;
		//}
		return false;
	}
	return true;
	//else
	//{
	//	cout << "\n" << dirname << "目录已覆盖!" << endl;
	//}
}
