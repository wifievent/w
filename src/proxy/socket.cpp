#include "socket.h"

Socket::~Socket()
{
    close(sock);
}

void Socket::error_handling(string msg)
{
    cout << msg << endl;
    exit(1);
}