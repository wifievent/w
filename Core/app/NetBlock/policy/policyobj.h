#ifndef POLICYOBJ_H
#define POLICYOBJ_H

#include <QtCore>

class PolicyObj
{
protected:
    int policyId;
    QString startTime;
    QString endTime;
    int dayOfTheWeek;
    int hostId;
    QString name;

public:
    PolicyObj();
    ~PolicyObj();

    void reset();
    void set(std::vector<std::string> row);
    int getPolicyId();
    QString getStartTime();
    QString getEndTime();
    int getDayOfTheWeek();
    int getHostId();
    QString getName();
};

#endif // POLICYOBJ_H
