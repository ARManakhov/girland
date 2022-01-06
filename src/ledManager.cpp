//
// Created by sirosh on 28.08.2021.
//


#include <Arduino.h>
#include <NeoPixelBus.h>
#include "settings.h"
#include "configManager.h"
#include "ledManager.h"

NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod> mainStrip(LED_NUM, LED_PIN);
static volatile size_t blendedIndex = 0;
static volatile float changeSpeed = 0.5; //color per second for one pixel
static volatile int updatesAdj = UPDATES_PER_SECOND / changeSpeed;
static LedConfig *ledConfig;

LedConfig *getLedConfig() {
    return ledConfig;
}

void ledShow() {
    std::vector<RgbColor> *currentPalette = ledConfig->getPaletteList()
            ->at(ledConfig->getPaletteList()->size() > ledConfig->getMode() ? ledConfig->getMode() : 0)->getColors();

    for (int i = 0; i < mainStrip.PixelCount(); ++i) {
        if (++blendedIndex >= currentPalette->size() * updatesAdj) {
            blendedIndex = 0;
        }
        size_t prevColorIndex = blendedIndex / updatesAdj;
        size_t nextColorIndex = prevColorIndex >= (currentPalette->size() - 1)
                                ? 0
                                : prevColorIndex + 1;
        mainStrip.SetPixelColor(i, RgbColor::LinearBlend(currentPalette->at(prevColorIndex),
                                                         currentPalette->at(nextColorIndex),
                                                         (float) (blendedIndex % updatesAdj) / (float) updatesAdj)
                .Dim(ledConfig->getBrightness()));
    }
    mainStrip.Show();
}

void updateConfig(JsonObject newLedConfig) {
    ledConfig->update(newLedConfig);
    saveLedConfig(ledConfig);
}

void updateMode(int mode) {
    ledConfig->setMode(mode);
    saveLedConfig(ledConfig);
}

void updateBrightness(int brightness) {
    ledConfig->setBrightness(brightness);
    saveLedConfig(ledConfig);
}

void initLed() {
    ledConfig = readLedConf();
    saveLedConfig(ledConfig);
    mainStrip.Begin();
    mainStrip.Show();
}