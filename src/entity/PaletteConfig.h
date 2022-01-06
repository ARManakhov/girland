//
// Created by sirosh on 18.12.2021.
//

#ifndef SMART_CAT_PALETTECONFIG_H
#define SMART_CAT_PALETTECONFIG_H

#include <vector>
#include "internal/RgbColor.h"
#include <ArduinoJson.h>

class PaletteConfig {
public:
    PaletteConfig();

    PaletteConfig(JsonVariant json);

    const String &getName() const;

    std::vector<RgbColor> *getColors() const;

    BasicJsonDocument<ArduinoJson6185_91::DefaultAllocator> serialize();

private:
    String name;
    std::vector<RgbColor> *colors;
};


#endif //SMART_CAT_PALETTECONFIG_H
