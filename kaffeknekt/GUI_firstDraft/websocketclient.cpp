// WebSocketClient.cpp
#include "websocketclient.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include <QDebug>

#define SOCKET_PATH "/tmp/socket"

WebSocketClient::WebSocketClient(QObject* parent)
    : QThread(parent)
{}

void WebSocketClient::run() {
    int qtSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (qtSocket < 0) return;  // error...
    struct sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strcpy(addr.sun_path, SOCKET_PATH);
    if (connect(qtSocket, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(qtSocket);
        return;
    }

    char buffer[1024];
    std::string rest;
    while (true) {
        ssize_t rec = recv(qtSocket, buffer, sizeof(buffer), 0);
        if (rec <= 0) break;
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
                emit dataReceived(pressure, temperature, flag);
            } catch (...) { /* ignore parse errors */ }
        }
    }
    close(qtSocket);
}
