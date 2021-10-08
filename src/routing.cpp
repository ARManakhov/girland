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
#include "palette.h"
#include "settings.h"
#include "update.h"

int brightness = 128;
int mode = 0;

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void ledGetHandler(AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument json(1024);
    json["brightness"] = brightness;
    json["mode"] = mode;
    serializeJson(json, *response);
    request->send(response);
}

void serverRoutingSetup() {
    //Route to load index.html
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/index.html", "text/html");
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

    AsyncCallbackJsonWebHandler *wifiUpdateHandler =
            new AsyncCallbackJsonWebHandler("/conf/wifi.json", [](AsyncWebServerRequest *request, JsonVariant &json) {
                initWifi(json.as<JsonObject>(), true);
            });
    wifiUpdateHandler->setMethod(HTTP_POST);
    server.addHandler(wifiUpdateHandler);

    AsyncCallbackJsonWebHandler *ledUpdateHandler =
            new AsyncCallbackJsonWebHandler("/conf/led.json", [](AsyncWebServerRequest *request, JsonVariant &json) {
                JsonObject object = json.as<JsonObject>();
                if (!object.containsKey("brightness")) {
                    request->send(400, "application/json", "brightness is not present");
                    return;
                }
                if (!object.containsKey("mode")) {
                    request->send(400, "application/json", "mode is not present");
                    return;
                }
                int inBrightness = object["brightness"];
                if (inBrightness > 255 || inBrightness < 0) {
                    request->send(400, "application/json", "brightness out of range");
                    return;
                }
                int inMode = object["mode"];
                if (inMode > 16 || inMode < 0) {
                    request->send(400, "application/json", "mode out of range");
                    return;
                }
                brightness = inBrightness;
                mode = inMode;
                setPallete(mode);
                FastLED.setBrightness(brightness);
                ledGetHandler(request);
            });
    wifiUpdateHandler->setMethod(HTTP_POST);
    server.addHandler(ledUpdateHandler);

    server.on("/conf/led.json", HTTP_GET, ledGetHandler);

    server.on("/upd/code", HTTP_POST, [](AsyncWebServerRequest *request) {}, httpProcessCodeUpdate);

    server.on("/upd/files", HTTP_POST, [](AsyncWebServerRequest *request) {}, httpProcessFSUpdate);

    server.onNotFound(notFound);
}

void serverInit() {
    serverRoutingSetup();
    server.begin();
}
