//
// Created by sirosh on 12.12.2021.
//

#ifndef SMART_CAT_BUTTONSMANAGER_H
#define SMART_CAT_BUTTONSMANAGER_H
#include "settings.h"
#include "entity/Button.h"


Button *buttons[] = {
        new Button(BUTTON_L_PIN, []() {
            Serial.print("pressed");
        }),
        new Button(BUTTON_R_PIN, []() {
            Serial.print("pressed");
        })};

void buttonInit() {
    for (int i = 0; i < 2; i++) {
        pinMode(buttons[i]->getPin(), INPUT_PULLUP);
        attachInterruptArg(digitalPinToInterrupt(buttons[i]->getPin()), Button::on_button_change, buttons[i], CHANGE);
    }
}

#endif //SMART_CAT_BUTTONSMANAGER_H
