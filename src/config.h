#pragma once
//#define DEBUG


#define DOOR_CONTACT  8 // Pin for the ddor contact (INPUT_PULLUP)
#define GATE_CONTACT  9 // Pin for the gate contact (INPUT_PULLUP)
#define WALLBOX_ENABLE_PIN 10


#define MODBUS_BAUD 9600
#define MODBUS_CONFIG SERIAL_8N1
#define MODBUS_SLAVE_ID 15

#if defined(BOARD) && BOARD == ATmega8

#define LEDPIN 13
#define ONE_WIRE_BUS 7

#define MODBUS_SERIAL Serial
#define DEBUG_SERIAL Serial
#define MAX485_DERE 4    // Data/Read enable pin

#else

#define LEDPIN 13
#define ONE_WIRE_BUS 7

#define MODBUS_SERIAL swSerial
#define DEBUG_SERIAL Serial
#define MODBUS_RX 6
#define MODBUS_TX 5
#define MAX485_DERE 4

#endif
