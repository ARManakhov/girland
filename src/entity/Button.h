//
// Created by sirosh on 12.12.2021.
//

#ifndef SMART_CAT_BUTTON_H
#define SMART_CAT_BUTTON_H

#include "c_types.h"
#include <Arduino.h>

#include <utility>

typedef std::function<void()> ButtonHandlerFunction;

class Button {
private:
    const int pin;
    const ButtonHandlerFunction function;
    volatile unsigned long press_time = 0;
public:

    static void IRAM_ATTR on_button_change(void *instance) {
        Button *button = (Button *) instance;
        if (digitalRead(button->getPin())) {
            if (button->getPressTime() + 100 < millis()) {
                button->handle();
            }
        } else {
            button->setPressTime(millis());
        }
    }

    const int getPin() const {
        return pin;
    }

    volatile unsigned long getPressTime() const {
        return press_time;
    }

    void setPressTime(volatile unsigned long pressTime) {
        press_time = pressTime;
    }

    void handle() {
        function();
    }

    Button(const int pin, const ButtonHandlerFunction &function) : pin(pin), function(function) {}
};


#endif //SMART_CAT_BUTTON_H
