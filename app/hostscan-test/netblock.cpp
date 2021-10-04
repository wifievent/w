#include "netblock.h"
#include "arppacket.h"

void NetBlock::getBlockHostMap(Week day, int hour, int minute)
{
    DB_Connect db_connect("/home/bob/pm/w/app/hostscan-test/test.db");
    Host g;
    list<Data_List> d1,d2,d3;
    int host_id, time_id;

    d1 = db_connect.select_query("SELECT * FROM time");

    m.lock();
    new_nb_map.clear();
    m.unlock();

    for(std::list<Data_List>::iterator iter = d1.begin(); iter != d1.end(); ++iter) { // check time db
        if(atoi(iter->argv[3])!=day) { continue; } // different day

        int sh = atoi(iter->argv[1])/100;
        int eh = atoi(iter->argv[2])/100;
        int sm = atoi(iter->argv[1])%100;
        int em = atoi(iter->argv[2])%100;

        if(sh <= hour && hour <= eh) {//check time
            if(sh == hour && sm > minute) { continue; }
            else if(hour == eh && em < minute) { continue; }

            time_id = atoi(iter->argv[0]);

            d2 = db_connect.select_query("SELECT * FROM policy");

            for(list<Data_List>::iterator iter2 = d2.begin(); iter2 != d2.end(); ++iter2) { // check policy db
                if(time_id == atoi(iter2->argv[2])) {
                    host_id = atoi(iter2->argv[1]);
                    d3 = db_connect.select_query("SELECT * FROM host");
                    for(list<Data_List>::iterator iter3 = d3.begin(); iter3 != d3.end(); ++iter3) { // check host db
                        if(host_id == atoi(iter3->argv[0])) {
                            g.mac_ = WMac(std::string(iter3->argv[1]));
                            g.ip_ = WIp(std::string(iter3->argv[2]));
                            g.name = iter3->argv[2];

                            gtrace("<Have to block>");
                            gtrace("time = %d : %d : %d",day,hour,minute);
                            gtrace("%s",string(g.mac_).data());
                            gtrace("%s",string(g.ip_).data());
                            gtrace("%s",g.name.data());

                            m.lock();
                            new_nb_map.insert(pair<WMac, Host>(g.mac_, g));
                            m.unlock();
                            break;
                        }
                    }
                }
            }
        }
    }
    Data_List::list_free(d1);
    Data_List::list_free(d2);
    Data_List::list_free(d3);
}

void NetBlock::sendInfect(){//full-scan = is_connect & policy
    map<WMac, Host> fs_map = fs.getFsMap();

    Packet& instance = Packet::getInstance();
    ARPPacket infect_packet;

    time_t timer;
    struct tm* t;

    while(end_check) {
        {
            std::lock_guard<mutex> lock(m);
            if(nb_map.size() == 0) { continue; }
        }
        m.lock();
        for(map<WMac,Host>::iterator iter = nb_map.begin(); iter != nb_map.end(); ++iter) {
            if(fs_map[iter->first].isConnected()){//full-scan & policy
                timer = time(NULL);
                t = localtime(&timer);

                gtrace("<sendarp>");
                gtrace("time = %d:%d:%d",(Week)t->tm_wday,t->tm_hour,t->tm_min);
                gtrace("%s",std::string(iter->first).data());
                gtrace("%s",std::string((iter->second).ip_).data());

                infect_packet.makeArppacket(iter->first, instance.getDevice().intf()->mac(), iter->first, (iter->second).ip_, instance.getDevice().intf()->gateway());
                infect_packet.packet.arp.op_ = htons(WArpHdr::Reply);
                infect_packet.send(3);
            }
        }
        m.unlock();
        sleep(30);
    }
}

void NetBlock::sendRecover(Host host) {
    ARPPacket recover_packet;
    recover_packet.makeArppacket(host.mac_, recover_packet.intf_g->mac(), host.mac_, host.ip_, recover_packet.intf_g->ip());
    recover_packet.send(3);
}

void NetBlock::update_map(){
    time_t timer;
    struct tm* t;

    while(end_check) {
        timer = time(NULL);
        t = localtime(&timer);

        if(t->tm_min % 10 != 0 && t->tm_sec != 0) { continue; } //per 10 minutes

        getBlockHostMap((Week)t->tm_wday,t->tm_hour,t->tm_min);//update NBmap

        m.lock();
        for(map<WMac, Host>::iterator iter_old = nb_map.begin(); iter_old != nb_map.end(); ++iter_old) {
            if(new_nb_map.find(iter_old->first) == new_nb_map.end()) {
                sendRecover(iter_old->second);//end policy
            }
        }
        m.unlock();

        m.lock();
        if(new_nb_map.size() > 0){
            nb_map.clear();
            nb_map.swap(new_nb_map);
        }
        m.unlock();
    }
}
