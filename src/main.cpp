#include <Arduino.h>
#include <Wire.h>
//=========PINS===========================
//based on ESP32 Devkit v1 pinout
//NeoPixel data lines
#define LED_SEQ 2
#define LED_TRACK 4
#define LED_PAGE 25
//SPI & I2C pins
#define SCL 22
#define SCI 23
#define SCK 9
#define SDA 11
#define DAC1 5
#define DAC2 17
//inputs
#define PLAY 26
#define CV_IN 15
#define GATE_IN 19
//gate outputs
#define GATEA 27
#define GATEB 14
#define GATEC 13
#define GATED 12
//==========VARIABLES=====================

//=========I2C HANDLING===================


//============SETUP/LOOP==================
void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
}
 
void loop() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000);          
}