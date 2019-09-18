#include "mainwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.setPointsNum(2);
    w.show();

    return a.exec();
}
