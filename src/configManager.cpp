//
// Created by sirosh on 28.08.2021.
//
#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "configManager.h"
#include "entity/WifiConnectionConfig.h"
#include "settings.h"
#include "routing.h"

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

bool saveWifiConfig(JsonObject object) {
    File file = LittleFS.open(WIFI_CONF_FILE, "w");
    serializeJson(object, file);
    onWifiUpdate();
    return true;
}

LedConfig *readLedConf() {
    File file = LittleFS.open(LED_CONF_FILE, "r");
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        Serial.print(F("Failed to read led config "));
        Serial.println(error.c_str());
        return new LedConfig();
    } else {
        return new LedConfig(doc.as<JsonObject>());
    }
}

bool saveLedConfig(LedConfig *object) {
    File file = LittleFS.open(LED_CONF_FILE, "w");
    serializeJson(object->serialize(), file);
    onLedUpdate();
    return true;
}
