#pragma once
#include "stdafx.h"

class Socket
{
protected:
    int sock;

protected:
    ~Socket();
    void error_handling(string msg);
};