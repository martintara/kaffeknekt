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
                qDebug() << "WebSocketClient received line:" << QString::fromStdString(line);

                if (!jsonData.contains("pressure") || !jsonData.contains("temperature") || !jsonData.contains("flag")) {
                    qWarning() << "Missing one or more expected fields in JSON:" << QString::fromStdString(line);
                    continue;
                }

                //QString flag       = QString::fromStdString(jsonData["flag"]);
                double pressure    = jsonData["pressure"];
                double temperature = jsonData["temperature"];

                QString flag = QString::fromStdString(jsonData["flag"].dump());
                flag.remove('\"');  // remove quotes if it's a string (e.g., "1")


                qDebug() << "Emitting dataReceived with:" << pressure << temperature << flag;

                emit dataReceived(pressure, temperature, flag);
            } catch (const std::exception& e) {
                qWarning() << "JSON parse error:" << e.what() << "for input:" << QString::fromStdString(line);
            }

            /*
            try {
                auto jsonData = nlohmann::json::parse(line);
                QString flag       = QString::fromStdString(jsonData["flag"]);
                double pressure    = jsonData["pressure"];
                double temperature = jsonData["temperature"];
                emit dataReceived(pressure, temperature, flag);
            } catch (...) {
                // ignore parse errors
            }*/
        }
    }


    // 5) Clean up
    close(qtSocket);
    qDebug() << "WebSocketClient: socket closed, thread ending";
}
