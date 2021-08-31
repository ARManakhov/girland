//
// Created by sirosh on 29.08.2021.
//

#ifndef GIRLAND_WIFICONNECTIONCONFIG_H
#define GIRLAND_WIFICONNECTIONCONFIG_H

#include <ArduinoJson.h>
#include <vector>
#include "WifiConnection.h"

class WifiConnectionConfig {
public:
    WifiConnectionConfig();

    WifiConnectionConfig(JsonObject object);

    const WifiConnection &getAccessPoint() const;

    void setAccessPoint(const WifiConnection &accessPoint);

    const std::vector<WifiConnection> &getConnections() const;

    void setConnections(const std::vector<WifiConnection> &connections);

    bool isUseAccessPoint() const;

    void setUseAccessPoint(bool useAccessPoint);

private:
    bool useAccessPoint{};
    WifiConnection accessPoint;
    std::vector<WifiConnection> connections;
};


#endif //GIRLAND_WIFICONNECTIONCONFIG_H
