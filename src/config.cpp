//
// Created by sirosh on 28.08.2021.
//
#include <Arduino.h>
#include <LittleFS.h>
#include "config.h"


String *readConf(String filename) {
    String *conf = new String[2];
    File file = LittleFS.open(filename, "r");
    if (!file) {
        Serial.println("Failed to open file for reading");
        conf[0] = "";
        conf[1] = "";
    } else {
        String name = file.readStringUntil('\n');
        String pass = file.readStringUntil('\n');
        conf[0] = name;
        conf[1] = pass;
    }
    return conf;
}



void trySaveConf(String name, String pass, String filename) {
    File file = LittleFS.open(filename, "w");
    if (!file) {
        Serial.println("Error opening file for writing");
        return;
    }
    int bytesWritten = file.println(name);
    bytesWritten += file.println(pass);
    if (bytesWritten > 0) {
        Serial.print("saved ");
        Serial.print(filename);
        Serial.println(bytesWritten);

    } else {
        Serial.println("File write failed");
    }
    file.close();
}