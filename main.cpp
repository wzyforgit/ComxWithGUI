#include <QApplication>
#include <QTextCodec>
#include "mainWindow.h"

int main(int argc,char* argv[])
{
    QApplication a(argc,argv);

    auto window=new MainWindow;

    window->show();

    /*foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
          QString s = QObject::tr("Port: ") + info.portName() + "\n"
                    + QObject::tr("Location: ") + info.systemLocation() + "\n"
                    + QObject::tr("Description: ") + info.description() + "\n"
                    + QObject::tr("Manufacturer: ") + info.manufacturer() + "\n"
                    + QObject::tr("Vendor Identifier: ") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Product Identifier: ") + (info.hasProductIdentifier() ?QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";
    }*/

    return a.exec();
}
