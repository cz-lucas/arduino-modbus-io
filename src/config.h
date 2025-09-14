#pragma once
#define LEDPIN LED_BUILTIN

#define ONE_WIRE_BUS 7
#define MAX485_DERE 4 // Data/Read enable pin

#define MODBUS_RX 6  // SoftwareSerial RX (Arduino receives data here)
#define MODBUS_TX 5  // SoftwareSerial TX (Arduino transmits data here)

#define MODBUS_BAUD 9600
#define MODBUS_CONFIG SERIAL_8N1
#define MODBUS_SLAVE_ID 15

