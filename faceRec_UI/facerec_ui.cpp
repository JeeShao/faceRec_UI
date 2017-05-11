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
#include "facerec_ui.h"
#include "train.h"

faceRec_UI::faceRec_UI(QWidget *parent)
	: QMainWindow(parent)
{
	/*setMinimumSize(200, 120);
	setMaximumSize(1366, 768);*/
	setupUi(this);
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
	Frame = new QFrame(centralwidget);
	Frame->setObjectName(QStringLiteral("Frame"));
	Frame->setGeometry(QRect(30, 60, 351, 301));
	Frame->setLayoutDirection(Qt::LeftToRight);
	Frame->setFrameShape(QFrame::StyledPanel);
	Frame->setFrameShadow(QFrame::Raised);
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
	progressBar = new QProgressBar(centralwidget);
	progressBar->setObjectName(QStringLiteral("progressBar"));
	progressBar->setGeometry(QRect(30, 370, 351, 31));
	progressBar->setValue(24);
	progressBar->setFont(font2);
	MainWindow->setCentralWidget(centralwidget);
	menubar = new QMenuBar(MainWindow);
	menubar->setObjectName(QStringLiteral("menubar"));
	menubar->setGeometry(QRect(0, 0, 692, 23));
	MainWindow->setMenuBar(menubar);

	retranslateUi(MainWindow);
	QObject::connect(train_button, SIGNAL(clicked()), this, SLOT(train()));
	QObject::connect(recognize_button, SIGNAL(clicked()), progressBar, SLOT(hide()));
	QObject::connect(exit_button, SIGNAL(clicked()), MainWindow, SLOT(close()));

	QMetaObject::connectSlotsByName(MainWindow);
}

void faceRec_UI::retranslateUi(QMainWindow *MainWindow)
{
	MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
	label_title->setText(QApplication::translate("MainWindow", "\344\272\272\350\204\270\350\257\206\345\210\253\347\263\273\347\273\237", Q_NULLPTR));
	label_author->setText(QApplication::translate("MainWindow", "\344\275\234\350\200\205\357\274\232\351\273\204\346\270\235\346\235\260", Q_NULLPTR));
	label_date->setText(QApplication::translate("MainWindow", "2017-05-10", Q_NULLPTR));
	train_button->setText(QApplication::translate("MainWindow", "\344\272\272\350\204\270\345\275\225\345\205\245", Q_NULLPTR));
	recognize_button->setText(QApplication::translate("MainWindow", "\344\272\272\350\204\270\350\257\206\345\210\253", Q_NULLPTR));
	exit_button->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272\347\263\273\347\273\237", Q_NULLPTR));
}

void faceRec_UI::train()
{
	detect();
	trainFace();
}

