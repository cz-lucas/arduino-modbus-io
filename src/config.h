#pragma once
//#define DEBUG


#define DOOR_CONTACT  8 // Pin for the ddor contact (INPUT_PULLUP)
#define GATE_CONTACT  9 // Pin for the gate contact (INPUT_PULLUP)
#define WALLBOX_ENABLE_PIN 10


#define MODBUS_BAUD 9600
#define MODBUS_CONFIG SERIAL_8N1
#define MODBUS_SLAVE_ID 11

#define NUM_INPUT_PINS 6
#define NUM_OUTPUT_PINS 6

#if defined(BOARD) && BOARD == ATmega8

#define LEDPIN PD2
#define ONE_WIRE_BUS PD3


#define MODBUS_SERIAL Serial
#define DEBUG_SERIAL Serial
#define MAX485_DERE PD4    // Data/Read enable pin

#else

#define LEDPIN LED_BUILTIN
#define ONE_WIRE_BUS 7

const uint8_t INPUT_PINS[NUM_INPUT_PINS] = {8, 9, 2, 3, 4, 5};
const uint8_t OUTPUT_PINS[NUM_OUTPUT_PINS] = {10, 11, 12, A0, A1, A2};

#define MODBUS_SERIAL swSerial
#define DEBUG_SERIAL Serial
#define MODBUS_RX 6
#define MODBUS_TX 5
#define MAX485_DERE 4

#endif
