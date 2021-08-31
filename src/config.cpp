//
// Created by sirosh on 28.08.2021.
//
#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "config.h"
#include "entity/WifiConnectionConfig.h"
#include "settings.h"

WifiConnectionConfig *readWifiConf() {
    File file = LittleFS.open(WIFI_CONF_FILE, "r");
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        Serial.print(F("Failed to read wifi config "));
        Serial.println(error.c_str());
        return new WifiConnectionConfig();
    } else {
        return new WifiConnectionConfig(doc.as<JsonObject>());
    }
}

bool saveConfig(JsonObject object) {
    File file = LittleFS.open(WIFI_CONF_FILE, "w");
    if (serializeJson(object, file)) {
        Serial.println(F("Failed to write wifi config"));
        return false;
    }
    return true;
}