#include <Wire.h>

#define CTRL_REG_1 0x20
#define FIFO_CTRL_REG 0x2E

// registres des axes
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

int i2cAdress = 25;  // 0x19 techniquement

void sensor_write_register(int addr, int registre, int value) {
  Wire.beginTransmission(addr);
  Wire.write(registre);
  Wire.write(value);
  Wire.endTransmission();
}

byte getValue(int addr, int registre) {
  Wire.beginTransmission(addr);
  Wire.write(registre);
  Wire.endTransmission();
  Wire.requestFrom(addr, 1);
  byte val = 0;

  while (Wire.available()) {
    val = Wire.read();
  }

  return val;
}

float getAxis(int addr, int lsb, int msb) {
  byte lsb_value = getValue(addr, lsb);
  byte msb_value = getValue(addr, msb);

  int16_t rawData = (msb_value << 8) | lsb_value;

  return rawData / 16000.0;  // conversion en g
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  // setup du capteur
  sensor_write_register(i2cAdress, CTRL_REG_1, 0b01010111);
}

void loop() {

  float x = getAxis(i2cAdress, OUT_X_L, OUT_X_H);
  float y = getAxis(i2cAdress, OUT_Y_L, OUT_Y_H);
  float z = getAxis(i2cAdress, OUT_Z_L, OUT_Z_H);

  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.println(z);
  delay(500);
}