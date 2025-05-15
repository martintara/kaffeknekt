//#include <QCoreApplication>
//#include <QDebug>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <nlohmann/json.hpp>

///@brief Path for the UNIX domain socket.
///This path is used to create the socket and connect to it.
#define SOCKET_PATH "/tmp/socket"

///@brief Creates a UNXI socket and connects it to the data handling server.
///Creates a UNIX domain socket, connects to the data handling server, and handles errors.
int createSocket(){
    int qtSocket;
    struct sockaddr_un addr;
    qtSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (qtSocket < 0){
        perror("Socket Error");
        return 1;
    }
    addr.sun_family = AF_UNIX;
    std::strcpy(addr.sun_path, SOCKET_PATH);
    if (connect(qtSocket,(struct sockaddr*)&addr, sizeof(addr)) == -1){
        perror("Connection Error");
        close(qtSocket);
        return 1;
    }
    return qtSocket;
}

///@brief Reads data from the connected socket.
///Reads data from the connected socket, adds the specified values to variables. 
void getData(int socket){
    char buffer[1024];
    std::string rest;
    bool status = true;
    while(status){
        ssize_t dataReceived = recv(socket, buffer, sizeof(buffer), 0);
        if (dataReceived <= 0) break;
        rest.append(buffer, dataReceived);

        size_t pos;
        while ((pos = rest.find('\n')) != std::string::npos) {
            std::string data = rest.substr(0, pos);
            rest.erase(0, pos + 1);
            if (nlohmann::json::accept(data)) {
                nlohmann::json jsonData = nlohmann::json::parse(data);
                std::string flag = jsonData["flag"];
                float pressure = jsonData["pressure"];
                float temperature = jsonData["temperature"];
                std::cout << "Received JSON: " << jsonData.dump(4) << std::endl; //Prints the received JSON (is not needed)
            } else {
                std::cout << "Invalid JSON data: " << data << std::endl;
                status = false;
                break;
            }

        }

    }
}

///@brief UNIX domain socket client.
///Connects to a UNIX domain socket using @ref createSocket() and receives data using @ref getData(). 
int main(){
    int newSocket = createSocket();
    getData(newSocket);
    close(newSocket);
    return 0;
}
