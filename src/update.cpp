//
// Created by sirosh on 08.10.2021.
//
#include "update.h"

#include <LittleFS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "wifi.h"
#include "config.h"
#include "espUtils.h"

void httpProcessCodeUpdate(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
                           size_t len, bool final) {
    uint32_t free_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    if (!index) {
        Serial.println("process update");
        Update.runAsync(true);
        if (!Update.begin(free_space)) {
            Update.printError(Serial);
        }
    }

    if (Update.write(data, len) != len) {
        Update.printError(Serial);
    }

    request->redirect("/");

    if (final) {
        if (!Update.end(true)) {
            Update.printError(Serial);
        } else {
            Serial.println("Update complete");
            restartNow();
        }
    }
}

void httpProcessFSUpdate(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
                         size_t len, bool final) {
    if (!index) {
        Update.runAsync(true);
        uint32_t fsSize = (&_FS_end - &_FS_start);
        if (!Update.begin(fsSize, U_FS)) {
            Update.printError(Serial);
        }
    }

    if (Update.write(data, len) != len) {
        Update.printError(Serial);
    }

    request->redirect("/");

    if (final) {
        if (!Update.end(true)) {
            Update.printError(Serial);
        } else {
            Serial.println("Update complete");
            restartNow();
        }
    }
}