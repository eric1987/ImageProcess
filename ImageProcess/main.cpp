#include "MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	//QApplication::addLibraryPath("./imageformats");
    MainWindow w;
    w.show();
    return a.exec();
}
