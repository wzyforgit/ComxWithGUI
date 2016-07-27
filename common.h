#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <QString>
#include <QByteArray>

std::string strToHex(const std::string &str);
QString hexToStr(const unsigned char *data);
QByteArray transcoding(QByteArray str,const char *srcCode,const char *dstCode);

#endif // COMMON_H
