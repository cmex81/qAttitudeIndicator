#include <QtGui/QApplication>
#include "qattitudeindicator.h"

//#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qAttitudeIndicator attInd;
//    MainWindow w;
//    w.show();
    attInd.show();
    return a.exec();
}
