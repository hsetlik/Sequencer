#ifndef SEQUENCE_H
#define SEQUENCE_H

#define SEQ_LENGTH 64
#define PAGE_LENGTH 16

#define NUM_TRACKS 4

#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include "Hsv.h"

//the microcontroller pins attached to each gate output
const uint8_t gatePins[] = {2, 3, 4, 5};

//One step in a sequence track
struct Step
{
    Step();
    uint8_t midiNumber;
    bool gate;
    //gate length as a percentage of the length of one step 
    int length;
};

//One of a seqnence's four tracks
struct Track
{
    Track(): gateHigh(false) {}
    Step steps[SEQ_LENGTH];
    bool gateHigh;
    //returns first the step at or before idx which has its gate toggled on
    uint8_t lastOnStep(uint8_t idx);
};

//Gate statuses

//Millivolts for each DAC channel

//All the data for playback/saving a sequence
class Sequence
{
public:
    Sequence();
    Track tracks[NUM_TRACKS];
    uint8_t currentStep;
    uint8_t currentTrack;
    uint8_t selectedStep;
    bool isPlaying; 
    //check whether enough time has elapsed to move to the next step. Call this on every loop before updating hardware
    void checkAdvance();
    //Update the 16-led display and the 4 page LEDs to reflect the current sequence
    void setSequenceLeds(Adafruit_NeoPixel* stepLeds, Adafruit_NeoPixel* pageLeds);
    //Set the gates
    void updateGates();
    //update the DAC voltages
    void updateMvs();
    //set the tempo and calcualte the period of each step
    void setTempo(int t);
    int getTempo() {return tempo;}
private:
    int tempo;
    unsigned long periodMicros;
    unsigned long microsIntoPeriod;
    unsigned long lastMicros;
    void advance();
};

#endif
