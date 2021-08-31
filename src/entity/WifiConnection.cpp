//
// Created by sirosh on 29.08.2021.
//

#include "WifiConnection.h"


WifiConnection::WifiConnection(const String &ssid, const String &password) : ssid(ssid), password(password) {}

const String &WifiConnection::getSsid() const {
    return ssid;
}

const String &WifiConnection::getPassword() const {
    return password;
}

WifiConnection::WifiConnection(const String &ssid, const String &password, int priority) : ssid(ssid),
                                                                                           password(password),
                                                                                           priority(priority) {}

int WifiConnection::getPriority() const {
    return priority;
}
