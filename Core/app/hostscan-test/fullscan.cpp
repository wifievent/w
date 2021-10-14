#include "fullscan.h"
#include "arppacket.h"

void FullScan::start()
{
    while(end_check) {
        GTRACE("scan");
        scan();
        updateDB();
        std::this_thread::sleep_for(std::chrono::milliseconds(fs_time));//sleep 3s
    }
}

void FullScan::setHostMap()
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

        std::lock_guard<std::mutex> lock(fs_map.m);
        fs_map.insert(std::pair<WMac, Host>(g.mac_, g));
    }
    Data_List::list_free(host_list);
}

void FullScan::scan()
{
    ARPPacket arp_packet;
    WMac my_mac;
    WIp my_ip;
    WIp mask;
    WIp gateway;
    {
        std::lock_guard<std::mutex> lock(packet_instance.m);
        my_mac = packet_instance.intf()->mac();
        my_ip = packet_instance.intf()->ip();
        mask = packet_instance.intf()->mask();
        gateway = packet_instance.intf()->gateway();
    }
    arp_packet.makeArppacket(WMac("FF:FF:FF:FF:FF:FF"), my_mac, WMac("00:00:00:00:00:00"), WIp::nullIp(), my_ip);


    uint32_t beginIp = (my_ip & mask)+1;
    uint32_t endIp = (my_ip | ~mask)-1;

    //find all ip connected to the network
    for(uint32_t ip = beginIp; ip != endIp; ++ip) {
        if(WIp(ip) == gateway) { continue; }
        arp_packet.packet.arp.tip_ = htonl(WIp(ip));
        arp_packet.send(3); //send packet
    }
}

void FullScan::updateDB() //update last_ip
{
    DB_Connect& db_connect = DB_Connect::getInstance();
    std::list<Data_List> d1;
    std::string query;

    d1 = db_connect.select_query("SELECT * FROM host");
    {
        std::lock_guard<std::mutex> lock(fs_map.m);
        for(std::map<WMac,Host>::iterator fs_iter = fs_map.begin(); fs_iter != fs_map.end(); ++fs_iter) { //fullscan
            int tmp = 0;
            for(std::list<Data_List>::iterator data_iter = d1.begin(); data_iter != d1.end(); ++data_iter) {
                if(WMac(data_iter->argv[1]) == fs_iter->first) {//same mac
                    if(WIp(data_iter->argv[2]) != fs_iter->second.ip_) {//need update
                        query = "UPDATE host SET last_ip = '"+std::string((fs_iter->second).ip_)+"' WHERE mac = '"+std::string(fs_iter->first)+"'";
                        db_connect.send_query(query.data());
                    }
                    tmp = 1;
                    break;
                }
            }
            if(tmp == 0) {
                //different mac -> insert
                GTRACE("\n\n\nINSERT!!!!!!!!!");
                query = "INSERT INTO host(mac, last_ip, name) VALUES('"+std::string(fs_iter->first)+"', '"+ std::string((fs_iter->second).ip_).data() +"', '"+ fs_iter->second.name.data() + "')";
                db_connect.send_query(query.data());
            }
        }
    }
    Data_List::list_free(d1);
}

void FullScan::updateHostInfo(WMac mac_, WIp ip_, struct timeval last_)
{
    std::lock_guard<std::mutex> lock(fs_map.m);
    fs_map[mac_].ip_ = ip_;
    fs_map[mac_].last = last_;
}

void FullScan::addHost(std::pair<WMac,Host> host)
{
    std::lock_guard<std::mutex> lock(fs_map.m);
    fs_map.insert(host);
}

bool FullScan::isConnect(std::string mac)
{
    WMac wmac(mac);
    std::lock_guard<std::mutex> lock(fs_map.m);
    return fs_map[wmac].isConnected();
}

void FullScan::delHost(std::string mac)
{
    WMac wmac(mac);
    std::lock_guard<std::mutex> lock(fs_map.m);
    fs_map.erase(wmac);
}
void FullScan::load(Json::Value& json) {
    json["fs_time"] >> fs_time;
}

void FullScan::save(Json::Value& json) {
    json["fs_time"] << fs_time;
}
