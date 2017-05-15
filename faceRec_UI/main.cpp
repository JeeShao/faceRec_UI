#pragma once
#include <QtCore/QObject>
#include "facerec_ui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	faceRec_UI w;
	w.show();
	a.connect(w.exit_button, SIGNAL(clicked()), &a, SLOT(quit()));
	//QObject::connect(w.exit_button, SIGNAL(clicked()), a, SLOT(quit()));
	return a.exec();
}
