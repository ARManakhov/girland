//
// Created by sirosh on 28.08.2021.
//
#include "entity/WifiConnectionConfig.h"

#ifndef GIRLAND_WIFI_H
#define GIRLAND_WIFI_H


void initWifi();

boolean initWifi(JsonObject object, bool save);

void initWifi(WifiConnectionConfig *config);


#endif //GIRLAND_WIFI_H
