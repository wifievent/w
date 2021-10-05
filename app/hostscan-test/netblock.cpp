#include "arppacket.h"
#include "/home/bob/project/host-list/w/app/db-connect/db-connect.h"

void NetBlock::getBlockHostMap(Week day, int hour, int minute){
    DB_Connect db_connect("/home/bob/real/arp-sppof/test.db");
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
void NetBlock::update_DB(){//update last_ip
    DB_Connect db_connect("/home/bob/real/arp-sppof/test.db");
    list<Data_List> d1;
    char query[50];
    d1 = db_connect.select_query("SELECT * FROM host");
    for(map<WMac,Host>::iterator iter = fs.getMap().begin(); iter!=fs.getMap().end();iter++){ //fullscan
        int tmp = 0;
        for(list<Data_List>::iterator iter2 = d1.begin(); iter2 != d1.end(); ++iter2) {
            if(WMac(iter2->argv[1])==iter->first){//same mac
                if(WIp(iter2->argv[2])!=iter->second.ip_){//need update
                    sprintf(query,"UPDATE host SET last_ip = '%s' WHERE mac = '%s'",std::string((iter->second).ip_).data(),std::string(iter->first).data());
                    db_connect.send_query(query);
                }else{//if exist
                    tmp = 1;
                }
                break;
            }
        }
        //different mac -> insert
        sprintf(query,"INSERT INTO host VALUES(%d, '%s', '%s', '%s')",fs.getMap().size()+1,std::string(iter->first).data(),std::string((iter->second).ip_).data(),iter->second.name);
        db_connect.send_query(query);
    }
    Data_List::list_free(d1);
}

void NetBlock::update_map(){
    Recover recover;
    time_t timer;
    struct tm* t;
    timer = time(NULL);

    while(check) {
        fs.start(); //full-scan
        sleep(3);
        update_DB(); //update_db

        t = localtime(&timer);
        getBlockHostMap((Week)t->tm_wday,t->tm_hour,t->tm_min);//update NBmap
        recover.send();//recover

        if(NBMap_new.size() > 0){
            NBMap_old.clear();
            NBMap_old.insert(NBMap_new.begin(),NBMap_new.end());
        }
    }
}
