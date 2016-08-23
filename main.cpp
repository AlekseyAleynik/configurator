#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    if(w.qEx){
        w.show();
        return a.exec();
    }
    else
        return 0;
}
