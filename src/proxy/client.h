#pragma once
#include "stdafx.h"
#include "socket.h"

#define BUF_SIZE 100

using namespace std;

//client -> connect API(port IP), send function, recv function, close function
//no Inheritance. 22:00

/* usage
    Client client1;
    client1.sendbuf = ...;
    client1.send_data()
    client1.recv_data() //return to client1.recvbuf

    if you want, you can use send_thread to send various data...
    -> Is this need in here?
    -> see in doc/uml repo proxy version2
    
    thread * send_thread;
    send_thread = new thread(send_data)
    delete send_thread; 

*/

class Client : public Socket
{
    char sendbuf[BUF_SIZE];
    char recvbuf[BUF_SIZE];
    struct sockaddr_in serv_addr;

public:
    Client();
    Client(int port);
    Client(string ip, int port);
    ~Client();

public:
    void send_data();
    int recv_data();
    void setSendBuf(char* buf);
    char* getSendBuf();
    char* getRecvBuf();
//     void makeSendThread(); //function
//     void startRecvThread();
};
