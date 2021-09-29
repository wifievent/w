#include "arppacket.h"
#include "parser.h"
void FullScan::start(){
    Connection conn;
    ARPParser arp;

    thread conn_(&Connection::send,&conn,FSMap); //find what device is connected to network
    thread arp_(&ARPParser::parse,&arp); //arp packet parsing

    conn_.detach();
    arp_.join();
}
void FullScan::finish(){

}
void FullScan::findName(){
    map<WMac,Host>::iterator iter;
    for(iter = FSMap.begin(); iter!=FSMap.end();iter++){
        gtrace("%s",string(iter->first).data());
        string fname = "nmblookup -A ";
        string fullname = fname + string((iter->second).ip_);
        FILE *fp = popen(fullname.c_str(),"r");

        if(fp == NULL){
            perror("popen() fail");
            exit(1);
        }

        char buf[1024];
        string str;
        fgets(buf,1024,fp);
        if(fgets(buf,1024,fp)){
            string str(strtok(buf," "));
            str.erase(str.begin());
            (iter->second).name = (char*)malloc(sizeof(char)*str.size());
            strcpy((iter->second).name,str.c_str());
            gtrace(str.c_str());
        }
    }
}

void FullScan::send_ARPpacket(EthArp etharp, int cnt){//every packet sending
    WPacket& packet = getWPacket();
    packet.buf_.data_ = reinterpret_cast<byte*>(&etharp);
    packet.buf_.size_ = sizeof(EthArp);
    for(int i =0; i<cnt; i++){
        instance.getDevice().write(packet.buf_);
    }
}

void FullScan::receive_packet(){//every packet receiving
    WPacket& packet = getWPacket();
    while(1){
        getMutex().lock();
        if(instance.getDevice().WPcapCapture::read(&packet)==WPacket::Result::Ok){ //if packet is ok
            setWPacket(&packet);//singleton pattern
            return;
        }
        getMutex().unlock();
    }
}
