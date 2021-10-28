#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <jsoncpp/json/json.h>

int main(int argc, char *argv[])
{
    try{
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }catch(Json::Exception e){
        std::cout << e.what() << std::endl;
    };
}
