#include "src/Services/Includes/WifiDriver.hpp"
#include <Arduino.h>
#include "src/Includes/GlobalData.hpp"
#include "src/Guards/Includes/LockGuard.hpp"

WifiDriver::~WifiDriver() {
    // Disconnect from WiFi
    WiFi.disconnect();
}

void WifiDriver::connect() {
    // Connect to WiFi
    // Start receiving data
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }
    Serial.println("WiFi connected\n");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());

    sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    struct sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(port_);
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sock_, (struct sockaddr *)&localAddr, sizeof(localAddr));
    Serial.printf("\nListening on UDP port %d\n", port_);
}

void WifiDriver::receiveUDP() {
    while(1) {
        struct sockaddr_in remoteAddr;
        socklen_t addrLen = sizeof(remoteAddr);
        int len = recvfrom(sock_, 
                            &buffer_, 
                            sizeof(buffer_), 
                            0, 
                            (struct sockaddr *)&remoteAddr, 
                            &addrLen);
        if (len > 0) {
            // Process received data
            {
                {   
                    LockGuard<Mutex> lock(serialMutex);
                    Serial.write(getBuffer());
                    Serial.println();
                }
            }
        }
    }
}

void WifiDriver::sendUDP(
    const uint8_t& data, const auto& ip, const uint16_t port) {
     int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    struct sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(port);
    remoteAddr.sin_addr.s_addr = inet_addr(ip);
    sendto(sock, &data, sizeof(data), 0, (struct sockaddr *)&remoteAddr, sizeof(remoteAddr));
    close(sock);
}