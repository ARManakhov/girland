//
// Created by sirosh on 28.08.2021.
//

#include "entity/WifiConnectionConfig.h"
#include "entity/LedConfig.h"

#ifndef GIRLAND_CONFIG_H
#define GIRLAND_CONFIG_H


WifiConnectionConfig *readWifiConf();

bool saveWifiConfig(JsonObject object);

LedConfig *readLedConf();

bool saveLedConfig(LedConfig *object);

#endif //GIRLAND_CONFIG_H
