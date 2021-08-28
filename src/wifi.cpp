//
// Created by sirosh on 28.08.2021.
//
#include <ESP8266WiFi.h>
#include "wifi.h"
#include "settings.h"
#include "config.h"

void initWifi() {
    WiFi.mode(WIFI_AP_STA);

    String *coConf = readConf("/co.conf");//todo ПОПРАВЬ БЛЯТЬ
    if (!coConf[0].equals("") && !coConf[1].equals("")) {
        WiFi.begin(coConf[0].c_str(), coConf[1].c_str());
        Serial.print("c_ssid:");
        Serial.println(coConf[0].c_str());
        Serial.print("c_pass:");
        Serial.println(coConf[1].c_str());

    } else {
        WiFi.begin(defaultClientSSID, defaultClientPass);
        Serial.print("c_ssid: (def):");
        Serial.println(defaultClientSSID);
        Serial.print("c_pass: (def)");
        Serial.println(defaultClientPass);
    }


    String *apConf = readConf("/ap.conf");

    if (!apConf[0].equals("") && !apConf[1].equals("")) {
        WiFi.softAP(apConf[0].c_str(), apConf[1].c_str());
        Serial.print("a_ssid:");
        Serial.println(apConf[0].c_str());
        Serial.print("a_pass:");
        Serial.println(apConf[1].c_str());
    } else {
        WiFi.softAP(defaultAPPass, defaultAPPass);
        Serial.print("a_ssid: (def)");
        Serial.println(defaultAPPass);
        Serial.print("a_pass: (def)");
        Serial.println(defaultAPPass);
    }

}