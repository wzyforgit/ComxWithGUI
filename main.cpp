#include <QApplication>
#include <QTextCodec>
#include "mainWindow.h"

int main(int argc,char* argv[])
{
    QApplication a(argc,argv);

    auto window=new MainWindow;

    window->show();

    return a.exec();
}
