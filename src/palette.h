//
// Created by sirosh on 28.08.2021.
//

#ifndef GIRLAND_PALETTE_H
#define GIRLAND_PALETTE_H

extern CRGBPalette16 currentPalette;
extern TBlendType currentBlending;

void setPallete(int secondHand);

void FillLEDsFromPaletteColors(uint8_t colorIndex);
void initFastLed();

#endif //GIRLAND_PALETTE_H
