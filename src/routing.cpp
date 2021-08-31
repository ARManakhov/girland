//
// Created by sirosh on 28.08.2021.
//

#include "routing.h"
#include <LittleFS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>
#include <AsyncJson.h>
#include "wifi.h"
#include "config.h"
#include "espUtils.h"
#include "palette.h"
#include "settings.h"

int brightness = 120;
int mode = 0;

AsyncWebServer server(80);

void httpProcessFSUpdate(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
                         size_t len, bool final);

void updateWifiAPAuth(AsyncWebServerRequest *request);

void httpProcessCodeUpdate(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
                           size_t len, bool final);

void updateWifiClientAuth(AsyncWebServerRequest *request);

//AsyncWebServer getServer();
void handleControls(AsyncWebServerRequest *request);

String indexProcessor(const String &var);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void serverRoutingSetup() {
    //Route to load index.html
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/index.html", String(), false, indexProcessor);
    });

    // Route to load milligram.css file
    server.on("/css/milligram.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/css/milligram.min.css", "text/css");
    });

    // Route to load mini-default.min.css file
    server.on("/css/main.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/css/main.css", "text/css");
    });

    // Route to load mini-default.min.css file
    server.on("/conf/wifi.json", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, WIFI_CONF_FILE, "plain/text");
    });

    server.on("/b", HTTP_POST, [](AsyncWebServerRequest *request) {
        handleControls(request);
    });


    AsyncCallbackJsonWebHandler *wifiUpdateHandler =
            new AsyncCallbackJsonWebHandler("/upd/wifi", [](AsyncWebServerRequest *request, JsonVariant &json) {
                initWifi(json.as<JsonObject>(), true);
            });
    wifiUpdateHandler->setMethod(HTTP_POST);
    server.addHandler(wifiUpdateHandler);

    server.on("/upd/code", HTTP_POST, [](AsyncWebServerRequest *request) {}, httpProcessCodeUpdate);

    server.on("/upd/files", HTTP_POST, [](AsyncWebServerRequest *request) {}, httpProcessFSUpdate);

    server.onNotFound(notFound);
}

void serverInit() {
    serverRoutingSetup();
    server.begin();
}

String indexProcessor(const String &var) {
    Serial.println(var);
    if (var == "MODE") {
        String modeStr = String(mode + 1);
        return modeStr;
    } else if (var == "BRIGHTNESS") {
        String brightnessStr = String(brightness / 10);
        return brightnessStr;
    }
    return "";
}

void handleControls(AsyncWebServerRequest *request) {
    bool ok = false;
    if (request->hasParam("t", true)) {
        String button = request->getParam("t", true)->value();
        if (button == "+") {
            if (brightness != 250) brightness = brightness + 10;
            FastLED.setBrightness(brightness);
            ok = true;
        }
        if (button == "-") {
            if (brightness != 0) brightness = brightness - 10;
            FastLED.setBrightness(brightness);
            ok = true;
        }
        if (button == ">") {
            if (mode != 15) mode++;
            setPallete(mode);
            ok = true;
        }
        if (button == "<") {
            if (mode != 0) mode--;
            setPallete(mode);
            ok = true;
        }
    }
    if (ok) {
        String reqStr = "{\"b\":\"" + String(brightness / 10) + "\", \"m\":\"" + String(1 + mode) + "\"}";
        request->send(200, "application/json", reqStr);
    } else {
        request->send(403, "application/json");
    }
}

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
    uint32_t free_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    if (!index) {
        Update.runAsync(true);
        size_t fsSize = ((size_t) &_FS_end - (size_t) &_FS_start);
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
