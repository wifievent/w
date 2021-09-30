#include "arppacket.h"
#include "base/db-connect.h"

void NetBlock::getBlockHostMap(Week day, int hour, int minute){
    DB_Connect db_connect("/home/kali/BoB-10/project/w/app/hostscan-test/test.db");
    Host g;
    list<Data_List> d1,d2;
    d1 = db_connect.select_query("SELECT * FROM time");
    NBMap_new.clear();

    for(list<Data_List>::iterator iter = d1.begin(); iter != d1.end(); ++iter) {
        if(atoi(iter->argv[3])!=day)continue; // different day

        int st = atoi(iter->argv[1])/100;
        int et = atoi(iter->argv[2])/100;
        int sm = atoi(iter->argv[1])%100;
        int em = atoi(iter->argv[2])%100;
        if(st<=hour&&hour<=et&&sm<=minute&&minute<=em) {
            d2 = db_connect.select_query("SELECT * FROM policy");
            int host_id;
            for(list<Data_List>::iterator iter2 = d2.begin(); iter2 != d2.end(); ++iter2) {
                if(atoi(iter2->argv[2])==atoi(iter->argv[0])){
                    host_id = atoi(iter2->argv[1]);
                    break;
                }
            }
            d2 = db_connect.select_query("SELECT * FROM host");
            for(list<Data_List>::iterator iter2 = d2.begin(); iter2 != d2.end(); ++iter2) {
                if(atoi(iter2->argv[0])==host_id){
                    g.mac_ = WMac(iter2->argv[1]);
                    g.ip_ = WIp(iter2->argv[2]);
                    g.name = iter2->argv[3];
                    break;
                }
            }
            NBMap_new.insert(pair<WMac,Host>(g.mac_, g));
        }
    }
    Data_List::list_free(d1);
    Data_List::list_free(d2);
}

void NetBlock::send_infect(){//full-scan : active & policy
    Infection infect;
    infect.send();
}

void NetBlock::update_map(){
    Recover recover;
    time_t timer;
    struct tm* t;

    while(end_check) {
        timer = time(NULL);
        gtrace("nb timer: %d \n", timer);
        t = localtime(&timer);
        if(t->tm_min % 10 != 0 && t->tm_sec != 0) {
            continue;
        }
        getBlockHostMap((Week)t->tm_wday,t->tm_hour,t->tm_min);//update NBmap
        recover.send();//recover

        if(NBMap_new.size() > 0){
            NBMap_old.clear();
            NBMap_old.insert(NBMap_new.begin(),NBMap_new.end());
        }
    }
}
