#include <QtWidgets>
#include <iostream>
#include "mainWindow.h"
#include "common.h"
#include <QTextCursor>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    createUartLayout();
    auto mainWidget=new QWidget;
    mainWidget->setLayout(mainLayout);
    setFixedSize(550,470);
    setCentralWidget(mainWidget);
    setWindowTitle(tr("串口调试助手V0.2"));
}

void MainWindow::createUartBoxs()
{
    /*串口基本数据设置*/
    ComBox=new QComboBox;

    BaudBox=new QComboBox;
    BaudBox->addItem(tr("1200"));
    BaudBox->addItem(tr("2400"));
    BaudBox->addItem(tr("4800"));
    BaudBox->addItem(tr("9600"));
    BaudBox->addItem(tr("14400"));
    BaudBox->addItem(tr("19200"));
    BaudBox->addItem(tr("38400"));
    BaudBox->addItem(tr("56000"));
    BaudBox->addItem(tr("57600"));
    BaudBox->addItem(tr("115200"));
    BaudBox->addItem(tr("128000"));
    BaudBox->addItem(tr("256000"));

    ParityBox=new QComboBox;
    ParityBox->addItem(tr("N"));
    ParityBox->addItem(tr("O"));
    ParityBox->addItem(tr("E"));

    ByteSizeBox=new QComboBox;
    ByteSizeBox->addItem(tr("5"));
    ByteSizeBox->addItem(tr("6"));
    ByteSizeBox->addItem(tr("7"));
    ByteSizeBox->addItem(tr("8"));

    StopBitBox=new QComboBox;
    StopBitBox->addItem(tr("1"));
    StopBitBox->addItem(tr("1.5"));
    StopBitBox->addItem(tr("2"));

    /*接收框*/
    DisplayByHex=new QCheckBox(tr("十六进制显示"));
    ReceiveBox=new QTextEdit;
    ReceiveBox->setReadOnly(true);

    ReceiveBoxClean=new QPushButton(tr("清空接收区"));
    connect(ReceiveBoxClean,&QPushButton::clicked,[=](){ReceiveBox->clear();});

    /*发送框*/
    SendBox=new QTextEdit;
    SendBox->setText(tr("串口调试助手v0.2(by海魂水晶)"));

    SendInHex=new QCheckBox(tr("十六进制发送"));
    SendInHex->setEnabled(false);

    SendData=new QPushButton(tr("发送数据"));
    SendData->setEnabled(false);
    connect(SendData,&QPushButton::clicked,[=](){this->sendData();});

    SendTimer=new QTimer;
    SendByTime=new QCheckBox(tr("定时发送"));

    SendTimeSet=new QLineEdit;
    QRegExp SendTimeSetRule("^[0-9]*");
    SendTimeSet->setValidator(new QRegExpValidator(SendTimeSetRule));

    SendByTime->setEnabled(false);
    SendTimeSet->setMaximumWidth(50);
    connect(SendByTime,SIGNAL(clicked()),this,SLOT(autoSend()));
    connect(SendTimer,&QTimer::timeout,[=](){this->sendData();});

    /*串口开关*/
    UartSwitch=new QPushButton(tr("打开串口"));
    connect(UartSwitch,SIGNAL(clicked()),this,SLOT(comSwitch()));

    /*错误提示框*/
    ErrorDialog=new QErrorMessage;
    ErrorDialog->setWindowTitle(tr("出错啦！"));
    ErrorDialog->setModal(true);

    /*收数据定时器*/
    ReceiveTimer=new QTimer;
    connect(ReceiveTimer,SIGNAL(timeout()),this,SLOT(ReceiveData()));

    /*编码选择*/
    CodeSet=new QButtonGroup;
    UseGB=new QRadioButton(tr("使用GB18030编码"));
    UseUTF=new QRadioButton(tr("使用UTF-8编码"));
    CodeSet->addButton(UseGB);
    CodeSet->addButton(UseUTF);
    UseUTF->setChecked(true);

    /*自动刷新可用串口信息*/
    updateSerialInfo=new QTimer;
    updateSerialInfo->start(1000);
    connect(updateSerialInfo,&QTimer::timeout,[=](){
        static QList<QSerialPortInfo> proList;
        QList<QSerialPortInfo> nowList;
        for(auto &SerialInfo:QSerialPortInfo::availablePorts())
        {
            nowList+=SerialInfo;
        }
        if(nowList==proList)
        {
            return;
        }
        else
        {
            updateComBox(nowList);
        }
        proList=nowList;
    });
}

void MainWindow::createUartLabels()
{
    ComLabel=new QLabel(tr("串口:"));
    BaudLabel=new QLabel(tr("波特率:"));
    ParityLabel=new QLabel(tr("校验位:"));
    ByteSizeLabel=new QLabel(tr("数据位:"));
    StopBitLabel=new QLabel(tr("停止位:"));
    SendTimeMs=new QLabel(tr("毫秒"));
}

void MainWindow::createUartLayout()
{
    createUartBoxs();
    createUartLabels();

    auto uartSetLayout=new QGridLayout;
    uartSetLayout->addWidget(ComLabel,0,0);
    uartSetLayout->addWidget(ComBox,0,1);
    uartSetLayout->addWidget(BaudLabel,1,0);
    uartSetLayout->addWidget(BaudBox,1,1);
    uartSetLayout->addWidget(ParityLabel,2,0);
    uartSetLayout->addWidget(ParityBox,2,1);
    uartSetLayout->addWidget(ByteSizeLabel,3,0);
    uartSetLayout->addWidget(ByteSizeBox,3,1);
    uartSetLayout->addWidget(StopBitLabel,4,0);
    uartSetLayout->addWidget(StopBitBox,4,1);

    uartSetLayout->addWidget(UartSwitch,5,0,1,2);
    uartSetLayout->addWidget(ReceiveBoxClean,6,0,1,2);
    uartSetLayout->addWidget(DisplayByHex,7,0,1,2);
    uartSetLayout->addWidget(UseUTF,8,0,1,2);
    uartSetLayout->addWidget(UseGB,9,0,1,2);

    auto uartSendLayout=new QGridLayout;
    uartSendLayout->addWidget(SendTimeSet,0,0);
    uartSendLayout->addWidget(SendTimeMs,0,1);
    uartSendLayout->addWidget(SendByTime,1,0,1,2);
    uartSendLayout->addWidget(SendInHex,2,0,1,2);
    uartSendLayout->addWidget(SendData,3,0,1,2);

    auto leftLayout=new QVBoxLayout;

    leftLayout->addLayout(uartSetLayout);
    leftLayout->addStretch();
    leftLayout->addLayout(uartSendLayout);

    ReceiveBox->setMinimumWidth(400);
    auto rightLayout=new QVBoxLayout;
    rightLayout->addWidget(ReceiveBox);
    rightLayout->addWidget(SendBox);

    mainLayout=new QGridLayout;
    mainLayout->addLayout(leftLayout,0,0);
    mainLayout->addLayout(rightLayout,0,1,1,2);
}

void MainWindow::comSwitch()
{
    if(fComOpen==false)
    {
        /*打开串口*/
        ser.setPortName(QString("\\\\.\\")+getComName(ComBox->currentText()));
        ser.setBaudRate(BaudBox->currentText().toInt());
        switch(ByteSizeBox->currentText().toInt())
        {
            case 5:ser.setDataBits(QSerialPort::Data5); break;
            case 6:ser.setDataBits(QSerialPort::Data6); break;
            case 7:ser.setDataBits(QSerialPort::Data7); break;
            case 8:ser.setDataBits(QSerialPort::Data8); break;
        }
        switch(ParityBox->currentText().toStdString()[0])
        {
            case 'N':ser.setParity(QSerialPort::NoParity);   break;
            case 'O':ser.setParity(QSerialPort::OddParity);  break;
            case 'E':ser.setParity(QSerialPort::EvenParity); break;
        }
        switch(static_cast<int>(StopBitBox->currentText().toFloat()*10))
        {
            case 10:ser.setStopBits(QSerialPort::OneStop);        break;
            case 15:ser.setStopBits(QSerialPort::OneAndHalfStop); break;
            case 20:ser.setStopBits(QSerialPort::TwoStop);        break;
        }

        if(ser.open(QIODevice::ReadWrite)==false)
        {
            ErrorDialog->showMessage(tr("串口正在被占用"));
            return;
        }
        fComOpen=true;
        ReceiveTimer->start(ReceiveCycle);
        UartSwitch->setText(tr("关闭串口"));

        ComBox->setEnabled(false);
        BaudBox->setEnabled(false);
        ByteSizeBox->setEnabled(false);
        ParityBox->setEnabled(false);
        StopBitBox->setEnabled(false);

        SendInHex->setEnabled(true);
        SendData->setEnabled(true);
        SendByTime->setEnabled(true);

        if(SendByTime->isChecked())
        {
            autoSend();
        }
    }
    else
    {
        UartSwitch->setText(tr("打开串口"));
        fComOpen=false;
        /*关闭串口*/
        ser.close();
        ReceiveTimer->stop();
        ComBox->setEnabled(true);
        BaudBox->setEnabled(true);
        ByteSizeBox->setEnabled(true);
        ParityBox->setEnabled(true);
        StopBitBox->setEnabled(true);

        SendInHex->setEnabled(false);
        SendData->setEnabled(false);
        SendByTime->setEnabled(false);

        if(SendTimer->isActive())
        {
            SendTimer->stop();
        }
    }
}

void MainWindow::autoSend()
{
    if(fComOpen&&SendByTime->isChecked())
    {
        if(SendTimeSet->text().toInt()>=1)
        {
            SendTimer->start(SendTimeSet->text().toInt());
            SendTimeSet->setEnabled(false);
        }
        else
        {
            ErrorDialog->showMessage("定时时间不能为0ms");
            SendByTime->setChecked(false);
        }
    }
    else
    {
        SendTimer->stop();
        SendTimeSet->setEnabled(true);
    }
}

void MainWindow::sendData()
{
    if(fComOpen)
    {
        bool fSend=true;
        if(!SendInHex->isChecked())
        {
            if(UseGB->isChecked())
            {
                fSend=ser.write(transcoding(SendBox->toPlainText().toUtf8(),"UTF-8","GB18030"));
            }
            else
            {
                fSend=ser.write(SendBox->toPlainText().toStdString().c_str());
            }
        }
        else
        {
            auto temp=strToHex(SendBox->toPlainText().toStdString());
            fSend=ser.write(temp.c_str());
        }
        if(!fSend)
        {
            ErrorDialog->showMessage(tr("发送失败,可能是串口已断开,即将关闭串口"));
            comSwitch();
        }
    }
    else
    {
        ErrorDialog->showMessage("尚未启动串口");
    }
}

void MainWindow::ReceiveData()
{
    auto DataBuffer=ser.readAll();
    if(!DataBuffer.isEmpty())
    {
        ReceiveBox->moveCursor(QTextCursor::EndOfBlock);
        if(DisplayByHex->isChecked())
        {
            ReceiveBox->insertPlainText(hexToStr((unsigned char *)DataBuffer.data()));
        }
        else
        {
            if(UseGB->isChecked())
            {
                ReceiveBox->insertPlainText(transcoding(DataBuffer,"GB18030","UTF-8"));
            }
            else
            {
                ReceiveBox->insertPlainText(DataBuffer);
            }
        }
    }
}

void MainWindow::updateComBox(const QList<QSerialPortInfo> &now)
{
    /*寻找减少*/
    for(int i=0;i<ComBox->count();i++)
    {
        auto inow=now.begin();
        for(;inow!=now.end();++inow)
        {
            if(ComBox->itemText(i)==inow->description()+" "+inow->portName())
            {
                break;
            }
        }
        if(inow==now.end())
        {
            ComBox->removeItem(i);
        }
    }

    /*寻找增加*/
    for(auto inow=now.begin();inow!=now.end();++inow)
    {
        int i=0;
        for(;i<ComBox->count();i++)
        {
            if(ComBox->itemText(i)==inow->description()+" "+inow->portName())
            {
                break;
            }
        }
        if(i==ComBox->count())
        {
            ComBox->addItem(inow->description()+" "+inow->portName());
        }
    }

    /*自适应调整下拉宽度*/
    int maxLen=0;
    for(auto &i:now)
    {
        if(maxLen<(i.description()+" "+i.portName()).size())
        {
            maxLen=(i.description()+" "+i.portName()).size();
        }
    }

    ComBox->view()->setMinimumWidth(maxLen*6+10);
}

bool operator==(const QSerialPortInfo &lhs,const QSerialPortInfo &rhs)
{
    return lhs.portName()==rhs.portName() &&
           lhs.description()==rhs.description();
}

QString MainWindow::getComName(const QString &ComBoxText)
{
    QString result;

    auto pText=ComBoxText.end();
    --pText;
    for(;*pText!=' ';--pText);
    pText++;

    for(;pText!=ComBoxText.end();++pText)
    {
        result+=*pText;
    }

    return result;
}
