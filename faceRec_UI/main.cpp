#include "facerec_ui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	faceRec_UI w;
	w.show();
	return a.exec();
}
