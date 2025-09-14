#include <Arduino.h>
#include <build_info.h>  // This will be auto-generated
#include "config.h"

#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>


SoftwareSerial swSerial (MODBUS_RX, MODBUS_TX);
#define MODBUS_SERIAL swSerial
#define DEBUG_SERIAL Serial
ModbusRTUSlave modbus(MODBUS_SERIAL, MAX485_DERE);

const uint8_t numInputRegisters = 2;
uint16_t inputRegisters[numInputRegisters];

void setup()
{
  DEBUG_SERIAL.begin(115200);
  DEBUG_SERIAL.println("Arduino modbus node");
  DEBUG_SERIAL.println(String("Commit: ") + GIT_COMMIT_HASH);
  DEBUG_SERIAL.println(String("Buildtime: ") + BUILD_TIMESTAMP);
  DEBUG_SERIAL.println("####################################");

  DEBUG_SERIAL.println("[SETUP] Initializing modbus");
  //modbus.configureCoils(coils, numCoils);
  //modbus.configureDiscreteInputs(discreteInputs, numDiscreteInputs);
  //modbus.configureHoldingRegisters(holdingRegisters, numHoldingRegisters);
  modbus.configureInputRegisters(inputRegisters, numInputRegisters);
  MODBUS_SERIAL.begin(9600);
  modbus.begin(MODBUS_SLAVE_ID, MODBUS_BAUD, MODBUS_CONFIG);

  pinMode(LEDPIN, OUTPUT);
}

void loop()
{
  inputRegisters[0] = random(0,200);
  inputRegisters[1] = 200;
  modbus.poll();
  //digitalWrite(LEDPIN, !digitalRead(LEDPIN));
}
