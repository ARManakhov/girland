//
// Created by sirosh on 28.08.2021.
//

#ifndef GIRLAND_ESPUTILS_H
#define GIRLAND_ESPUTILS_H

void restart() {
    Serial.println("Restart");
    yield();
    delay(1000);
    yield();
    ESP.restart();
}
#endif //GIRLAND_ESPUTILS_H
