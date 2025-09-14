#include <Arduino.h>
#include <build_info.h> // This will be auto-generated
#include "config.h"
#include "data.h"

#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>
#include <OneWire.h>
#include "SimpleTimer.h"

// SoftwareSerial setup for Modbus
SoftwareSerial swSerial(MODBUS_RX, MODBUS_TX);
#define MODBUS_SERIAL swSerial
#define DEBUG_SERIAL Serial
ModbusRTUSlave modbus(MODBUS_SERIAL, MAX485_DERE);

// OneWire setup for DS18B20 temperature sensor
OneWire oneWire(ONE_WIRE_BUS);

// SimpleTimers
SimpleTimer digitalInputTimer(750);
SimpleTimer DS18B20ReadTimer(10000);

// Data storage
Data data;

const uint8_t numCoils = 2;
const uint8_t numInputRegisters = 2;

// State tracking arrays
bool coils[numCoils];
uint16_t inputRegisters[numInputRegisters];

// Inline function to save space
inline void readDS18B20()
{
  byte oneWireData[12]; // Array to store sensor data
  byte addr[8];         // Address of the sensor

  if (!oneWire.search(addr))
  {
    oneWire.reset_search();
    return; // If no sensor is found, exit early
  }

  // If found, send the command to get the temperature
  oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0x44, 1); // Start temperature conversion (0x44 is the command)

  delay(1000); // Wait for the conversion to complete (usually takes 750ms to 1 second)

  // Now read the temperature data
  oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0xBE); // Command to read the scratchpad

  // Read 9 bytes from the sensor (the first two are temperature data)
  for (int i = 0; i < 9; i++)
  {
    oneWireData[i] = oneWire.read();
  }

  // Convert the raw data to temperature
  int16_t rawTemp = (oneWireData[1] << 8) | oneWireData[0]; // Combine high and low byte
  float temperature = (float)rawTemp / 16.0;                // Convert to Celsius (the DS18B20 outputs temperature in 16ths of a degree)

  data.setTemperature(temperature);
  // DEBUG_SERIAL.println(F("[readDS18B20] Temperature: ") + String(data.getTemperature()) + F("°C"));

  // Write the temperature to the input registers
  data.writeToInputRegisters(inputRegisters, 0);
}

inline void readDigitalSensors()
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

// Debugging setup (only if debug is enabled)
#ifdef DEBUG
  DEBUG_SERIAL.begin(115200);
  DEBUG_SERIAL.println(F("Arduino modbus node"));
  DEBUG_SERIAL.println(String(F("Buildtime: ")) + String(BUILD_TIMESTAMP));
  DEBUG_SERIAL.println(String(F("Commit: ")) + String(GIT_COMMIT_HASH));
  DEBUG_SERIAL.println(String(F("Modbus ID: ")) + String(MODBUS_SLAVE_ID));
  DEBUG_SERIAL.println(F("####################################"));
  DEBUG_SERIAL.println(F("[SETUP] Initializing modbus"));
#endif

  // Modbus initialization
  modbus.configureCoils(coils, numCoils);
  modbus.configureInputRegisters(inputRegisters, numInputRegisters);
  MODBUS_SERIAL.begin(9600);
  modbus.begin(MODBUS_SLAVE_ID, MODBUS_BAUD, MODBUS_CONFIG);

// OneWire setup
#ifdef DEBUG
  DEBUG_SERIAL.println(F("[SETUP] Initializing onewire"));
#endif

#ifdef DEBUG
  DEBUG_SERIAL.println(F("[SETUP] Reading sensors"));
#endif
  readDS18B20();        // Read temperature from DS18B20
  readDigitalSensors(); // Read digital sensors

#ifdef DEBUG
  DEBUG_SERIAL.println(F("[SETUP] Setup complete"));
#endif
  digitalWrite(LEDPIN, LOW);
}

void loop()
{
  // Poll Modbus
  modbus.poll();

  // Periodic sensor readings
  if (DS18B20ReadTimer.expired())
  {
    readDS18B20();
  }
  
  if (digitalInputTimer.expired())
  {
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));
    readDigitalSensors();
  }
}
