//
// Created by sirosh on 28.08.2021.
//
#include <ESP8266WiFi.h>
#include "wifi.h"
#include "config.h"

void initWifi() {
    WiFi.mode(WIFI_AP_STA);
    WifiConnectionConfig *config = readWifiConf();
    Serial.print("using conf:");
    Serial.println(config->getAccessPoint().getSsid());
    Serial.println(config->getConnections().size());
    initWifi(config);
}


boolean initWifi(JsonObject object, bool save) {
    bool saved = false;
    if (save) {
        saved = saveConfig(object);
    }
    initWifi(new WifiConnectionConfig(object));
    return saved;
}

void initWifi(WifiConnectionConfig *config) {
    if (config->isUseAccessPoint() || config->getConnections().empty()) {
        WiFi.softAP(config->getAccessPoint().getSsid(), config->getAccessPoint().getPassword());
    }
    int8_t resCount = WiFi.scanNetworks(); //todo optimize that crap
    if (resCount != 0) {
        for (const WifiConnection &connection: config->getConnections()) {
            for (int8_t i = 0; i < resCount; ++i) {
                if (WiFi.SSID(i).equals(connection.getSsid())) {
                    WiFi.begin(connection.getSsid(), connection.getPassword());
                }
            }
        }
    }
    WiFi.scanComplete();
}