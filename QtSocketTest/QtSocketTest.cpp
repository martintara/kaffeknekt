#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <nlohmann/json.hpp>

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

    char buffer[1024];
    ssize_t bytesRead = recv(qtSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0){
        buffer[byttesRead] = '\0';
        try {
            nlohmann::json jsonData = nlohmann::json::parse(buffer);
            std::cout << "Received: " << jsonData.dump(4) << std::endl;
        } catch (const nlohmann::json::parse_error& e) {
            std::cerr << "JSON Parse Error: " << e.what() << std::endl;
        }
    }
    else if {bytesRead == 0){
        std::cout << "Connection closed by server." << std::endl;
    }
    else {
        perror("Receive Error");
    }

    close(qtSocket);
    unlink(SOCKET_PATH);
    return 0;

}