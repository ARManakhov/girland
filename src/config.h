//
// Created by sirosh on 28.08.2021.
//

#include "entity/WifiConnectionConfig.h"

#ifndef GIRLAND_CONFIG_H
#define GIRLAND_CONFIG_H


WifiConnectionConfig *readWifiConf();

bool saveConfig(JsonObject object);

#endif //GIRLAND_CONFIG_H
