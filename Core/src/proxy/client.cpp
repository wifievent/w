#include "client.h"

Client::Client()
{
    sock=socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(8080);
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        error_handling("connect() error");
}

Client::Client(int port)
{
    sock=socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(port);
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        error_handling("connect() error");
}

Client::Client(string ip, int port)
{
    sock=socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(ip.c_str());
    serv_addr.sin_port=htons(port);
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        error_handling("connect() error");
}
//how to delete same code in init func?

void Client::send_data()
{
    write(sock, sendbuf, strlen(sendbuf));
    return;
}

int Client::recv_data()
{
    int str_len;
    for(int i = 0; i < 2; i++)
    {
        str_len = read(sock, recvbuf, sizeof(recvbuf));
        cout << "len: " << str_len << endl;
        if(str_len==-1)
        {
            error_handling("Can't read data");
            return 0;
        }
    }
    return 1;
}

void Client::setSendBuf(char* buf)
{
    memcpy(sendbuf, buf, BUF_SIZE);
}

char* Client::getSendBuf()
{
    return sendbuf;
}

char* Client::getRecvBuf()
{
    return recvbuf;
}