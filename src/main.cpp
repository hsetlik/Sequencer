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
//Other definitions
#define BRIGHTNESS 30
//==========VARIABLES=====================
//WiFi/OTA stuff
const char* ssid = "SD Airport 5GHz";
const char* password = "Plinkun1737";

AsyncWebServer server(80);
//Output DACs for control voltage
MCP4822 dac1(DAC1);
MCP4822 dac2(DAC2);
//LEDs
Adafruit_NeoPixel pagePixels(4, LED_PAGE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stepPixels(16, LED_SEQ, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel trackPixels(4, LED_TRACK, NEO_GRB + NEO_KHZ800);
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
    case 6: //play/stop button
    {
      seq.isPlaying = !seq.isPlaying;
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
//============SETUP SUBROUTINES===========
//Set up wifi and enable OTA firmware updates
void initWifi()
{
  WiFi.mode(WIFI_STA);
  auto res = WiFi.begin(ssid, password);
  if (res == WL_CONNECT_FAILED)
  {
    Serial.println("Connection failed");
  } else if (res == WL_NO_SSID_AVAIL)
  {
    Serial.print("No SSID Available with name ");
    Serial.println(ssid);
  }
  Serial.println(res);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) 
  {
    request->send(200, "text/plain", "Hi! I am ESP32.");
  });
  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}

//Establish I2C serial connection to input controller
void initI2C()
{
  Wire.begin(CONTROLLER);
  Wire.onReceive(recieveEvent);
}

//Set up MCP4822 DAC outputs
void initDACs()
{
  dac1.init();
  dac2.init();

  dac1.turnOnChannelA();
  dac1.turnOnChannelB();

  dac2.turnOnChannelA();
  dac2.turnOnChannelB();

  dac1.setGainA(MCP4822::High);
  dac1.setGainB(MCP4822::High);

  dac2.setGainA(MCP4822::High);
  dac2.setGainB(MCP4822::High);
}

//Set up LEDs
void initLEDs()
{
  pagePixels.begin();
  trackPixels.begin();
  stepPixels.begin();

  pagePixels.setBrightness(BRIGHTNESS);
  trackPixels.setBrightness(BRIGHTNESS);
  stepPixels.setBrightness(BRIGHTNESS);
}
//============SETUP/LOOP==================
void setup() 
{
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  initWifi();
  initI2C();
  initDACs();
  initLEDs();
}
 
void loop() 
{
  digitalWrite(2, HIGH);
  delay(200);
  digitalWrite(2, LOW);
  delay(200);
  

}