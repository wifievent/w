#pragma once
#include "stdafx.h"
#include "socket.h"

#define BUF_SIZE 100

// using namespace std;
// using std::thread;

class Server : public Socket
{
    unsigned int clnt_adr_sz;
    bool broadcast;
    bool echo;
    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;

protected:
    std::mutex _mutex; //protected
    std::list<int> clnt_socks; //protected

protected:
    thread* serv_thread;

public:
    Server();
    Server(int port);
    ~Server(); //Question

public:
    void start(Server* server);

protected:
    void delete_clnt(int clnt_sock);
    void start_handle_clnt(int clnt_sock);
    virtual void handle_clnt(int clnt_sock) {};
};
