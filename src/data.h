#pragma once

class Data {
    private:
        float temperature;
        bool doorState;
        bool gateState;

    public:
        float getTemperature();
        void setTemperature(float _temperature);
        void writeToInputRegisters(uint16_t* inputRegisters, uint16_t startIndex);

        bool getGateState();
        void setGateState(bool _state);
};