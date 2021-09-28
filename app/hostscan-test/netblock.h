#include "fullscan.h"
class NetBlock
{
private:
    map<WMac,Host> NBMap;
    FullScan& fs = FullScan::instance_fs();
    mutex m;
public:
    NetBlock(){};
    ~NetBlock(){};
    enum Week{
        Sunday = 0,
        Monday = 1,
        Tuesday = 2,
        Wednesday = 3,
        Thirsday = 4,
        Friday = 5,
        Saturday = 6,
    };
    void getBlockHostMap(Week day, int hour, int minute);
    void send_infect();//no sleep
    void update_DB();
    void update_map();//db list update -> 1. db read 2. compare : new-> input list, have to remove -> recover -> per 5minute
};
