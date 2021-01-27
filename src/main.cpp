#include "mainwindow.h"
#include <QApplication>
#include <iostream>

#include <cppcsv/include/CsvParser.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
