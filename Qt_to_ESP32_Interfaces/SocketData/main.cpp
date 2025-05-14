#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>
#include <nlohmann/json.hpp>

#define SOCKET_PATH "/tmp/socket"

///@brief UNIX domain socket client.
///Connects to a UNIX domain socket as a client and receives data. 
int main(){
    int qtSocket; ///Socket descriptor
    struct sockaddr_un addr; ///Socket address
    bool status = true; ///Bool that can be used to stop sending of data
    qtSocket = socket(AF_UNIX, SOCK_STREAM, 0); ///Create socket
    ///@if Checks if socket was created
    if (qtSocket < 0){
        perror("Socket Error");
        return 1;
    }
    addr.sun_family = AF_UNIX; ///Sets address to UNIX
    std::strcpy(addr.sun_path, SOCKET_PATH); ///Sets address to file path
    ///@if Checks if address was set
    if (connect(qtSocket,(struct sockaddr*)&addr, sizeof(addr)) == -1){
        perror("Connection Error");
        close(qtSocket);
        return 1;
    }

    char buffer[1024]; ///Buffer for stored data
    std::string rest; ///String for remainder of data
    ///@brief While loop that receives data from socket.
    ///
    while(status){
        ssize_t dataRecived = recv(qtSocket, buffer, sizeof(buffer), 0);
        if (dataRecived <= 0) break;
        rest.append(buffer, dataRecived);

        size_t pos;
        while ((pos = rest.find('\n')) != std::string::npos) {
            std::string data = rest.substr(0, pos);
            rest.erase(0, pos + 1);
            if (nlohmann::json::accept(data)) {
                nlohmann::json jsonData = nlohmann::json::parse(data);
                std::string flag = jsonData["flag"];
                float pressure = jsonData["pressure"];
                float temperature = jsonData["temperature"];
                std::cout << "Received JSON: " << jsonData.dump(4) << std::endl;
            } else {
                std::cerr << "Invalid JSON data: " << data << std::endl;
            }

        }

    }
    close(qtSocket);
    return 0;

}
