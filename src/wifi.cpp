//
// Created by sirosh on 28.08.2021.
//
#include <ESP8266WiFi.h>
#include "wifi.h"
#include "config.h"

void initWifi() {
    WiFi.mode(WIFI_AP_STA);
    initWifi(readWifiConf());
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
    int resCount = WiFi.scanNetworks(); //todo optimize that crap
    if (resCount != 0) {
        for (const WifiConnection &connection: config->getConnections()) {
            for (int i = 0; i < resCount; ++i) {
                if (WiFi.SSID(i).equals(connection.getSsid())) {
                    WiFi.begin(connection.getSsid(), connection.getPassword());
                }
            }
        }
    }
    WiFi.scanComplete();
}