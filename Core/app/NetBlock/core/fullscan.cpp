#include "fullscan.h"
#include "arppacket.h"

void FullScan::start()
{
    while(end_check) {
        GTRACE("scan");
        scan();
        updateDB();
        std::this_thread::sleep_for(std::chrono::milliseconds(fsTime));//sleep 10s
    }
}

void FullScan::setHostMap()//update fsmap
{
    DB_Connect& db_connect = DB_Connect::getInstance();
    std::list<Data_List> host_list = db_connect.select_query("SELECT * FROM host");

    for(std::list<Data_List>::iterator iter = host_list.begin(); iter != host_list.end(); ++iter) {
        Host g;
        g.mac_ = WMac(iter->argv[1]);
        g.ip_ = WIp(iter->argv[2]);
        g.name = std::string(iter->argv[3]);

        struct timeval now;
        gettimeofday(&now, NULL);
        now.tv_sec -= 60;
        g.last = now;

        std::lock_guard<std::mutex> lock(fsMap.m);
        fsMap.insert(std::pair<WMac, Host>(g.mac_, g));
    }
}

void FullScan::scan()
{
    ARPPacket arp_packet;
    WMac my_mac;
    WIp my_ip;
    WIp mask;
    WIp gateway;
    {
        std::lock_guard<std::mutex> lock(packetInstance.m);
        my_mac = packetInstance.intf()->mac();
        my_ip = packetInstance.intf()->ip();
        mask = packetInstance.intf()->mask();
        gateway = packetInstance.intf()->gateway();
    }
    arp_packet.makeArppacket(WMac("FF:FF:FF:FF:FF:FF"), my_mac, WMac("00:00:00:00:00:00"), WIp::nullIp(), my_ip);


    uint32_t beginIp = (my_ip & mask)+1;
    uint32_t endIp = (my_ip | ~mask)-1;

    //find all ip connected to the network
    for(uint32_t ip = beginIp; ip != endIp; ++ip) {
        arp_packet.packet.arp.tip_ = htonl(WIp(ip));
        arp_packet.send(sendCountPerIp);//send packet -> json화 sendCountPerIp
        std::this_thread::sleep_for(std::chrono::milliseconds(sendCountForNextIp)); //짧은 시간으로 sendCountForNextIp
    }
}

void FullScan::updateDB()//update last_ip
{
    DB_Connect& db_connect = DB_Connect::getInstance();
    std::list<Data_List> d1;
    std::string query;

    d1 = db_connect.select_query("SELECT * FROM host");
    {
        std::lock_guard<std::mutex> lock(fsMap.m);
        for(std::map<WMac,Host>::iterator fsIter = fsMap.begin(); fsIter != fsMap.end(); ++fsIter) { //fullscan
            int tmp = 0;
            for(std::list<Data_List>::iterator dataIter = d1.begin(); dataIter != d1.end(); ++dataIter) {
                if(WMac(dataIter->argv[1]) == fsIter->first) {//same mac
                    if(WIp(dataIter->argv[2]) != fsIter->second.ip_) {//need update
                        query = "UPDATE host SET last_ip = '"+std::string((fsIter->second).ip_)+"' WHERE mac = '"+std::string(fsIter->first)+"'";
                        db_connect.send_query(query.data());
                    }
                    tmp = 1;
                    break;
                }
            }
            if(tmp == 0) {
                //different mac -> insert
                GTRACE("\n\n\nINSERT!!!!!!!!!");
                query = "INSERT INTO host(mac, last_ip, name) VALUES('"+std::string(fsIter->first)+"', '"+ std::string((fsIter->second).ip_).data() +"', '"+ fsIter->second.name.data() + "')";
                db_connect.send_query(query.data());
            }
        }
    }
}

void FullScan::updateHostInfo(WMac mac_, WIp ip_, struct timeval last_)
{
    std::lock_guard<std::mutex> lock(fsMap.m);
    fsMap[mac_].ip_ = ip_;
    fsMap[mac_].last = last_;
    GTRACE("ip = %s",std::string(ip_).data());
    GTRACE("time = %d",last_.tv_sec);
}

void FullScan::addHost(std::pair<WMac,Host> host)
{
    std::lock_guard<std::mutex> lock(fsMap.m);
    fsMap.insert(host);
}

bool FullScan::isConnect(std::string mac)
{
    WMac wmac(mac);
    std::lock_guard<std::mutex> lock(fsMap.m);
    return fsMap[wmac].isConnected();
}

void FullScan::delHost(std::string mac)
{
    WMac wmac(mac);
    std::lock_guard<std::mutex> lock(fsMap.m);
    fsMap.erase(wmac);
}
void FullScan::load(Json::Value& json) {
    json["fs_time"] >> fsTime;
    json["sendCountPerIp"] >> sendCountPerIp;
    json["sendCountForNextIp"] >> sendCountForNextIp;
}

void FullScan::save(Json::Value& json) {
    json["fs_time"] << fsTime;
    json["sendCountPerIp"] << sendCountPerIp;
    json["sendCountForNextIp"] << sendCountForNextIp;
}
