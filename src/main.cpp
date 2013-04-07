#include <QTextCodec>
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    MainWindow w;
    w.show();
    return a.exec();
}
