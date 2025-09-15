#include <Arduino.h>
#include <build_info.h> // This will be auto-generated
#include "config.h"
#include "data.h"
#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>
#include <OneWire.h>
#include "SimpleTimer.h"

#if !(defined(BOARD) && BOARD == ATmega8)
// SoftwareSerial setup for Modbus
SoftwareSerial swSerial(MODBUS_RX, MODBUS_TX);
#endif

ModbusRTUSlave modbus(MODBUS_SERIAL, MAX485_DERE);

// OneWire setup for DS18B20 temperature sensor
OneWire oneWire(ONE_WIRE_BUS);

// SimpleTimers
SimpleTimer digitalIoTimer(750);     // LED blinking timer
SimpleTimer DS18B20ReadTimer(10000); // Temperature reading timer
SimpleTimer wallboxTimer(1000);      // Wallbox timer to control the enable pin every 1 second

// Data storage
Data data;

const uint8_t numCoils = 2;
const uint8_t numDiscreteRegisters = 2;
const uint8_t numInputRegisters = 2;

// State tracking arrays
bool discrete[numDiscreteRegisters];
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
  discrete[0] = data.getDoorState();
  discrete[1] = data.getGateState();

  // Set the wallbox pin state based on the coils[1] (could be controlled via Modbus)
  digitalWrite(WALLBOX_ENABLE_PIN, coils[1]);
  coils[1] = digitalRead(WALLBOX_ENABLE_PIN); // Track the pin state
}

void setup()
{
  pinMode(LEDPIN, OUTPUT);
  pinMode(WALLBOX_ENABLE_PIN, OUTPUT);
  pinMode(MAX485_DERE, OUTPUT);
  digitalWrite(MAX485_DERE, LOW);
  pinMode(DOOR_CONTACT, INPUT_PULLUP);
  pinMode(GATE_CONTACT, INPUT_PULLUP);
  digitalWrite(LEDPIN, HIGH);
  digitalWrite(WALLBOX_ENABLE_PIN, LOW);

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

  MODBUS_SERIAL.begin(9600);
  #ifndef DEBUG
  delay(20);
  DEBUG_SERIAL.println(GIT_COMMIT_STRING);
  DEBUG_SERIAL.flush();
  #endif
  delay(50);

  // Modbus initialization
  modbus.configureCoils(coils, numCoils);
  modbus.configureInputRegisters(inputRegisters, numInputRegisters);
  modbus.configureDiscreteInputs(discrete, numDiscreteRegisters);
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

  digitalWrite(WALLBOX_ENABLE_PIN, coils[0]);

  // Periodic sensor readings
  if (DS18B20ReadTimer.expired())
  {
    readDS18B20();
  }

  // LED control (this is for blinking the LED)
  if (digitalIoTimer.expired())
  {
    digitalWrite(LEDPIN, !digitalRead(LEDPIN)); // Toggle LED
    readDigitalSensors();                       // Read the digital sensors after LED toggle
  }

  // Wallbox timer for periodic control (can add logic here if needed)
  /*if (wallboxTimer.expired())
  {
    // Additional logic for periodic updates, if necessary
  }*/
}
