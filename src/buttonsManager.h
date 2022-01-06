//
// Created by sirosh on 12.12.2021.
//

#ifndef SMART_CAT_BUTTONSMANAGER_H
#define SMART_CAT_BUTTONSMANAGER_H

#include "settings.h"
#include "entity/Button.h"
#include "ledManager.h"


Button *buttons[] = {
        new Button(BUTTON_L_PIN, []() {
            LedConfig *ledConf = getLedConfig();
            int newBrightness = ledConf->getBrightness() + 17;
            if (newBrightness > 255) {
                newBrightness = 0;
            }
            updateBrightness(newBrightness);
        }),
        new Button(BUTTON_R_PIN, []() {
            LedConfig *ledConf = getLedConfig();
            int newMode = ledConf->getMode() + 1;
            if (ledConf->getPaletteList()->size() < newMode) {
                newMode = 0;
            }
            updateMode(newMode);
        })};

void buttonInit() {
    for (auto &button: buttons) {
        pinMode(button->getPin(), INPUT_PULLUP);
        attachInterruptArg(digitalPinToInterrupt(button->getPin()), Button::on_button_change, button, CHANGE);
    }
}

#endif //SMART_CAT_BUTTONSMANAGER_H
