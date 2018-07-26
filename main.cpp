#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(Qt::FramelessWindowHint);//去掉窗口标题栏
    w.move(((QApplication::desktop()->width()-w.width())*0.5),(QApplication::desktop()->height()-w.height())*0.5);
    w.show();

    return a.exec();
}
