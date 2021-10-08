//
// Created by sirosh on 08.10.2021.
//
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>

#ifndef GIRLAND_UPDATE_H
#define GIRLAND_UPDATE_H

void httpProcessFSUpdate(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
                         size_t len, bool final);

void httpProcessCodeUpdate(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
                           size_t len, bool final);

#endif //GIRLAND_UPDATE_H
