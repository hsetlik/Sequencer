#include "HardwareSequencer.h"

HardwareSequencer::HardwareSequencer() :
server(80),
dac1(DAC1),
dac2(DAC2),
pagePixels(4, LED_PAGE, NEO_GRB + NEO_KHZ800),
stepPixels(16, LED_SEQ, NEO_GRB + NEO_KHZ800),
trackPixels(4, LED_PAGE, NEO_GRB + NEO_KHZ800),
display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
menu(&seq, &display)
{

}

void HardwareSequencer::moveEncoder(uint8_t idx, bool dir)
{
    switch (idx)
    {
    case 0: // First encoder moves selected note
    {
        seq.shiftSelected(dir);
        break;
    }
    case 1: // second encoder adjusts quantization root, pitch of selected step, gate length depending on mode
    {
        if (quantizeMode)
            seq.shiftQuantRoot(dir);
        else if (pitchMode)
            seq.shiftNote(dir);
        else
            seq.shiftGateLength(dir);
        break;
    }
    case 2: // Toggled between controlling tempo and controlling quantize mode
    {
        if (quantizeMode)
            seq.shiftQuantType(dir);
        else
            seq.shiftTempo(dir);
        break;
    }
    case 3:
    {
        // TODO
        // do something with the menu idk
        break;
    }
    default:
        break;
    }
}

void HardwareSequencer::buttonPressed(ButtonId button, bool longPress)
{
    switch (button)
    {
    case A: // Toggles gate of selected note
    {
        auto s = seq.getCurrentStep();
        s.gate = !s.gate;
        break;
    }
    case B: // toggles between pitch and gate length control
    {
        pitchMode = !pitchMode;
        break;
    }
    case C: // toggles quantize mode
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

//handle the incoming I2C signal from the input controller
void HardwareSequencer::handleControlSignal(int)
{

}
//initializer stuff

void HardwareSequencer::initWifi()
{
    WiFi.mode(WIFI_STA);
    auto res = WiFi.begin(ssid.c_str(), password.c_str());
    if (res == WL_CONNECT_FAILED)
    {
        OledLog::writeLn("Connection failed");
    }
    else if (res == WL_NO_SSID_AVAIL)
    {
        OledLog::writeLn("Network " + ssid + " not available");
    }
    Serial.println(res);
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    auto ip = WiFi.localIP();
    std::string logString = "Connected to " + ssid + " with IP address " + std::to_string(ip);
    OledLog::writeLn(logString);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hi! I am ESP32."); });
    AsyncElegantOTA.begin(&server); // Start ElegantOTA
    server.begin();
    OledLog::writeLn("HTTP server started");
}
void HardwareSequencer::initDACs()
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
void HardwareSequencer::initLEDs()
{
    pagePixels.begin();
    trackPixels.begin();
    stepPixels.begin();

    pagePixels.setBrightness(BRIGHTNESS);
    trackPixels.setBrightness(BRIGHTNESS);
    stepPixels.setBrightness(BRIGHTNESS);
}
void HardwareSequencer::initDisplay()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    display.display();
    delay(2000);
    display.clearDisplay();
}

//to match main.cpp

void HardwareSequencer::setupSequencer()
{
    pinMode(2, OUTPUT);
    pinMode(13, OUTPUT);
    Serial.begin(115200);
    Wire.begin(SDA, SCL);
    Wire.onReceive(onReceive);
    initDisplay();
    initWifi();
    initDACs();
    initLEDs();
}

void HardwareSequencer::loopSequencer()
{
    seq.checkAdvance();
    seq.setSequenceLeds(&stepPixels, &pagePixels);
    seq.updateGates();
    seq.updateMvs();
}
