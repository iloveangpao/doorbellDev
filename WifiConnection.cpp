#include "WiFiConnection.h"
#include <Arduino.h> // Include if you're using Arduino functions

WiFiConnection::WiFiConnection(const char* ssid, const char* password) : ssid(ssid), password(password) {}

void WiFiConnection::connect() {
    WiFi.begin(ssid, password);

    Serial.println("\nConnecting to WiFi");

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
        if (++attempts > 20) { // Attempt to connect for 20 seconds
            Serial.println("\nConnection failed. Check credentials or network.");
        }
    }

    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void WiFiConnection::disconnect() {
    WiFi.disconnect();
    Serial.println("WiFi disconnected");
}

IPAddress WiFiConnection::getIP() {
    return WiFi.localIP();
}

bool WiFiConnection::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}
