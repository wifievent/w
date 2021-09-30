#include "fullscan.h"

void FullScan::start(){
    conn_th = std::thread(&Connection::send, &conn); //find what device is connected to network
    conn_th.detach();
}
void FullScan::finish(){

}
void FullScan::findName(Host* g){
    gtrace("%s", std::string(g->mac_).data());
    std::string fname = "nmblookup -A ";
    std::string fullname = fname + std::string(g->ip_);
    FILE *fp = popen(fullname.c_str(), "r");

    if(fp == NULL){
        perror("popen() fail");
        exit(1);
    }

    char buf[1024];
    std::string str;
    fgets(buf, 1024, fp);
    if(fgets(buf, 1024, fp)){
        std::string str(strtok(buf, " "));
        str.erase(str.begin());
        g->name = (char*)malloc(sizeof(char) * str.size());
        strcpy(g->name, str.data());
        gtrace("%s", str.data());
    }
}

void FullScan::scan(){
    while(end_check) {
        start(); //full-scan
        update_DB();
        sleep(3);
    }
}

void FullScan::update_DB(){//update last_ip
    DB_Connect db_connect("/home/kali/BoB-10/project/w/app/hostscan-test/test.db");
    list<Data_List> d1;
    char query[50];
    d1 = db_connect.select_query("SELECT * FROM host");
    for(map<WMac,Host>::iterator iter = fs_map.begin(); iter!=fs_map.end();iter++){ //fullscan
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
        sprintf(query,"INSERT INTO host VALUES(%d, '%s', '%s', '%s')",fs_map.size()+1,std::string(iter->first).data(),std::string((iter->second).ip_).data(),iter->second.name);
        db_connect.send_query(query);
    }
    Data_List::list_free(d1);
}
void FullScan::addHost(std::pair<WMac,Host> host) {
    fs_map.insert(host);
}

bool FullScan::isConnect(std::string mac) {
    WMac wmac(mac);
    return fs_map[wmac].is_connect;
}

void FullScan::delHost(std::string mac) {
    WMac wmac(mac);
    fs_map.erase(wmac);
}