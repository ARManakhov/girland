//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>
#include <settings.h>
#include <LittleFS.h>

boolean b1ps = 1;
boolean b2ps = 1;
boolean b3ps = 1;
boolean b4ps = 1;

unsigned long b1pt = 0;
unsigned long b2pt = 0;
unsigned long b3pt = 0;
unsigned long b4pt = 0;
int idelta = 25;

CRGB leds[LED_NUM];

CRGBPalette16 currentPalette;
TBlendType currentBlending;

extern const TProgmemPalette16 myRedWhiteBluePalette_p
PROGMEM;


AsyncWebServer server(80);


int brightness = 120;
int mode = 0;

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette() {
    for (int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV(random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette() {
    // 'black out' all 16 palette entries...
    fill_solid(currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette() {
    CRGB purple = CHSV(HUE_PURPLE, 255, 255);
    CRGB green = CHSV(HUE_GREEN, 255, 255);
    CRGB black = CRGB::Black;

    currentPalette = CRGBPalette16(
            green, green, black, black,
            purple, purple, black, black,
            green, green, black, black,
            purple, purple, black, black);
}


const TProgmemPalette16 myRGYB_p
PROGMEM =
        {
                CRGB::Red,
                CRGB::Green,
                CRGB::Yellow,
                CRGB::Blue,

                CRGB::Red,
                CRGB::Green,
                CRGB::Yellow,
                CRGB::Blue,

                CRGB::Red,
                CRGB::Green,
                CRGB::Yellow,
                CRGB::Blue,

                CRGB::Red,
                CRGB::Green,
                CRGB::Yellow,
                CRGB::Blue
        };
// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p
PROGMEM =
        {
                CRGB::Red,
                CRGB::Gray, // 'white' is too bright compared to red and blue
                CRGB::Blue,
                CRGB::Black,

                CRGB::Red,
                CRGB::Gray,
                CRGB::Blue,
                CRGB::Black,

                CRGB::Red,
                CRGB::Red,
                CRGB::Gray,
                CRGB::Gray,
                CRGB::Blue,
                CRGB::Blue,
                CRGB::Black,
                CRGB::Black
        };



// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void setPallete(int secondHand) {
    if (secondHand == 0) {
        currentPalette = RainbowColors_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 1) {
        currentPalette = RainbowStripeColors_p;
        currentBlending = NOBLEND;
    }
    if (secondHand == 2) {
        currentPalette = RainbowStripeColors_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 3) {
        SetupPurpleAndGreenPalette();
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 4) {
        SetupTotallyRandomPalette();
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 5) {
        SetupBlackAndWhiteStripedPalette();
        currentBlending = NOBLEND;
    }
    if (secondHand == 6) {
        SetupBlackAndWhiteStripedPalette();
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 7) {
        currentPalette = CloudColors_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 8) {
        currentPalette = PartyColors_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 9) {
        currentPalette = myRedWhiteBluePalette_p;
        currentBlending = NOBLEND;
    }
    if (secondHand == 10) {
        currentPalette = myRedWhiteBluePalette_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 11) {
        currentPalette = myRGYB_p;
        currentBlending = NOBLEND;
    }
    if (secondHand == 12) {
        currentPalette = myRGYB_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 13) {
        currentPalette = OceanColors_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 14) {
        currentPalette = LavaColors_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 15) {
        currentPalette = ForestColors_p;
        currentBlending = LINEARBLEND;
    }

}


void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


void handleButton() {
    unsigned long curentTime = millis();
    if (b1ps != digitalRead(b1gpio) && b1pt + idelta < curentTime) {
        b1ps = digitalRead(b1gpio);
        if (!digitalRead(b1gpio)) {
            b1pt = curentTime;
            if (brightness != 250)
                brightness = brightness + 10;
            FastLED.setBrightness(brightness);
        }
    }

    if (b2ps != digitalRead(b2gpio) && b2pt + idelta < curentTime) {
        b2ps = digitalRead(b2gpio);
        if (!digitalRead(b2gpio)) {
            b2pt = curentTime;
            if (brightness != 0)
                brightness = brightness - 10;
            FastLED.setBrightness(brightness);
        }
    }

    if (b3ps != digitalRead(b3gpio) && b3pt + idelta < curentTime) {
        b3ps = digitalRead(b3gpio);
        if (!digitalRead(b3gpio)) {
            b3pt = curentTime;
            if (mode != 15)
                mode++;
            setPallete(mode);
        }
    }

    if (b4ps != digitalRead(b4gpio) && b4pt + idelta < curentTime) {
        b4ps = digitalRead(b4gpio);
        if (!digitalRead(b4gpio)) {
            b4pt = curentTime;
            if (mode != 0)
                mode--;
            setPallete(mode);
        }
    }
}

String *readConf(String filename) {
    String *conf = new String[2];
    File file = LittleFS.open(filename, "r");
    if (!file) {
        Serial.println("Failed to open file for reading");
        conf[0] = "";
        conf[1] = "";
    } else {
        String name = file.readStringUntil('\n');
        String pass = file.readStringUntil('\n');
        conf[0] = name;
        conf[1] = pass;
    }
    return conf;
}


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

String indexProcessor(const String &var) {
    Serial.println(var);
    if (var == "MODE") {
        String modeStr = String(mode + 1);
        return modeStr;
    } else if (var == "BRIGHTNESS") {
        String brightnessStr = String(brightness / 10);
        return brightnessStr;
    }
}

void trySaveConf(String name, String pass, String filename) {
    File file = LittleFS.open(filename, "w");
    if (!file) {
        Serial.println("Error opening file for writing");
        return;
    }
    int bytesWritten = file.println(name);
    bytesWritten += file.println(pass);
    if (bytesWritten > 0) {
        Serial.print("saved ");
        Serial.print(filename);
        Serial.println(bytesWritten);

    } else {
        Serial.println("File write failed");
    }
    file.close();
}


void loop() {
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
    uint8_t brightness = 255;

    for (int i = 0; i < LED_NUM; i++) {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void updateWifiClientAuth(AsyncWebServerRequest *request);

void restartNow();

void updateWifiAPAuth(AsyncWebServerRequest *request);

void handleControls(AsyncWebServerRequest *request);

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
        size_t fsSize = ((size_t) & _FS_end - (size_t) & _FS_start);
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

    server.on("/b", HTTP_POST, [](AsyncWebServerRequest *request) {
        handleControls(request);
    });

    server.on("/updAP", HTTP_POST, [](AsyncWebServerRequest *request) {
        updateWifiAPAuth(request);
    });

    server.on("/updClient", HTTP_POST, [](AsyncWebServerRequest *request) {
        updateWifiClientAuth(request);
    });

    server.on("/updCode", HTTP_POST, [](AsyncWebServerRequest *request) {}, httpProcessCodeUpdate);

    server.on("/updFS", HTTP_POST, [](AsyncWebServerRequest *request) {}, httpProcessFSUpdate);

    server.onNotFound(notFound);
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

void updateWifiAPAuth(AsyncWebServerRequest *request) {
    if (request->hasParam("ssid_a", true) && request->hasParam("pass_a", true)) {
        String name = request->getParam("ssid_a", true)->value();
        String pass = request->getParam("pass_a", true)->value();
        if (pass.length() >= 8) {
            trySaveConf(name, pass, "/ap.conf");
            Serial.println("saving ap");
            initWifi();
        } else {
            Serial.println("err pass < 8 ");
        }

    } else {
        Serial.println("err saving ap");
    }
    request->send(LittleFS, "/settings.html", "text/html");
}

void setup() {


    Serial.begin(115200);

    if (!LittleFS.begin()) {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }


    if (LittleFS.begin()) {
        Serial.println("LittleFS Active");
        Serial.println();
    } else {
        Serial.println("Unable to activate LittleFS");
    }


    initWifi();

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_NUM).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(brightness);

    setPallete(mode);

    serverRoutingSetup();

    server.begin();

    pinMode(b1gpio, INPUT_PULLUP);
    pinMode(b2gpio, INPUT_PULLUP);
    pinMode(b3gpio, INPUT_PULLUP);
    pinMode(b4gpio, INPUT_PULLUP);

    while (true) {
//        if (needRestartNow) {
//            restartNow();
//        }
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* motion speed */

        FillLEDsFromPaletteColors(startIndex);
        handleButton();
        FastLED.show();
        FastLED.delay(1000 / UPDATES_PER_SECOND);
    }
}

void restartNow() {
    Serial.println("Restart");
    ESP.restart();
}


void updateWifiClientAuth(AsyncWebServerRequest *request) {
    if (request->hasParam("ssid_c", true) && request->hasParam("pass_c", true)) {
        String name = request->getParam("ssid_c", true)->value();
        String pass = request->getParam("pass_c", true)->value();
        if (pass.length() >= 8) {
            trySaveConf(name, pass, "/co.conf");
            Serial.println("saving conn");
            initWifi();
        } else {
            Serial.println("err pass < 8 ");
        }
    } else {
        Serial.println("err saving ap");
    }
    request->send(LittleFS, "/settings.html", "text/html");
}
