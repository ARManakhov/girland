#ifndef AsyncElegantOTA_h
#define AsyncElegantOTA_h

#include "FS.h"
#include "ESPAsyncWebServer.h"
#include "espUtils.h"

void registerOtaUpdate(AsyncWebServer *server) {
    server->on("/update", HTTP_POST, [&](AsyncWebServerRequest *request) {
                   AsyncWebServerResponse *response = request->beginResponse((Update.hasError()) ? 500 : 200, "text/plain",
                                                                             (Update.hasError()) ? "FAIL" : "OK");
                   response->addHeader("Connection", "close");
                   response->addHeader("Access-Control-Allow-Origin", "*");
                   request->send(response);
                   restart();
               },
               [&](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len,
                   bool final) {
                   //Upload handler chunks in data
                   if (!index) {
                       if (!request->hasParam("MD5", true)) {
                           return request->send(400, "text/plain", "MD5 parameter missing");
                       }

                       if (!Update.setMD5(request->getParam("MD5", true)->value().c_str())) {
                           return request->send(400, "text/plain", "MD5 parameter invalid");
                       }

                       int cmd = (filename == "filesystem") ? U_FS : U_FLASH;
                       Update.runAsync(true);
                       size_t fsSize = ((size_t) &_FS_end - (size_t) &_FS_start);
                       uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
                       if (!Update.begin((cmd == U_FS) ? fsSize : maxSketchSpace,
                                         cmd)) { // Start with max available size
                           Update.printError(Serial);
                           return request->send(400, "text/plain", "OTA could not begin");
                       }
                   }

                   // Write chunked data to the free sketch space
                   if (len) {
                       if (Update.write(data, len) != len) {
                           return request->send(400, "text/plain", "OTA could not begin");
                       }
                   }

                   if (final) { // if the final flag is set then this is the last frame of data
                       if (!Update.end(true)) { //true to set the size to the current progress
                           Update.printError(Serial);
                           return request->send(400, "text/plain", "Could not end OTA");
                       }
                   } else {
                       return;
                   }
               });
}

#endif
