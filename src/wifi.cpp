//
// Created by sirosh on 28.08.2021.
//
#include <ESP8266WiFi.h>
#include "wifi.h"
#include "configManager.h"

void initWifi() {
    WifiConnectionConfig *config = readWifiConf();
    initWifi(config);
}


boolean initWifi(JsonObject object, bool save) {
    bool saved = false;
    if (save) {
        saved = saveWifiConfig(object);
    }
    initWifi(new WifiConnectionConfig(object));
    return saved;
}

void initWifi(WifiConnectionConfig *config) {
    if (config->isUseAccessPoint() || config->getConnections().empty()) {
        WiFi.mode(WIFI_AP_STA);
        WiFi.softAP(config->getAccessPoint().getSsid(), config->getAccessPoint().getPassword());
    } else {
        WiFi.mode(WIFI_STA);
    }
    int8_t resCount = WiFi.scanNetworks(); //todo optimize that crap
    if (resCount != 0) {
        for (const WifiConnection &connection: config->getConnections()) {
            WiFi.begin(connection.getSsid(), connection.getPassword());
        }
    }
    WiFi.scanComplete();
}