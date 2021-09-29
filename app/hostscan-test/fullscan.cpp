#include "parser.h"
void FullScan::start(){
    Connection conn;
    thread conn_(&Connection::send,&conn); //find what device is connected to network
    conn_.detach();
}
void FullScan::finish(){

}
void FullScan::findName(Host* g){
    gtrace("%s",string(g->mac_).data());
    string fname = "nmblookup -A ";
    string fullname = fname + string(g->ip_);
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
        g->name = (char*)malloc(sizeof(char)*str.size());
        strcpy(g->name,str.c_str());
        gtrace(str.c_str());
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
