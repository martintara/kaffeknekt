// WebSocketClient.cpp
#include "websocketclient.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include <QDebug>

#include <cstring>  // for std::strcpy()

#include <QCoreApplication> // for Qt’s event loop + qDebug()

#define SOCKET_PATH "/tmp/socket"

WebSocketClient::WebSocketClient(QObject* parent)
    : QThread(parent)
{}

void WebSocketClient::run() {
    // 1) Open UNIX-domain socket
    int qtSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    //if (qtSocket < 0) return;  // error...

    if (qtSocket < 0) {
            qWarning() << "WebSocketClient: socket() failed";
            return;
         }
    qDebug() << "WebSocketClient: attempting connect() to" << SOCKET_PATH;

     // 2) Set up address struct
    struct sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strcpy(addr.sun_path, SOCKET_PATH);
    if (::connect(qtSocket, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(qtSocket);
        return;
    }
    qDebug() << "WebSocketClient: connected successfully";

    // 4) Read loop
    qDebug() << "WebSocketClient: entering recv() loop";

    char buffer[1024];
    std::string rest;

    /*
    while (true) {
        ssize_t bytesRead = recv(qtSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0)
            break;
            qDebug() << "WebSocketClient: recv() returned" << rec << "; exiting loop";
            break;
        }
        rest.append(buffer, rec);
        size_t pos;
        while ((pos = rest.find('\n')) != std::string::npos) {
            auto line = rest.substr(0, pos);
            rest.erase(0, pos+1);
            try {
                auto jsonData = nlohmann::json::parse(line);
                QString flag = QString::fromStdString(jsonData["flag"]);
                double pressure    = jsonData["pressure"];
                double temperature = jsonData["temperature"];
                emit dataReceived(pressure, temperature, flag);*/
            //} catch (...) { /* ignore parse errors */ }
        //}
    //}
    while (true) {
        // Read into buffer and store the number of bytes read
        ssize_t bytesRead = ::recv(qtSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            // Log and break out on error or EOF
            qDebug() << "WebSocketClient: recv() returned" << bytesRead << "; exiting loop";
            break;
        }

        // Append exactly the bytes we just read
        rest.append(buffer, static_cast<size_t>(bytesRead));

        // Process complete lines ending in '\n'
        size_t pos;
        while ((pos = rest.find('\n')) != std::string::npos) {
            auto line = rest.substr(0, pos);
            rest.erase(0, pos + 1);
            try {
                auto jsonData = nlohmann::json::parse(line);
                QString flag       = QString::fromStdString(jsonData["flag"]);
                double pressure    = jsonData["pressure"];
                double temperature = jsonData["temperature"];
                emit dataReceived(pressure, temperature, flag);
            } catch (...) {
                // ignore parse errors
            }
        }
    }


    // 5) Clean up
    close(qtSocket);
    qDebug() << "WebSocketClient: socket closed, thread ending";
}
