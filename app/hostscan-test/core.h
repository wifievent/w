#include "netblock.h"
class Core
{
private:
    FullScan& fs = FullScan::instance_fs();
    NetBlock nb;
    thread* recv_;
    thread* dhcp_;
public:
    Core(){};
    ~Core(){};
    void start();//fullscan(connection) -> receive_packet
    void end();//program end
};

