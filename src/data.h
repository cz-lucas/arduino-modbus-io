#pragma once

class Data {
    private:
        double temperature;
        bool doorState;
        bool gateState;

    public:
        double getTemperature();
        void setTemperature(double _temperature);
        void writeToInputRegisters(uint16_t* inputRegisters, uint16_t startIndex);

        bool getDoorState();
        void setDoorState(bool _state);

        bool getGateState();
        void setGateState(bool _state);
};