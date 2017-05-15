#include "train.h"
#include "csv.h"

Train::Train()
{
	path = FACES_PIC_DIR;
	csv_addr = CSV_FILE;
}


int Train::facetoCsv() {
	vector<string> cates = dir(path); //人脸文件目录路径容器./pic/1; ./pic/2……
	vector<string> images;
	//图片ID  
	vector<int> labels;
	if ((int)cates.size() > 0) {
		for (int i = 0; i<(int)cates.size(); i++)
		{
			//获取类别绝对地址  
			string cate_dir = cates[i];
			//获取图片  
			vector<string> imgs = dir(cate_dir); 
			for (int j = 0; j<(int)imgs.size(); j++)
			{
				string img_dir = imgs[j];
				images.push_back(img_dir);//把所有人脸图片文件放入images
				labels.push_back(i); //放入ID  注意此处不能用++i或者i++
			}
		}
		if (!write_csv(images, labels, csv_addr.c_str())) {
			return 0; //csv写入失败
		};
		return 1;//写入成功
	}
	return -1; //目录下面没有人脸文件
}

void Train::trainFace()
{
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
		//exit(1);
	}
	// 如果没有读取到足够图片，也退出.  
	if (images.size() <= 1) {
		string error_message = "图片太少，请添加更多人脸以训练!";
		CV_Error(CV_StsError, error_message);
		//exit(1);
	}
	model = RECOGNIZE;  //人脸识别算法（在config.h中配置有三种算法，可以根据配置修改）
	model->train(images, labels);  //训练人脸
	model->save(TRAINMODEL_FILE);  //保存模型，识别时加载该模型
}

void Train::run()
{
	facetoCsv();
	trainFace();
}