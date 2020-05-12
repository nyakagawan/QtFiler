#include "stdafx.h"
#include "Filer.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	Filer w;
	w.show();
	return a.exec();
}
