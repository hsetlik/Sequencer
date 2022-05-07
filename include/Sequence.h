#ifndef SEQUENCE_H
#define SEQUENCE_H

#define SEQ_LENGTH 64
#define PAGE_LENGTH 16

#define NUM_TRACKS 4

#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include "Hsv.h"


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
    Track() {}
    Step steps[SEQ_LENGTH];
    //returns first the step at or before idx which has its gate toggled on
    Step* lastOnStep(uint8_t idx);
};

//Gate statuses
typedef bool TrackGates[NUM_TRACKS];

//Millivolts for each DAC channel
typedef int TrackMvs[NUM_TRACKS];

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
    //check whether enough time has elapsed to move to the next step
    void checkAdvance();
    //Update the 16-led display and the 4 page LEDs to reflect the current sequence
    void setSequenceLeds(Adafruit_NeoPixel* stepLeds, Adafruit_NeoPixel* pageLeds);
    //Set the gates
    void updateGates(TrackGates& gates);
    //update the DAC voltages
    void updateMvs(TrackMvs& mvs);
    //set the tempo and calcualte the period of each step
    void setTempo(int t);
private:
    int tempo;
    unsigned long periodMicros;
    unsigned long microsIntoPeriod;
    unsigned long lastMicros;
    void advance();
};

#endif
