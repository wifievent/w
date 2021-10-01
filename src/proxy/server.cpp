#include "server.h"

//std::mutex _mutex;
//std::list<int> clnt_socks;

Server::Server()
{
    sock=socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(8080);
    if(bind(sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error");
    if(listen(sock, 5)==-1)
        error_handling("listen() error");
    return;
}

Server::Server(int port)
{
    sock=socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(port);
    if(bind(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error");
    if(listen(sock, 5)==-1)
        error_handling("listen() error");
    return;
}

Server::~Server()
{
    delete serv_thread;
    //is it right either join or detach?
}

void Server::start(Server* server)
{
    cout << "start!" << endl;
    int clnt_sock;
    while(1)
    {
        clnt_adr_sz=sizeof(clnt_adr);
        clnt_sock=accept(sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        _mutex.lock();
        clnt_socks.push_back(clnt_sock);
        _mutex.unlock();
        thread handle_clnt_thread(&Server::start_handle_clnt, server, clnt_sock);
        //Watch out here
        handle_clnt_thread.detach();
        printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
    }
}

void Server::start_handle_clnt(int clnt_sock)
{
    this->handle_clnt(clnt_sock);
    this->delete_clnt(clnt_sock);
}

void Server::delete_clnt(int clnt_sock)
{
    std::list<int>::iterator iter;
    _mutex.lock();
    for(iter = clnt_socks.begin(); iter != clnt_socks.end(); iter++)// remove disconnected client
    {
        if(clnt_sock==*iter)
        {
            clnt_socks.erase(iter);
            break;
        }
    }
    _mutex.unlock();
    close(clnt_sock);
    return;
}