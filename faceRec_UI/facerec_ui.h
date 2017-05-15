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
#include <QtWidgets/QMessageBox>
#include <QtCore/QTimer>
#include <QVBoxLayout>  
#include <QLineEdit> 
#include <QPainter> 
#include <QPaintEvent>
#include "video.h"
#include "faceDetect.h"
#include "train.h"
#include "recognize.h"

/*******************************************
—————摄像头框绘制矩形类————————
********************************************/
class InputDialog :public QWidget
{
	Q_OBJECT

public:
	QFont *font;
	QFont *font1;
	QLabel *labelInfo;
	QLabel *msgInfo;
	QLineEdit *editUserName;
	QPushButton *pushButton_accept;
	QVBoxLayout *gl;
public:
	InputDialog(QWidget *parent = Q_NULLPTR);
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



/*******************************************
—————————主窗体类—————————
********************************************/
class faceRec_UI : public QMainWindow
{
	Q_OBJECT
public:
	QWidget *centralwidget;
	QLabel *label_title;
	Mylabel *frame;
	QLabel *label_author;
	QLabel *label_date;
	QLabel *labelRes;
	QPushButton *train_button;
	QPushButton *recognize_button;
	QPushButton *exit_button;
	QProgressBar *progressBar;
	QMenuBar *menubar;
	QTimer *timer;
	QTimer *_timer;
	string path;
	int recTimes;// 识别检测次数
	int captureFlag;//人脸采集数 控制ProsserBar
	Video video;
	Train *trainPro;
	Recognize *recognize;
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
private:
	void trainfaces();
public slots:
	void train();
	void reciveRes();
	void playVideo();
	void trainOver();
	void recognizer();
	void reciveRecongnizeRes();
};



