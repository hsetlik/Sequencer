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
#include "ControlSignal.h"
#include "OledLog.h"
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
//Display stuff
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C
//==========VARIABLES=====================
//WiFi/OTA stuff
const char* ssid = "SD Airport";
const char* password = "plinsky1737";

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
//Display

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//mode states
bool pitchMode = true;
bool quantizeMode = false;
//========================================
//we need to implement the function declared in OledLog.h now that we have a display object
//how many lines the screen can fit
#define LOG_HEIGHT 4

std::string logStrings[LOG_HEIGHT];

void insertLogString(std::string str)
{
  for(int i = 1; i < LOG_HEIGHT; ++i)
  {
    logStrings[i - 1] = logStrings[i];
  }
  logStrings[LOG_HEIGHT - 1] = str;
}

void OledLog::writeLn(std::string str)
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.setTextSize(1);
  insertLogString(str);
  for(auto s : logStrings)
  {
    display.println(s.c_str());
  }
  display.display();
}
//=========I2C HANDLING===================

void moveEncoder(uint8_t idx, bool dir)
{
  switch (idx)
  {
    case 0: //First encoder moves selected note
    {
      seq.shiftSelected(dir);
      break;
    }
    case 1: //second encoder adjusts quantization root, pitch of selected step, gate length depending on mode
    {
      if (quantizeMode)
        seq.shiftQuantRoot(dir);
      else if (pitchMode)
        seq.shiftNote(dir);
      else
        seq.shiftGateLength(dir);
      break;
    }
    case 2: //Toggled between controlling tempo and controlling quantize mode
    {
      if (quantizeMode)
        seq.shiftQuantType(dir);
      else
        seq.shiftTempo(dir);
      break;
    }
    case 3:
    {
      //TODO
      //do something with the menu idk
      break;
    }
    default:
      break;
  }
}
void buttonPressed(ButtonId button)
{
  switch (button)
  {
    case A: //Toggles gate of selected note
    {
      auto s = seq.getCurrentStep();
      s.gate = !s.gate;
      break;
    }
    case B: //toggles between pitch and gate length control
    {
      pitchMode = !pitchMode;
      break;
    }
    case C: //toggles quantize mode
    {
      quantizeMode = !quantizeMode;
      break;
    }
    case D:
    {
      OledLog::writeLn("TODO: program encoder D");
      break;
    }
    case Left:
    {
      OledLog::writeLn("TODO: program left button");
      break;
    }
    case Right:
    {
      OledLog::writeLn("TODO: program right button");
      break;
    }
    case Play:
    {
      seq.isPlaying = !seq.isPlaying;
      break;
    }
    case Trk1:
    {
      seq.currentTrack = 0;
      break;
    }
    case Trk2:
    {
      seq.currentTrack = 1;
      break;
    }
    case Trk3:
    {
      seq.currentTrack = 2;
      break;
    }
    case Trk4:
    {
      seq.currentTrack = 3;
      break;
    }
    
    default:
      break;

  }
}
//alternate I2C callback
void recieveControlSignal(int)
{
  ControlSignal sig((byte)Wire.read());
  if (sig.isButton)
    buttonPressed((ButtonId)sig.idx);
  else
    moveEncoder(sig.idx, sig.dir);
}

//============HARDWARE UPDATING===========

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
//Set up OLED display
void initDisplay()
{
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    displayFailed = true;
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000);
  display.clearDisplay();
}
//============SETUP/LOOP==================
bool displayFailed = false;
void setup() 
{
  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  Wire.begin(21, 22);
  Wire.onReceive(recieveControlSignal);
  initDisplay();
  initWifi();
  initDACs();
  initLEDs();
}
int num = 0; 
void loop() 
{
  seq.checkAdvance();
  seq.setSequenceLeds(&stepPixels, &pagePixels);
  seq.updateGates();
  seq.updateMvs();
}