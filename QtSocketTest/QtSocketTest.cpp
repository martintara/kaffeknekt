#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>
#include <nlohmann/json.hpp>

#define SOCKET_PATH "/tmp/socket"


int main(){
    int qtSocket;
    struct sockaddr_un addr;
    bool status = true;

    qtSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (qtSocket < 0){
        perror("Socket Error");
        std::cout << "Socket";
        status = false;
        return 1;
    }

    addr.sun_family = AF_UNIX;
    std::strcpy(addr.sun_path, SOCKET_PATH);
    if (connect(qtSocket,(struct sockaddr*)&addr, sizeof(addr) - 1) == -1){
        perror("Connection Error");
        std::cout << "Connection";
        close(qtSocket);
        status = false;
        return 1;
    }

    char buffer[1024];
    std::string rest;
    while(status){
        ssize_t dataRecived = recv(qtSocket, buffer, sizeof(buffer), 0);
        if (dataRecived <= 0) break;
        rest.append(buffer, dataRecived);

        size_t pos;
        while ((pos = rest.find('\n')) != std::string::npos) {
            std::string data = rest.substr(0, pos);

            

            rest.erase(0, pos + 1);
            try { 
                nlohmann::json jsonData = nlohmann::json::parse(data);
                std::cout << "Received Data: " << jsonData.dump(4) << std::endl;
            }   catch (const std::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                close(qtSocket);
            }
        
        }

    }
    close(qtSocket);
    return 0;

}