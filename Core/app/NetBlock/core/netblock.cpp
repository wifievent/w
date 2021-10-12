#include "netblock.h"
#include "arppacket.h"

void NetBlock::sendInfect()//full-scan : is_connect & policy
{
    std::map<WMac, Host> fs_map = fsInstance.getFsMap();
    ARPPacket infect_packet;

    time_t timer;
    struct tm* t;

    while(end_check) {
        std::lock_guard<std::mutex> lock(m);
        if(nbMap.size() == 0) { continue; }
        for(std::map<WMac,Host>::iterator iter = nbMap.begin(); iter != nbMap.end(); ++iter) {
            if(fsInstance.isConnect(std::string(iter->first))){//full-scan & policy
                timer = time(NULL);
                t = localtime(&timer);

                GTRACE("<sendarp>");
                GTRACE("time = %d:%d:%d",(Week)t->tm_wday,t->tm_hour,t->tm_min);
                GTRACE("%s",std::string(iter->first).data());
                GTRACE("%s",std::string((iter->second).ip_).data());

                {
                    std::lock_guard<std::mutex> lock(packet_instance.m);
                    infect_packet.makeArppacket(iter->first, packet_instance.intf()->mac(), iter->first, (iter->second).ip_, packet_instance.intf()->gateway());
                }
                infect_packet.packet.arp.op_ = htons(WArpHdr::Reply);
                infect_packet.send(sendInfectNum);
                GTRACE("\n!!!");

                {
                    std::lock_guard<std::mutex> lock(packet_instance.m);
                    infect_packet.makeArppacket(infect_packet.gate_mac, packet_instance.intf()->mac(), infect_packet.gate_mac, packet_instance.intf()->gateway(), (iter->second).ip_);
                }
                infect_packet.packet.arp.op_ = htons(WArpHdr::Reply);
                infect_packet.send(sendInfectNum);
                GTRACE("\n@@@");
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(nbTime));//sleep 30s
    }
}

void NetBlock::sendRecover(Host host)
{
    ARPPacket recover_packet;
    recover_packet.makeArppacket(host.mac_, recover_packet.gate_mac, host.mac_, host.ip_, recover_packet.gate_ip);
    recover_packet.send(sendRecoverNum);
}

void NetBlock::getBlockHostMap()
{
    DB_Connect& db_connect = DB_Connect::getInstance();
    Host g;
    std::list<Data_List> d1;
    newNbMap.clear();

    d1 = db_connect.select_query("SELECT * FROM block_host");
    for(std::list<Data_List>::iterator iter2 = d1.begin(); iter2 != d1.end(); ++iter2) {
        g.mac_ = WMac(iter2->argv[0]);
        g.ip_ = WIp(iter2->argv[1]);
        g.name = iter2->argv[2];
        newNbMap.insert(std::pair<WMac, Host>(g.mac_, g));
    }
}

void NetBlock::updateMap()
{
    time_t timer;
    struct tm* t;
    int cnt = 0;
    while(end_check) {
        timer = time(NULL);
        t = localtime(&timer);
        if(t->tm_min % dbMin != 0 || t->tm_sec != 0) {
            cnt = 0;
            continue;
        }
        if(cnt++ == 0) {
            GTRACE("\n<updateMap: h: %d, m: %d, s: %d>", t->tm_hour, t->tm_min, t->tm_sec);
            getBlockHostMap();//update NBmap

            for(std::map<WMac, Host>::iterator iter_old = nbMap.begin(); iter_old != nbMap.end(); ++iter_old) {
                if(newNbMap.find(iter_old->first) == newNbMap.end()) { //finish policy
                    sendRecover(iter_old->second);
                }
            }
            {
                std::lock_guard<std::mutex> lock(m);
                if(newNbMap.size() > 0) {
                    nbMap.clear();
                    nbMap.swap(newNbMap);
                }
            }
        }
    }
}

void NetBlock::sendRelay(WPacket& packet)
{
    if(packet.ethHdr_->smac()==packet.ethHdr_->dmac()) {
        packet_instance.write(&packet);
    }
}

void NetBlock::load(Json::Value& json) {
    json["nb_time"] >> nbTime;
    json["db_min"] >> dbMin;
    json["sendInfectNum"] >> sendInfectNum;
    json["sendRecoverNum"] >> sendRecoverNum;
}

void NetBlock::save(Json::Value& json) {
    json["nb_time"] << nbTime;
    json["db_min"] << dbMin;
    json["sendInfectNum"] << sendInfectNum;
    json["sendRecoverNum"] << sendRecoverNum;
}
