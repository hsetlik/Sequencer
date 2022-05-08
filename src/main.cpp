#include <Arduino.h>
#include <Wire.h>
#include <MCP48xx.h>
#include <Adafruit_NeoPixel.h>
#include "Sequence.h"
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
//I2C device addresses
#define CONTROLLER 8
//==========VARIABLES=====================
//Output DACs for control voltage
MCP4822 dac1(DAC1);
MCP4822 dac2(DAC2);
//LEDs
Adafruit_NeoPixel pagePixels(4, LED_PAGE, NEO_GRB + NEO_KHZ800);
//Sequence
Sequence seq;
//=========I2C HANDLING===================
void moveEncoder(uint8_t idx, bool dir)
{
  //TODO
}
void buttonPressed(uint8_t idx)
{
  switch (idx)
  {
    case 0: //Encoder A switch
    {
      break;
    }
    case 1://Encoder B switch
    {
      break;
    }
   case 2: //Encoder C switch
    {
      break;
    }
    case 3: //Encoder D switch
    {
      break;
    }
    case 4: //Menu left button
    {
      break;
    }
    case 5: //Menu right button
    {
      break;
    }
    default:
      break;

  }
}
void recieveEvent(int num)
{
    bool isEncoder = Wire.read() == 1;
    int idx = Wire.read();
    bool dir = Wire.read() == 1;
    if(isEncoder)
    {
        moveEncoder(idx, dir);
    }
    else
    {
        buttonPressed(idx);
    }
}
//============HARDWARE UPDATING===========
void updateDACs()
{

}
void updatePixels()
{

}
void updateGates()
{

}
//============SETUP/LOOP==================
void setup() 
{
  Wire.begin(CONTROLLER);
  Wire.onReceive(recieveEvent);
  Serial.begin(9600);
}
 
void loop() 
{

}