#ifndef POLICYOBJ_H
#define POLICYOBJ_H

#include <QtCore>
#include <QtWidgets>

class PolicyObj
{
public:
    PolicyObj();
    int policyId;
    QString start_time;
    QString end_time;
    int day_of_the_week;
    int hostId;
    QString name;

    void reset() {
      policyId = 0;
      start_time = nullptr;
      end_time = nullptr;
      day_of_the_week = -1;
      hostId = 0;
      name = nullptr;
    };
};

#endif // POLICYOBJ_H
