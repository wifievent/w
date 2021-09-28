#include "fullscan.h"
class Parser
{
public:
    Parser() {};
    ~Parser() {};
    FullScan& fs = FullScan::instance_fs();
    Packet& instance = Packet::instance();
    virtual void parse(){};
};

class DHCPParser: public Parser{
public:
    virtual void parse()override;
};

class ARPParser: public Parser{
public:
    virtual void parse()override;
};
