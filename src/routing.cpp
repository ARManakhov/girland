//
// Created by sirosh on 28.08.2021.
//

#include "routing.h"
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include "wifi.h"
#include "ledManager.h"
#include "configManager.h"
#include "settings.h"
#include "OTAUpdate.h"

AsyncWebServer server(80);

void registerStaticContent(AsyncWebServer *server) {
    //Route to load index.html
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/index.html", "text/html");
    });

    // Route to load any css file
    server->on("/css/*", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, request->url(), "text/css");
    });

    // Route to load any js file
    server->on("/js/*", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, request->url(), "application/javascript");
    });

    server->onNotFound([](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/not_found.html", "text/html");
    });
}

void registerStatic(AsyncWebServer *server) {
    server->on("/conf/wifi.json", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, WIFI_CONF_FILE, "plain/text");
    });
    server->on("/conf/led.json", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, LED_CONF_FILE, "plain/text");
    });

    server->addHandler(
            new AsyncCallbackJsonWebHandler("/conf/wifi.json", [](AsyncWebServerRequest *request, JsonVariant &json) {
                initWifi(json.as<JsonObject>(), true);
                request->send(LittleFS, WIFI_CONF_FILE, "plain/text");
            }));

    server->addHandler(
            new AsyncCallbackJsonWebHandler("/conf/led.json", [](AsyncWebServerRequest *request, JsonVariant &json) {
                JsonObject object = json.as<JsonObject>();
                if (object.containsKey("brightness")) {
                    int inBrightness = object["brightness"];
                    if (inBrightness > 255 || inBrightness < 0) {
                        request->send(400, "application/json", "brightness out of range");
                        return;
                    }
                }
                if (object.containsKey("mode")) {
                    int inMode = object["mode"];
                    if (inMode > getLedConfig()->getPaletteList()->size() || inMode < 0) {
                        request->send(400, "application/json", "mode out of range");
                        return;
                    }
                }
                updateConfig(object);
                request->send(LittleFS, LED_CONF_FILE, "plain/text");
            }));
}

void serverInit() {
    registerStaticContent(&server);
    registerStatic(&server);
    registerOtaUpdate(&server);
    server.begin();
}
