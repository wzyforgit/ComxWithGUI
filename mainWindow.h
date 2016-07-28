#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QRadioButton>

class QComboBox;
class QPushButton;
class QLineEdit;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QTextEdit;
class QErrorMessage;
class QCheckBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    static const int ReceiveCycle=20;
public:
    MainWindow(QWidget *parent = 0);
public slots:
    void comSwitch();
    void autoSend();
    void ReceiveData();
private:
    void createUartBoxs();
    void updateComBox(const QList<QSerialPortInfo> &now);
    QTimer *updateSerialInfo;

    QComboBox *ComBox;
    QComboBox *BaudBox;
    QComboBox *ParityBox;
    QComboBox *ByteSizeBox;
    QComboBox *StopBitBox;
    QPushButton *UartSwitch;
    QString getComName(const QString &ComBoxText);

    QTextEdit *ReceiveBox;
    QPushButton *ReceiveBoxClean;
    QCheckBox *DisplayByHex;

    QButtonGroup *CodeSet;
    QRadioButton *UseGB;
    QRadioButton *UseUTF;

    void sendData();
    QTextEdit *SendBox;
    QPushButton *SendData;
    QCheckBox *SendInHex;
    QLabel *SendTimeMs;
    QLineEdit *SendTimeSet;
    QCheckBox *SendByTime;
    QTimer *SendTimer;

    bool fComOpen=false;

    void createUartLabels();
    QLabel *ComLabel;
    QLabel *BaudLabel;
    QLabel *ParityLabel;
    QLabel *ByteSizeLabel;
    QLabel *StopBitLabel;

    void createUartLayout();
    QHBoxLayout *mainLayout;

    QErrorMessage *ErrorDialog;

    QSerialPort ser;
    QTimer *ReceiveTimer;
};

bool operator==(const QSerialPortInfo &lhs,const QSerialPortInfo &rhs);

#endif // MAINWINDOW_H
