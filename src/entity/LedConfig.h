//
// Created by sirosh on 18.12.2021.
//

#ifndef SMART_CAT_LEDCONFIG_H
#define SMART_CAT_LEDCONFIG_H

#include <vector>
#include "PaletteConfig.h"
#include <ArduinoJson.h>

class LedConfig {
private:

    uint8_t brightness;
    uint8_t mode;
public:
    void setBrightness(uint8_t brightness);

    void setMode(uint8_t mode);

private:
    std::vector<PaletteConfig *> *paletteList;
public:
    uint8_t getBrightness() const;

    void update(JsonObject object);

    uint8_t getMode() const;

    const std::vector<PaletteConfig *> *getPaletteList() const;

    LedConfig(JsonObject object);

    LedConfig();

    BasicJsonDocument<ArduinoJson6185_91::DefaultAllocator> serialize();
};


#endif //SMART_CAT_LEDCONFIG_H
