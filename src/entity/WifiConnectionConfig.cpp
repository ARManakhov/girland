//
// Created by sirosh on 29.08.2021.
//
#include <ArduinoJson.h>
#include "settings.h"
#include "WifiConnectionConfig.h"

const WifiConnection &WifiConnectionConfig::getAccessPoint() const {
    return accessPoint;
}

void WifiConnectionConfig::setAccessPoint(const WifiConnection &accessPoint) {
    WifiConnectionConfig::accessPoint = accessPoint;
}

const std::vector<WifiConnection> &WifiConnectionConfig::getConnections() const {
    return connections;
}

void WifiConnectionConfig::setConnections(const std::vector<WifiConnection> &connections) {
    WifiConnectionConfig::connections = connections;
}

WifiConnectionConfig::WifiConnectionConfig() : useAccessPoint(true),
                                               accessPoint(WifiConnection(DEFAULT_AP_SSID, DEFAULT_AP_PASSWORD)) {
}

bool WifiConnectionConfig::isUseAccessPoint() const {
    return useAccessPoint;
}

void WifiConnectionConfig::setUseAccessPoint(bool useAccessPoint) {
    WifiConnectionConfig::useAccessPoint = useAccessPoint;
}

WifiConnectionConfig::WifiConnectionConfig(JsonObject object) : useAccessPoint(true),
                                                                accessPoint(WifiConnection(
                                                                        DEFAULT_AP_SSID,
                                                                        DEFAULT_AP_PASSWORD)) {
    if (object["ap"] && object["ap"]["ssid"] && object["ap"]["pass"]) {
        setAccessPoint(WifiConnection(object["ap"]["ssid"], object["ap"]["pass"]));
    }
    if (object["apOn"]) {
        setUseAccessPoint(object["apOn"]);
    }

    if (object.containsKey("co")) {
        JsonArray connectionsArray = object.getMember("co").as<JsonArray>();
        for (JsonVariant connectionJson: connectionsArray) {
            if (connectionJson["ssid"] && connectionJson["pass"]) {
                connections.emplace_back(connectionJson["ssid"], connectionJson["pass"], connectionJson["p"]);
            }
        }
    }
}
