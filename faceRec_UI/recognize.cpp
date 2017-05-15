#include "recognize.h"

Recognize::Recognize()
{
	confidence = 0.0;
	predictedLabel = -1;
	times = 0;
	//names[200] = {};
	/*recRse[200] = {};*/
	csv_addr = CSV_FILE;
	model = RECOGNIZE;
	model->load(TRAINMODEL_FILE);//加载训练好的模型
}

string Recognize::getNamefromPath(string path) {
	string str;
	int left, right, len;
	left = path.find("\\", 2) + 1;
	right = path.find_last_of('\\');
	len = right - left;
	str = path.substr(left, len);
	return str;
}

void Recognize::getNames()
{
	string str;
	std::ifstream file(csv_addr.c_str(), ifstream::in);
	if (!file) {
		string error_message = "CSV文件不存在!";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, label, name;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, ';');
		getline(liness, label);
		if (!path.empty() && !label.empty()) {
			name = getNamefromPath(path);
			names[atoi(label.c_str())] = name;
		}
	}
	file.close();
}

void Recognize::startRec(Mat face)
{
	model->predict(face, predictedLabel, confidence);
	if (confidence <= THRESHOLD)
	{
		++recRse[predictedLabel];
		++times;
	}
}

