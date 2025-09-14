#pragma once
//#define DEBUG

#define LEDPIN 13
#define ONE_WIRE_BUS 7

#define DOOR_CONTACT  8 // Pin for the ddor contact (INPUT_PULLUP)
#define GATE_CONTACT  9 // Pin for the gate contact (INPUT_PULLUP)
#define WALLBOX_ENABLE_PIN 10

#define MODBUS_RX 6  // SoftwareSerial RX (Arduino receives data here)
#define MODBUS_TX 5  // SoftwareSerial TX (Arduino transmits data here)
#define MAX485_DERE 4 // Data/Read enable pin

#define MODBUS_BAUD 9600
#define MODBUS_CONFIG SERIAL_8N1
#define MODBUS_SLAVE_ID 15

