//
// Created by sirosh on 18.12.2021.
//

#include "PaletteConfig.h"

PaletteConfig::PaletteConfig(JsonVariant json) {
    colors = new std::vector<RgbColor>();
    if (json.containsKey("name") && json["name"].is<String>()) {
        name = json["name"].as<String>();
    } else {
        name = "";
    }
    if (json.containsKey("colors")) {
        JsonArray colorsJson = json.getMember("colors").as<JsonArray>();
        for (JsonVariant color: colorsJson) {
            if (color.containsKey("r") && color.containsKey("g") && color.containsKey("b")) {
                colors->emplace_back(color["r"], color["g"], color["b"]);
            }
        }
    }
}

const String &PaletteConfig::getName() const {
    return name;
}

std::vector<RgbColor> *PaletteConfig::getColors() const {
    return colors;
}

PaletteConfig::PaletteConfig() : name("error") {
    colors = new std::vector<RgbColor>();
    colors->emplace_back(RgbColor(255, 0, 0));
}

BasicJsonDocument<ArduinoJson6185_91::DefaultAllocator> PaletteConfig::serialize() {
    auto jsonDoc = DynamicJsonDocument(8192);
    JsonObject json = jsonDoc.to<JsonObject>();
    json["name"] = name;
    JsonArray colorsJson = jsonDoc.createNestedArray("colors");
    for (auto &color: *colors) {
        JsonObject colorJonObject = colorsJson.createNestedObject();
        colorJonObject["r"] = color.R;
        colorJonObject["g"] = color.G;
        colorJonObject["b"] = color.B;
    }
    return jsonDoc;
}