#pragma once
//#define DEBUG

#define MODBUS_BAUD 9600
#define MODBUS_CONFIG SERIAL_8N1
#define MODBUS_SLAVE_ID 11

#define NUM_INPUT_PINS 6
#define NUM_OUTPUT_PINS 6

#if defined(BOARD) && BOARD == ATmega8

#define LEDPIN PD2
#define ONE_WIRE_BUS PD3

#define GATE_PIN PD6
#define WALLBOX_PIN PD7

#define MODBUS_SERIAL Serial
#define DEBUG_SERIAL Serial
#define MAX485_DERE PD4    // Data/Read enable pin

#else

#define LEDPIN LED_BUILTIN
#define ONE_WIRE_BUS 7

#define GATE_PIN 2
#define WALLBOX_PIN 3

#define MODBUS_SERIAL swSerial
#define DEBUG_SERIAL Serial
#define MODBUS_RX 6
#define MODBUS_TX 5
#define MAX485_DERE 4

#endif
