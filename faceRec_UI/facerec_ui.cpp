#pragma once
#include <sstream>
//#include <time.h>
#include "facerec_ui.h"
#include "common.h"


faceRec_UI* faceRec_UI::facerec_ui; //用于静态函数调用非静态成员

faceRec_UI::faceRec_UI(QWidget *parent)
	: QMainWindow(parent)
{
	//this->setAttribute(Qt::WA_DeleteOnClose, true);
	timer = new QTimer(this);
	facerec_ui = this;
	path = "";
	captureFlag = FACE_NUM;
	setupUi(this);
	_timer = new QTimer(this);
	date_timer = new QTimer(this);
	//QPalette palette;
	//palette.setBrush(QPalette::Background, QBrush(Qt::red));
	//parent->setPalette(palette);
	//parentsetAutoFillbackground(true);
	connect(_timer, SIGNAL(timeout()), this, SLOT(playVideo()));
	connect(date_timer, SIGNAL(timeout()), this, SLOT(updatetime()));
	date_timer->start(1000);
	//_timer->start();
	this->update();

}

void faceRec_UI::setupUi(QMainWindow *MainWindow)
{
	if (MainWindow->objectName().isEmpty())
		MainWindow->setObjectName(QStringLiteral("MainWindow"));
	MainWindow->resize(692, 539);
	centralwidget = new QWidget(MainWindow);
	centralwidget->setObjectName(QStringLiteral("centralwidget"));
	label_title = new QLabel(centralwidget);
	label_title->setObjectName(QStringLiteral("label_title"));
	label_title->setGeometry(QRect(250, 10, 191, 41));
	QFont font;
	font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
	font.setPointSize(22);
	font.setBold(true);
	font.setWeight(75);
	label_title->setFont(font);
	frame = new Mylabel(centralwidget);
	frame->setObjectName(QStringLiteral("Frame"));
	frame->setGeometry(QRect(30, 80, 391, 311));
	frame->setLayoutDirection(Qt::LeftToRight);
	frame->setScaledContents(true); //图片自适应窗口大小
	getHomePic();

	label_author = new QLabel(centralwidget);
	label_author->setObjectName(QStringLiteral("label_author"));
	label_author->setGeometry(QRect(20, 480, 111, 21));

	QFont font1;
	font1.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
	font1.setPointSize(12);
	label_author->setFont(font1);
	label_date = new QLabel(centralwidget);
	label_date->setObjectName(QStringLiteral("label_date"));
	label_date->setGeometry(QRect(310, 480, 111, 21));
	label_date->setFont(font1);

	train_button = new QPushButton(centralwidget);
	train_button->setObjectName(QStringLiteral("train_button"));
	train_button->setGeometry(QRect(470, 120, 171, 51));
	recognize_button = new QPushButton(centralwidget);
	recognize_button->setObjectName(QStringLiteral("recognize_button"));
	recognize_button->setGeometry(QRect(470, 230, 171, 51));

	exit_button = new QPushButton(centralwidget);
	exit_button->setObjectName(QStringLiteral("exit_button"));
	exit_button->setGeometry(QRect(470, 340, 171, 51));
	QFont font2;
	font2.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
	font2.setPointSize(18);
	train_button->setFont(font2);
	recognize_button->setFont(font2);
	exit_button->setFont(font2);

	QFont font3;
	font3.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
	font3.setPointSize(15);
	progressBar = new QProgressBar(centralwidget);
	progressBar->setObjectName(QStringLiteral("progressBar"));
	progressBar->setGeometry(QRect(30, 400, 410, 31));
	progressBar->setValue(0);
	progressBar->setFont(font3);
	progressBar->setRange(0, 30);
	progressBar->setVisible(false);

	labelRes = new QLabel(centralwidget);
	labelRes->setObjectName(QStringLiteral("labelRes"));
	labelRes->setGeometry(QRect(30, 400, 411, 30));
	labelRes->setFont(font3);
	labelRes->setVisible(false);

	//inputDialog = new InputDialog(); //输入用户名窗口
	inputDialog.setWindowModality(Qt::ApplicationModal);//阻塞父窗口
	inputDialog.resize(200, 40);

	MainWindow->setCentralWidget(centralwidget);
	menubar = new QMenuBar(MainWindow);
	menubar->setObjectName(QStringLiteral("menubar"));
	menubar->setGeometry(QRect(0, 0, 692, 23));
	MainWindow->setMenuBar(menubar);

	retranslateUi(MainWindow);
	QObject::connect(train_button, SIGNAL(clicked()), this, SLOT(train()));
	QObject::connect(recognize_button, SIGNAL(clicked()), this, SLOT(recognizer()));
	//QObject::connect(exit_button, SIGNAL(clicked()), MainWindow, SLOT(close()));
	 
	QMetaObject::connectSlotsByName(MainWindow);
}

void faceRec_UI::getHomePic()
{
	QImage qimage;
	srand(time(NULL));
	int i = rand()%8+1;
	Mat image = imread(format(".\\homePic\\%d.jpg",i));
	QImage Qimage = video.mat2QImage(image);
	frame->setPixmap(QPixmap::fromImage(Qimage));
}

void faceRec_UI::retranslateUi(QMainWindow *MainWindow)
{
	MainWindow->setWindowTitle(QApplication::translate("BiuBiuBiu", "BiuBiuBiu", Q_NULLPTR));
	label_title->setText(QApplication::translate("MainWindow", "\344\272\272\350\204\270\350\257\206\345\210\253\347\263\273\347\273\237", Q_NULLPTR));
	label_author->setText(QStringLiteral("作者: 黄渝杰"));
	//label_date->setText(QApplication::translate("MainWindow", "2017-05-10", Q_NULLPTR));
	train_button->setText(QApplication::translate("MainWindow", "\344\272\272\350\204\270\345\275\225\345\205\245", Q_NULLPTR));
	recognize_button->setText(QApplication::translate("MainWindow", "\344\272\272\350\204\270\350\257\206\345\210\253", Q_NULLPTR));
	exit_button->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272\347\263\273\347\273\237", Q_NULLPTR));
}

void faceRec_UI::playVideo()
{
	QImage Qimage;
	Mat cvImage;
	int x = 0, y = 0, w = 0, h = 0;
	if (facedetece->face.area() != 0 && captureFlag>0)
	{
		//640x480像素转换到Qt中frame399x311
		frame->setRect(facedetece->face.x*0.6, facedetece->face.y*0.6,
			facedetece->face.width*0.6, facedetece->face.height*0.68);
	}
	else
	{
		frame->setRect(0, 0, 0, 0);
	}
	frame->setPixmap(QPixmap::fromImage(video.getQImageFrame()));

}

void faceRec_UI::updatetime()
{
	time_t timer;
	time(&timer);
	tm* t_tm = localtime(&timer);
	label_date->setText(QString::number(long(t_tm->tm_year + 1900),10) + "-"
		+ QString::number((t_tm->tm_mon + 1),10)+"-"+ QString::number(t_tm->tm_mday,10));
}

/*******************************************
————————人脸录入模块————————
********************************************/
void faceRec_UI::train()
{
	inputDialog.setInfo(QStringLiteral("请输入用户名"));
	inputDialog.clear();
	inputDialog.show();
}

void faceRec_UI::reciveRes()
{
	Mat cvImage;
	Mat facepic;
	bool write;
	facepic = facedetece->getFace();
	if (!facepic.empty()) {
		string filename = path + format("/%d.jpg", captureFlag);
		write = imwrite(filename, facepic);
		--captureFlag;
		progressBar->setValue(FACE_NUM - captureFlag);
	}
	cvImage = video.getGrayCVImage();
	facedetece->setImage(cvImage);
	if (captureFlag > 0) {  
		facedetece->start();
	}
	else     //人脸采集结束
	{
		facedetece->terminate();
		facedetece->wait();
		trainfaces();
	}
}

void faceRec_UI::trainfaces()
{
	trainPro = new Train();
	_timer->stop();//关闭视频流
	video.capture.release();
	frame->setRect(0, 0, 0, 0);//清空矩形
	frame->clear();//清空图像
	QFont font;
	font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
	font.setPointSize(20);
	font.setBold(true);
	//font.setWeight(75);
	frame->setFont(font);
	frame->setText(QStringLiteral("<font color=green>正在录入人脸…………莫作急！</font>"));
	trainPro->start();//训练
	connect(trainPro, SIGNAL(finished()), this, SLOT(trainOver()));
}

void faceRec_UI::trainOver()
{
	trainPro->terminate();
	trainPro->wait();
	frame->clear();
	getHomePic(); //123
	//QMessageBox::about(NULL, "About", "About this <font color='red'>application</font>");
	QMessageBox msgBox;
	QFont font;
	font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
	font.setPointSize(15);
	//font.setBold(true);
	//font.setWeight(75);
	msgBox.setFont(font);
	msgBox.setWindowTitle("About");
	msgBox.setText(QStringLiteral("人脸录入成功!"));
	//msgBox.about(NULL, "About", QStringLiteral("人脸录入成功!"));
	bool r = msgBox.exec();
	progressBar->setValue(0);
	progressBar->setVisible(false);
	train_button->setEnabled(true);
	recognize_button->setEnabled(true);
	//this->setupUi;
}

void  faceRec_UI::reciveUserName(QString name)
{
	string userName;
	name = name.trimmed();
	if (name.isEmpty())
	{
		facerec_ui->inputDialog.seterrMsg(QStringLiteral("<font color=red>用户名不能为空!</font>"));
	}
	else
	{
		facerec_ui->train_button->setEnabled(false);
		facerec_ui->recognize_button->setEnabled(false);
		userName = name.toStdString();
		facerec_ui->path = FACES_PIC_DIR + string("\\") + userName;
		judgefile(facerec_ui->path);
		//QString path = QDir::currentPath();
		facerec_ui->inputDialog.close();
		facerec_ui->progressBar->setVisible(true);
		facerec_ui->facedetece = new faceDetect();
		facerec_ui->startCollect();
	}
}

void faceRec_UI::startCollect()
{
	Mat cvImage;
	frame->clear();//123
	captureFlag = FACE_NUM;
	_timer->start();
	connect(facedetece, SIGNAL(finished()), this, SLOT(reciveRes()));
	cvImage = video.getGrayCVImage();
	facedetece->setImage(cvImage);
	facedetece->start();
}


/*******************************************
————————人脸识别模块————————
********************************************/

void faceRec_UI::recognizer()
{
	recTimes = 0;
	captureFlag = FACE_NUM;
	Mat cvImage;
	facedetece = new faceDetect();
	_timer->start();
	labelRes->setVisible(true);
	train_button->setEnabled(false);
	recognize_button->setEnabled(false);

	connect(facedetece, SIGNAL(finished()), this, SLOT(reciveRecongnizeRes()));
	cvImage = video.getGrayCVImage();
	facedetece->setImage(cvImage);
	recognize = new Recognize();
	recognize->getNames();//获取csv中用户名信息
	recognize->names;
	frame->clear();//123
	facedetece->start();
}

void faceRec_UI::reciveRecongnizeRes()
{
	//Mat cvImage;
	Mat cvImage;
	Mat facepic;
	if (recognize->times >= 20 || recTimes >= 50) {
		QMessageBox msgBox;
		QFont font;
		labelRes->setVisible(false);
		font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
		font.setPointSize(15);
		msgBox.setFont(font);
		if (recognize->times >= 20)
		{
			int resLabel = getResLabel(recognize->recRse);
			string resUser = recognize->names[resLabel];
			msgBox.setWindowTitle("success");
			msgBox.setText(QStringLiteral("识别结果：") + QString::fromStdString(resUser));
		}
		else
		{
			msgBox.setWindowTitle("failed");
			msgBox.setText(QStringLiteral("无法识别当前人脸！"));
		}
		//msgBox.about(NULL, "About", QStringLiteral("人脸录入成功!"));
		if (msgBox.exec()) {
			train_button->setEnabled(true);
			recognize_button->setEnabled(true);
			frame->setRect(0, 0, 0, 0);
			frame->clear();
			getHomePic();
			_timer->stop();
			video.capture.release();
		}
	}
	else
	{
		facepic = facedetece->getFace();
		if (!facepic.empty()) {
			recognize->startRec(facepic);
			recTimes++;
			ostringstream os;
			os << recognize->confidence;
			string info = "<font color=blue>name: </font>" + recognize->names[recognize->predictedLabel]
				+"<font color=blue>   confidence: </font>" +format("%4s",os.str());
			labelRes->setText(QString::fromStdString(info));
		}
		cvImage = video.getGrayCVImage();
		facedetece->setImage(cvImage);
		facedetece->start();
	}
}



/*******************************************
———————用户名输入框模块———————
********************************************/
InputDialog::InputDialog(/*const char* callbackName,*/QWidget *parent)
	: QWidget(parent)
{
	//callBack = callbackName;
	setupUi(this);
}

void InputDialog::setupUi(QWidget *Qwidget)
{
	font = new QFont();
	font->setPointSize(11);

	labelInfo = new QLabel(Qwidget);
	labelInfo->setFont(*font);

	font1 = new QFont();
	font1->setPointSize(10);
	msgInfo = new QLabel(Qwidget);
	msgInfo->setFont(*font1);
	msgInfo->clear();

	editUserName = new QLineEdit(Qwidget);
	editUserName->setFont(*font);

	pushButton_accept = new QPushButton(Qwidget);
	pushButton_accept->setText(QStringLiteral("确认"));
	pushButton_accept->setFont(*font);
	QObject::connect(pushButton_accept, SIGNAL(clicked()), Qwidget, SLOT(reciveUserName()));

	gl = new QVBoxLayout;
	gl->addWidget(labelInfo);
	gl->addWidget(editUserName);
	gl->addWidget(pushButton_accept);
	gl->addWidget(msgInfo);
	//QWidget *window = new QWidget;
	this->setLayout(gl);
	//window->show();
}

void InputDialog::setInfo(QString info)
{
	labelInfo->setText(info);
}

void InputDialog::seterrMsg(QString info)
{
	msgInfo->setText(info);
}

void InputDialog::clear()
{
	editUserName->clear();
	msgInfo->clear();
}

void InputDialog::reciveUserName()
{
	QString userName;
	userName = editUserName->text();
	faceRec_UI::reciveUserName(userName);

	/*callback*/
}


/*******************************************
——————摄像头框绘制矩形模块——————
********************************************/
Mylabel::Mylabel(QWidget *parent) :
	QLabel(parent)
{
	this->x1 = 0;
	this->y1 = 0;
	this->x2 = 0;
	this->y2 = 0;
}

void Mylabel::setRect(int x1, int y1, int x2, int y2)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
}

void Mylabel::paintEvent(QPaintEvent *event)
{
	QLabel::paintEvent(event);
	QPainter painter(this);
	QPen pen;       //设置画笔，颜色、宽度
	pen.setColor(Qt::blue);
	pen.setWidth(3);
	painter.setPen(pen);
	painter.drawRect(x1, y1, x2, y2);
}

