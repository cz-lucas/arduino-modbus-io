#include <Arduino.h>
#include <build_info.h> // This will be auto-generated
#include "config.h"
#include "data.h"

#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "SimpleTimer.h"

SoftwareSerial swSerial(MODBUS_RX, MODBUS_TX);
#define MODBUS_SERIAL swSerial
#define DEBUG_SERIAL Serial
ModbusRTUSlave modbus(MODBUS_SERIAL, MAX485_DERE);

// Setup a OneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass OneWire reference to Dallas Temperature
DallasTemperature sensors(&oneWire);

SimpleTimer ledBlinkTimer(750);
SimpleTimer DS18B20ReadTimer(20000);
SimpleTimer ReedSwitchReadTimer(2500);

Data data;

const uint8_t numCoils = 2;
const uint8_t numInputRegisters = 2;

bool coils[numCoils];
uint16_t inputRegisters[numInputRegisters];

void readDS18B20()
{
  sensors.requestTemperatures();
  data.setTemperature(sensors.getTempCByIndex(0));
  Serial.println("[readDS18B20] Reading DS18B20: Temperature: " + String(data.getTemperature()) + "°C");
  data.writeToInputRegisters(inputRegisters, 0);
}

void readDigitalSensors()
{
  data.setDoorState(digitalRead(DOOR_CONTACT));
  data.setGateState(digitalRead(GATE_CONTACT));
  coils[0] = data.getDoorState();
  coils[1] = data.getGateState();
}

void setup()
{
  pinMode(LEDPIN, OUTPUT);
  pinMode(DOOR_CONTACT, INPUT_PULLUP);
  pinMode(GATE_CONTACT, INPUT_PULLUP);
  digitalWrite(LEDPIN, HIGH);

  DEBUG_SERIAL.begin(115200);
  DEBUG_SERIAL.println("Arduino modbus node");
  DEBUG_SERIAL.println(String("Commit: ") + GIT_COMMIT_HASH);
  DEBUG_SERIAL.println(String("Buildtime: ") + BUILD_TIMESTAMP);
  DEBUG_SERIAL.println(String("Modbus ID: ") + MODBUS_SLAVE_ID);
  DEBUG_SERIAL.println("####################################");

#ifdef DEBUG
  DEBUG_SERIAL.println("[SETUP] Initializing modbus");
#endif

  modbus.configureCoils(coils, numCoils);
  // modbus.configureDiscreteInputs(discreteInputs, numDiscreteInputs);
  // modbus.configureHoldingRegisters(holdingRegisters, numHoldingRegisters);
  modbus.configureInputRegisters(inputRegisters, numInputRegisters);

  MODBUS_SERIAL.begin(9600);
  modbus.begin(MODBUS_SLAVE_ID, MODBUS_BAUD, MODBUS_CONFIG);

#ifdef DEBUG
  DEBUG_SERIAL.println("[SETUP] Initializing onewire");
#endif
  sensors.begin();

#ifdef DEBUG
  DEBUG_SERIAL.println("[SETUP] Reading sensors");
#endif
  readDS18B20();
  readDigitalSensors();

#ifdef DEBUG
  DEBUG_SERIAL.println("[SETUP] Setup complete");
#endif
  digitalWrite(LEDPIN, LOW);
}

void loop()
{
  modbus.poll();

  if (ledBlinkTimer.expired())
  {
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));
  }

  if (DS18B20ReadTimer.expired())
  {
    readDS18B20();
  }

  if (ReedSwitchReadTimer.expired())
  {
    readDigitalSensors();
  }
}
