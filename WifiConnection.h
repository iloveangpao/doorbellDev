#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

#include <WiFi.h>

class WiFiConnection {
private:
    const char* ssid;
    const char* password;

public:
    WiFiConnection(const char* ssid, const char* password);

    void connect();
    void disconnect();
    IPAddress getIP();
    bool isConnected();
};

#endif /* WIFI_CONNECTION_H */
