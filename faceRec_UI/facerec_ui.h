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

class faceRec_UI : public QMainWindow
{
	Q_OBJECT
public:
	QWidget *centralwidget;
	QLabel *label_title;
	QFrame *Frame;
	QLabel *label_author;
	QLabel *label_date;
	QPushButton *train_button;
	QPushButton *recognize_button;
	QPushButton *exit_button;
	QProgressBar *progressBar;
	QMenuBar *menubar;

public:
	faceRec_UI(QWidget *parent = Q_NULLPTR);
	void setupUi(QMainWindow *MainWindow);
	void retranslateUi(QMainWindow *MainWindow);

public slots:
	void train();
};
