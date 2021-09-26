#ifndef DINFO_H
#define DINFO_H

#include <QString>

class dInfo
{
public:
    dInfo();
    int hostID;
    QString oui;
    QString mac;
    QString ip;
    QString name;
    int vectorID;
};

#endif // DINFO_H
