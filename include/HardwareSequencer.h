#ifndef HARDWARESEQUENCER_H

#include <Arduino.h>
#include <Wire.h>
#include <MCP48xx.h>
#include <Adafruit_NeoPixel.h>
#include "Sequence.h"
//#include <iostream>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <WiFi.h>
#include <Adafruit_SSD1306.h>
#include "OledLog.h"
#include "ControlSignal.h"
#include "PresetMenu.h"
//=========PINS===========================
//based on ESP32 Devkit v1 pinout
//NeoPixel data lines
#define LED_SEQ 13
#define LED_TRACK 12
#define LED_PAGE 14
//SPI & I2C pins
#define SCL 26
#define SCI 23
#define SCK 18
#define SDA 25
#define DAC1 35
#define DAC2 34
//inputs
#define CV_IN 33
#define GATE_IN 27
//gate outputs
#define GATEA 22
#define GATEB 1
#define GATEC 3
#define GATED 21
//I2C device addresses
#define CONTROLLER 8
//Other definitions
#define BRIGHTNESS 30
//Display stuff
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C



class HardwareSequencer;

//NOTE:: this function must be implemented in main.cpp
void onReceive(int i);

class HardwareSequencer
{
private:const std::string ssid = "SD Airport";
const std::string password = "plinsky1737";

AsyncWebServer server;
//Output DACs for control voltage
MCP4822 dac1;
MCP4822 dac2;
//LEDs
Adafruit_NeoPixel pagePixels;
Adafruit_NeoPixel stepPixels;
Adafruit_NeoPixel trackPixels;
//Sequence
Sequence seq;
//Display

Adafruit_SSD1306 display;

PresetMenu menu;

//mode states
bool pitchMode = true;
bool quantizeMode = false;
// input handling

void moveEncoder(uint8_t idx, bool dir);
void buttonPressed(ButtonId button, bool longPress);
//handle the incoming I2C signal from the input controller
void handleControlSignal(int);


//initializer stuff

void initWifi();
void initDACs();
void initLEDs();
void initDisplay();

//to match main.cpp

void setupSequencer();
void loopSequencer();

public:
    HardwareSequencer();
    ~HardwareSequencer() {}
};

#endif // !HARDWARESEQUENCER_H