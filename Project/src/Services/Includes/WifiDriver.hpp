#pragma once
#include <stdint.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define WIFI_SSID     "Vishnu's S23"
#define WIFI_PASSWORD "8971741307"

class WifiDriver final {
    public:
    WifiDriver() = default;
    ~WifiDriver(); // Disconnect
    void connect(); // Connect to WiFi
    void receiveUDP();
    uint8_t getBuffer() { return buffer_; }
    void sendUDP(const uint8_t& data, 
        const auto& ip, const uint16_t port);
    
    private:
    uint16_t port_ = 5000;
    WiFiUDP udp_;
    int sock_;
    uint8_t buffer_;
};