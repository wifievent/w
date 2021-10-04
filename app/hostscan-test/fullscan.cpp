#include "fullscan.h"
#include "arppacket.h"

void FullScan::start()
{
    while(end_check) {
        scan();
        update_DB();
        sleep(3);
    }
}

void FullScan::scan()
{
    WIntf* intf = instance.getDevice().intf();
    ARPPacket arp_packet;

    arp_packet.makeArppacket(WMac("FF:FF:FF:FF:FF:FF"), intf->mac(), WMac("00:00:00:00:00:00"), WIp::nullIp(), intf->ip());

    uint32_t beginIp = (intf->ip() & intf->mask())+1;
    uint32_t endIp = (intf->ip() | ~intf->mask())-1;

    fs_map.m.lock();
    for(map<WMac, Host>::iterator iter = fs_map.begin(); iter != fs_map.end(); ++iter) {
        gettimeofday(&iter->second.last,NULL);
    }
    fs_map.m.unlock();

    //find all ip connected to the network
    for(uint32_t ip = beginIp; ip != endIp; ++ip) {
        if(WIp(ip) == intf->gateway()) { continue; } //if gateway -> continue
        arp_packet.packet.arp.tip_ = htonl(WIp(ip));
        arp_packet.send(3);//send packet
        usleep(3921); //send 255 packets per 1 second
    }
}

void FullScan::update_DB()//update last_ip
{
    DB_Connect db_connect("/home/bob/pm/w/app/hostscan-test/test.db");
    list<Data_List> d1 = db_connect.select_query("SELECT * FROM host");

    char query[50];
    int tmp;

    fs_map.m.lock();
    gtrace("size : %d",fs_map.size());

    for(map<WMac,Host>::iterator fs_iter = fs_map.begin(); fs_iter != fs_map.end(); ++fs_iter) { //fullscan
        tmp = 0;

        gtrace("mac = %s",std::string(fs_iter->first).data());
        gtrace("ip = %s",std::string((fs_iter->second).ip_).data());
        gtrace("name = %s",(fs_iter->second).name.data());

        for(list<Data_List>::iterator data_iter = d1.begin(); data_iter != d1.end(); ++data_iter) { // check host db
            if(WMac(data_iter->argv[1]) == fs_iter->first) {//same mac
                if(WIp(data_iter->argv[2]) != fs_iter->second.ip_) {//need update
                    sprintf(query, "UPDATE host SET last_ip = '%s' WHERE mac = '%s';", std::string((fs_iter->second).ip_).data(), std::string(fs_iter->first).data());
                    db_connect.send_query(query);
                }
                tmp = 1;
                break;
            }
        }
        if(tmp == 0) {
            sprintf(query, "INSERT INTO host(mac, last_ip, name) VALUES('%s', '%s', '%s');", std::string(fs_iter->first).data(), std::string((fs_iter->second).ip_).data(), fs_iter->second.name.data());
            db_connect.send_query(query);
        }
    }
    fs_map.m.unlock();
    Data_List::list_free(d1);
}

void FullScan::finish(){

}
void FullScan::findName(Host* g) {
    time_t start,end;
    string fname = "nmblookup -A ";
    string fullname = fname + std::string(g->ip_);
    FILE *fp = popen(fullname.c_str(), "r");

    start = time(NULL);

    if(fp == NULL) {
        perror("popen() fail");
        exit(1);
    }

    char buf[1024];
    std::string str;

    fgets(buf, 1024, fp);
    end = time(NULL);
    if(end-start>2) {
        g->name=string(g->ip_);
        gtrace("%s", g->name.data());
    }else if(fgets(buf, 1024, fp)) {
        string str(strtok(buf, " "));
        str.erase(str.begin());
        g->name =str.data();
        gtrace("%s", g->name.data());
    }
}

void FullScan::addHost(pair<WMac,Host> host) {
    std::lock_guard<mutex> lock(fs_map.m);
    fs_map.insert(host);
}

bool FullScan::isConnect(string mac) {
    fs_map.m.lock();
    map<WMac, Host>::iterator iter = fs_map.find(mac);
    fs_map.m.unlock();
    return iter->second.isConnected();
}

void FullScan::delHost(std::string mac) {
    WMac wmac(mac);
    fs_map.m.lock();
    fs_map.erase(wmac);
    fs_map.m.unlock();
}
