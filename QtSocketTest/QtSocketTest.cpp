#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

#define SOCKET_PATH "tmp/socket"

int main(){
    int qtSocket;
    struct sockaddr_un addr;
    bool status = true;

    qtSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (qtSocket < 0){
        perror("Socket Error");
        status = false;
        return 1;
    }

    addr.sun_family = AF_UNIX;
    std::strcpy(addr.sun_path, SOCKET_PATH);
    if (connect(qtSocket,(struct sockaddr*)&addr, sizeof(addr) - 1) == -1){
        perror("Connection Error");
        status = false;
        return 1;
    }

    
    while(status){
        //Code for reciving data
    }

    unlink(SOCKET_PATH);
    return 0;

}