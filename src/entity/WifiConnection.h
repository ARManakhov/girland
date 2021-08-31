//
// Created by sirosh on 29.08.2021.
//

#ifndef GIRLAND_WIFICONNECTION_H
#define GIRLAND_WIFICONNECTION_H


#include <WString.h>

class WifiConnection {

private:
    String ssid;
    String password;
    int priority;
public:
    int getPriority() const;

    const String &getSsid() const;

    const String &getPassword() const;

    WifiConnection(const String &ssid, const String &password, int priority);

    WifiConnection(const String &ssid, const String &password);

};


#endif //GIRLAND_WIFICONNECTION_H
