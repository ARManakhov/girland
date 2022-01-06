//
// Created by sirosh on 18.12.2021.
//

#include "LedConfig.h"

LedConfig::LedConfig(JsonObject object) {
    mode = 0;
    brightness = 255;
    update(object);
    if (paletteList == nullptr || paletteList->empty()) {
        paletteList = new std::vector<PaletteConfig *>();
        paletteList->emplace_back(new PaletteConfig());
        Serial.println(paletteList->size());
    }
}

uint8_t LedConfig::getBrightness() const {
    return brightness;
}

uint8_t LedConfig::getMode() const {
    return mode;
}

const std::vector<PaletteConfig *> *LedConfig::getPaletteList() const {
    return paletteList;
}

LedConfig::LedConfig() : brightness(0), mode(0) {
    paletteList = new std::vector<PaletteConfig *>();
}

BasicJsonDocument<ArduinoJson6185_91::DefaultAllocator> LedConfig::serialize() {
    auto jsonDoc = DynamicJsonDocument(512);
    JsonObject jsonObject = jsonDoc.to<JsonObject>();
    jsonObject["mode"] = mode;
    jsonObject["brightness"] = brightness;
    JsonArray paletteListJson = jsonDoc.createNestedArray("paletteList");
    for (auto &p: *paletteList) {
        paletteListJson.add(p->serialize());
    }
    return jsonDoc;
}

void LedConfig::update(JsonObject object) {
    if (object.containsKey("mode")) {
        mode = object["mode"];
    }
    if (object.containsKey("brightness")) {
        brightness = object["brightness"];
    }
    if (object.containsKey("paletteList")) {
        JsonArray palettes = object.getMember("paletteList").as<JsonArray>();
        if (palettes.size() != 0) {
            paletteList = new std::vector<PaletteConfig *>();
            for (JsonVariant palette: palettes) {
                if (palette.containsKey("name") && palette.containsKey("colors")) {
                    Serial.println();
                    paletteList->emplace_back(new PaletteConfig(palette));
                }
            }
        }
    }
}
