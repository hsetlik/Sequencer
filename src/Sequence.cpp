#include "Sequence.h"

//=====================STEP==============================
Step::Step() : midiNumber(69), gate(false), length(80)
{
}
//=====================TRACK==============================
//=====================SEQUENCE==============================
Sequence::Sequence() :
currentStep(0),
currentTrack(0),
selectedStep(0),
isPlaying(false),
tempo(120),
periodMicros(0),
microsIntoPeriod(0),
lastMicros(0)
{
    setTempo(tempo);
}

void Sequence::checkAdvance()
{
    //TODO
    auto now = micros();
    microsIntoPeriod += (now - lastMicros);
    if (microsIntoPeriod >= periodMicros)
        advance();
    lastMicros = now;
}

void Sequence::setSequenceLeds(Adafruit_NeoPixel* stepLeds, Adafruit_NeoPixel* pageLeds)
{
    //TODO
}

void Sequence::advance()
{
    microsIntoPeriod -= periodMicros;
    currentStep = (currentStep + 1) % SEQ_LENGTH;
}

void Sequence::updateGates(TrackGates& gates)
{
    //TODO
}

void Sequence::setTempo(int t)
{
    tempo = t;
    periodMicros = (unsigned long)(60000000.0f / (float)tempo);
}
