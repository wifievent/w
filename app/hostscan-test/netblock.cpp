#include "netblock.h"
#include "arppacket.h"

void NetBlock::getBlockHostMap(Week day, int hour, int minute){
    DB_Connect db_connect("/home/kali/BoB-10/project/w/app/hostscan-test/test.db");
    Host g;
    std::list<Data_List> d1,d2;
    d1 = db_connect.select_query("SELECT * FROM time");
    new_nb_map.clear();
    
    for(std::list<Data_List>::iterator iter = d1.begin(); iter != d1.end(); ++iter) {
        if(atoi(iter->argv[3])!=day) { continue; } // different day

        int sh = atoi(iter->argv[1])/100;
        int eh = atoi(iter->argv[2])/100;
        int sm = atoi(iter->argv[1])%100;
        int em = atoi(iter->argv[2])%100;
        if(sh <= hour && hour <= eh && sm <= minute && minute <= em) {
            d2 = db_connect.select_query("SELECT * FROM block_host");
            for(list<Data_List>::iterator iter2 = d2.begin(); iter2 != d2.end(); ++iter2) {
                g.mac_ = WMac(iter2->argv[0]);
                g.ip_ = WIp(iter2->argv[1]);
                g.name = iter2->argv[2];
                break;
            }
            new_nb_map.insert(pair<WMac, Host>(g.mac_, g));
        }
    }
    Data_List::list_free(d1);
    Data_List::list_free(d2);
}

void NetBlock::sendInfect(){//full-scan : is_connect & policy
    std::map<WMac,Host>::iterator iter;

    std::map<WMac, Host> fs_map = fs.getFsMap();
    
    Packet& instance = Packet::getInstance();

    ARPPacket infect_packet;

    while(end_check) {
        if(nb_map.size() == 0) { continue; }
        for(iter = nb_map.begin(); iter != nb_map.end(); ++iter) {
            if(fs_map[iter->first].is_connect){//full-scan & policy
                gtrace("<sendarp>");
                gtrace("%s",std::string(iter->first).data());
                gtrace("%s",std::string((iter->second).ip_).data());

                infect_packet.makeArppacket(iter->first, instance.getDevice().intf()->mac(), iter->first, (iter->second).ip_, instance.getDevice().intf()->gateway());
                infect_packet.packet.arp.op_ = htons(WArpHdr::Reply);
                infect_packet.send(3);
            }
        }
        sleep(30);
    }
}

void NetBlock::sendRecover(Host host) {
    ARPPacket recover_packet;
    recover_packet.makeArppacket(host.mac_, recover_packet.intf_g->mac(), host.mac_, host.ip_, recover_packet.intf_g->ip());
    m.lock();
    recover_packet.send(3);
    m.unlock();
}

void NetBlock::update_map(){
    time_t timer;
    struct tm* t;

    while(end_check) {
        timer = time(NULL);
        t = localtime(&timer);
        if(t->tm_min % 10 != 0 && t->tm_sec != 0) {
            continue;
        }
        getBlockHostMap((Week)t->tm_wday,t->tm_hour,t->tm_min);//update NBmap
        
        for(std::map<WMac, Host>::iterator iter_old = nb_map.begin(); iter_old != nb_map.end(); ++iter_old) {
            if(new_nb_map.find(iter_old->first) == new_nb_map.end()) {
                sendRecover(iter_old->second);
            }
        }

        if(new_nb_map.size() > 0){
            nb_map.clear();
            nb_map.swap(new_nb_map);
        }
    }
}
