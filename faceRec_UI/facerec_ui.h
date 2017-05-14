#pragma once
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QtCore/QTimer>
#include <QVBoxLayout>  
#include <QLineEdit> 
#include <QPainter> 
#include <QPaintEvent>
#include "video.h"
#include "faceDetect.h"

class InputDialog :public QWidget
{
	Q_OBJECT

public:
	QFont *font;
	QLabel *labelInfo;
	QLabel *msgInfo;
	QLineEdit *editUserName;
	QPushButton *pushButton_accept;
	QVBoxLayout *gl;
	//const char* callBack;
public:
	InputDialog(/*const char* callbackName,*/QWidget *parent = Q_NULLPTR);
	void setupUi(QWidget *Qwidget);
	void setInfo(QString info);
	void seterrMsg(QString info);
	void clear();
public slots:
	void reciveUserName();
};

class Mylabel :public QLabel
{
public:
	explicit Mylabel(QWidget *parent = 0);
	void paintEvent(QPaintEvent *event);
	void setRect(int x1, int y1, int x2, int y2);
private:
	int x1, y1, x2, y2;
};

class faceRec_UI : public QMainWindow
{
	Q_OBJECT
public:
	QWidget *centralwidget;
	QLabel *label_title;
	Mylabel *frame;
	QLabel *label_author;
	QLabel *label_date;
	QPushButton *train_button;
	QPushButton *recognize_button;
	QPushButton *exit_button;
	QProgressBar *progressBar;
	QMenuBar *menubar;
	QTimer *timer;
	QTimer *_timer;
	string path;
	//int i;//保存人脸图片文件序号
	int captureFlag;//人脸采集数 控制ProsserBar
	Video video;
	InputDialog inputDialog;
	faceDetect *facedetece;
	static faceRec_UI* facerec_ui;


public:
	faceRec_UI(QWidget *parent = Q_NULLPTR);
	void setupUi(QMainWindow *MainWindow);
	void retranslateUi(QMainWindow *MainWindow);
	void startCollect();
	static void reciveUserName(QString name);
	//void paintEvent(QPaintEvent *);



public slots:
	void train();
	void nextFrame();
	void reciveRes();
	void playVideo();


};



