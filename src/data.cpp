#pragma once
#include <Arduino.h>
#include "data.h"


float Data::getTemperature() {
    return temperature;
}

void Data::setTemperature(float _temperature) {
    if(_temperature != -127) {
        this->temperature = _temperature;
    }
}

void Data::writeToInputRegisters(uint16_t* inputRegisters, uint16_t startIndex) {
    int32_t scaledTemp = static_cast<int32_t>(this->temperature * 10);
    inputRegisters[startIndex] = static_cast<uint16_t>((scaledTemp >> 16) & 0xFFFF);   // High word
    inputRegisters[startIndex + 1] = static_cast<uint16_t>(scaledTemp & 0xFFFF);       // Low word
}

bool Data::getGateState() {
    return this->gateState;
}

void Data::setGateState(bool _state) {
    this->gateState = !_state;
}