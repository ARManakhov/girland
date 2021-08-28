//
// Created by sirosh on 28.08.2021.
//
#include <Arduino.h>
#include <FastLED.h>
#include "palette.h"
#include "settings.h"
#include "routing.h"

CRGB leds[LED_NUM];

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette() {
    for (int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV(random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette() {
    // 'black out' all 16 palette entries...
    fill_solid(currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette() {
    CRGB purple = CHSV(HUE_PURPLE, 255, 255);
    CRGB green = CHSV(HUE_GREEN, 255, 255);
    CRGB black = CRGB::Black;

    currentPalette = CRGBPalette16(
            green, green, black, black,
            purple, purple, black, black,
            green, green, black, black,
            purple, purple, black, black);
}


const TProgmemPalette16 myRGYB_p
PROGMEM =
        {
                CRGB::Red,
                CRGB::Green,
                CRGB::Yellow,
                CRGB::Blue,

                CRGB::Red,
                CRGB::Green,
                CRGB::Yellow,
                CRGB::Blue,

                CRGB::Red,
                CRGB::Green,
                CRGB::Yellow,
                CRGB::Blue,

                CRGB::Red,
                CRGB::Green,
                CRGB::Yellow,
                CRGB::Blue
        };
// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p
PROGMEM =
        {
                CRGB::Red,
                CRGB::Gray, // 'white' is too bright compared to red and blue
                CRGB::Blue,
                CRGB::Black,

                CRGB::Red,
                CRGB::Gray,
                CRGB::Blue,
                CRGB::Black,

                CRGB::Red,
                CRGB::Red,
                CRGB::Gray,
                CRGB::Gray,
                CRGB::Blue,
                CRGB::Blue,
                CRGB::Black,
                CRGB::Black
        };


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.
CRGBPalette16 currentPalette;
TBlendType currentBlending;

void setPallete(int secondHand) {
    if (secondHand == 0) {
        currentPalette = RainbowColors_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 1) {
        currentPalette = RainbowStripeColors_p;
        currentBlending = NOBLEND;
    }
    if (secondHand == 2) {
        currentPalette = RainbowStripeColors_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 3) {
        SetupPurpleAndGreenPalette();
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 4) {
        SetupTotallyRandomPalette();
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 5) {
        SetupBlackAndWhiteStripedPalette();
        currentBlending = NOBLEND;
    }
    if (secondHand == 6) {
        SetupBlackAndWhiteStripedPalette();
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 7) {
        currentPalette = CloudColors_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 8) {
        currentPalette = PartyColors_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 9) {
        currentPalette = myRedWhiteBluePalette_p;
        currentBlending = NOBLEND;
    }
    if (secondHand == 10) {
        currentPalette = myRedWhiteBluePalette_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 11) {
        currentPalette = myRGYB_p;
        currentBlending = NOBLEND;
    }
    if (secondHand == 12) {
        currentPalette = myRGYB_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 13) {
        currentPalette = OceanColors_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 14) {
        currentPalette = LavaColors_p;
        currentBlending = LINEARBLEND;
    }
    if (secondHand == 15) {
        currentPalette = ForestColors_p;
        currentBlending = LINEARBLEND;
    }

}


void FillLEDsFromPaletteColors(uint8_t colorIndex) {
    uint8_t brightness = 255;

    for (auto & led : leds) {
        led = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void initFastLed(){
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_NUM).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(brightness);
}