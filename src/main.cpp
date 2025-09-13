#include <Arduino.h>
#include <build_info.h>  // This will be auto-generated
#include "pins.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("Arduino modbus node");
  Serial.println(String("Commit: ") + GIT_COMMIT_HASH);
  Serial.println(String("Buildtime: ") + BUILD_TIMESTAMP);
  Serial.println("####################################");

  pinMode(LEDPIN, OUTPUT);
}

void loop()
{
  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
  delay(250);
}
