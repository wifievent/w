#include "fullscan.h"
#include "arppacket.h"

void FullScan::start(){
    while(end_check) {
        scan();
        update_DB();
        gtrace("test1");
        sleep(3);
    }
}

void FullScan::scan(){
    m.lock();
    WIntf* intf = instance.getDevice().intf();
    ARPPacket arp_packet;
    arp_packet.makeArppacket(WMac("FF:FF:FF:FF:FF:FF"), intf->mac(), WMac("00:00:00:00:00:00"), WIp::nullIp(), intf->ip());

    uint32_t beginIp = (intf->ip() & intf->mask())+1;
    uint32_t endIp = (intf->ip() | ~intf->mask())-1;
    m.unlock();

    std::map<WMac, Host>::iterator iter;

    for(iter = fs_map.begin(); iter != fs_map.end(); ++iter){
        iter->second.is_connect = 0;
    }

    //find all ip connected to the network
    for(uint32_t ip = beginIp; ip != endIp; ++ip){
        if(WIp(ip) == intf->gateway()) { continue; }
        arp_packet.packet.arp.tip_ = htonl(WIp(ip));
        arp_packet.send(3);//send packet
    }
}

void FullScan::update_DB(){//update last_ip
    DB_Connect db_connect("/home/kali/BoB-10/project/w/app/hostscan-test/test.db");
    std::list<Data_List> d1;
    char query[50];
    d1 = db_connect.select_query("SELECT * FROM host");
    for(std::map<WMac,Host>::iterator fs_iter = fs_map.begin(); fs_iter != fs_map.end(); ++fs_iter){ //fullscan
        int tmp = 0;
        for(std::list<Data_List>::iterator data_iter = d1.begin(); data_iter != d1.end(); ++data_iter) {
            if(WMac(data_iter->argv[1]) == fs_iter->first){//same mac
                if(WIp(data_iter->argv[2]) != fs_iter->second.ip_){//need update
                    sprintf(query, "UPDATE host SET last_ip = '%s' WHERE mac = '%s'", std::string((fs_iter->second).ip_).data(), std::string(fs_iter->first).data());
                    db_connect.send_query(query);
                }
                tmp = 1;
                break;
            }
        }
        if(tmp == 0) {
            //different mac -> insert
            sprintf(query, "INSERT INTO host(mac, last_ip, name) VALUES('%s', '%s', '%s')", std::string(fs_iter->first).data(), std::string((fs_iter->second).ip_).data(), fs_iter->second.name);
            db_connect.send_query(query);
        }
    }
    Data_List::list_free(d1);
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


void FullScan::addHost(std::pair<WMac,Host> host) {
    fs_map.insert(host);
}

bool FullScan::isConnect(std::string mac) {
    WMac wmac(mac);

    return true;
}

void FullScan::delHost(std::string mac) {
    WMac wmac(mac);
    fs_map.erase(wmac);
}