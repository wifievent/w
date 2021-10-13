#include "policyobj.h"

PolicyObj::PolicyObj() {}

PolicyObj::~PolicyObj() {}

void PolicyObj::reset()
{
  policyId = 0;
  startTime = nullptr;
  endTime = nullptr;
  dayOfTheWeek = -1;
  hostId = 0;
  name = nullptr;
};

void PolicyObj::set(std::vector<std::string> row)
{
    policyId = stoi(row[0]);
    startTime = QString::fromStdString(row[1]);
    endTime = QString::fromStdString(row[2]);
    dayOfTheWeek = stoi(row[3]);
    hostId = stoi(row[4]);
    name = QString::fromStdString(row[5]);
}

int PolicyObj::getPolicyId()
{
    return policyId;
}

QString PolicyObj::getStartTime()
{
    return startTime;
}

QString PolicyObj::getEndTime()
{
    return endTime;
}

int PolicyObj::getDayOfTheWeek()
{
    return dayOfTheWeek;
}

int PolicyObj::getHostId()
{
    return hostId;
}

QString PolicyObj::getName()
{
    return name;
}
