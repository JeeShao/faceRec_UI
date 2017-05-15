#pragma once
#include <QtCore/QObject>
#include "facerec_ui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	faceRec_UI w;
	w.setWindowIcon(QIcon(".//homePic//love_icon.png"));

	QPalette palette;
	QPixmap pixmap(".//homePic/10.jpg");
	palette.setBrush(QPalette::Background, QBrush(pixmap));
	w.setPalette(palette);
	w.setAutoFillBackground(true);
	w.show();

	a.connect(w.exit_button, SIGNAL(clicked()), &a, SLOT(quit()));
	//QObject::connect(w.exit_button, SIGNAL(clicked()), a, SLOT(quit()));
	return a.exec();
}
