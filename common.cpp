#include "common.h"
#include <string>
#include <cctype>
#include <QTextCodec>

std::string strToHex(const std::string &str)
{
    std::string temp;
    for(decltype(str.size()) count=0;count!=str.size()&&count!=str.size()-1;count+=2)
    {
        int high;
        int low;
        char strTemp=toupper(str[count]);
        if(strTemp>='0'&&strTemp<='9')
        {
            high=strTemp-48;
        }
        else if(strTemp>='A'&&strTemp<='F')
        {
            high=strTemp-55;
        }
        else
        {
            return temp;
        }

        strTemp=toupper(str[count+1]);
        if(strTemp>='0'&&strTemp<='9')
        {
            low=strTemp-48;
        }
        else if(strTemp>='A'&&strTemp<='F')
        {
            low=strTemp-55;
        }
        else
        {
            return temp;
        }

        temp.push_back(high<<4|low);
    }
    return temp;
}

QString hexToStr(const unsigned char *data)
{
    QString result;
    unsigned char temp;

    while(*data!='\0')
    {
        temp=*data>>4;
        if(temp>=0x0A&&temp<=0x0F)
        {
            temp+=55;
        }
        else
        {
            temp+=48;
        }
        result+=static_cast<char>(temp);

        temp=*data&0x0F;
        if(temp>=0x0A&&temp<=0x0F)
        {
            temp+=55;
        }
        else
        {
            temp+=48;
        }
        result+=static_cast<char>(temp);

        result+=' ';
        data++;
    }
    return result;
}

QByteArray transcoding(QByteArray str,const char *srcCode,const char *dstCode)
{
    QTextCodec *codec=QTextCodec::codecForName(srcCode);
    QString temp=codec->toUnicode(str);

    QTextCodec *decodec=QTextCodec::codecForName(dstCode);
    return (decodec->fromUnicode(temp));
}
