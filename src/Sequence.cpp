#include "Sequence.h"

//=====================STEP==============================
Step::Step() : midiNumber(69), gate(false), length(80)
{
}
//=====================TRACK==============================
Step* Track::lastOnStep(uint8_t idx)
{
    uint8_t stepsChecked = 0;
    while (stepsChecked < SEQ_LENGTH)
    {
        if (steps[idx].gate)
            return &steps[idx];
        idx = (idx > 0) ? idx - 1 : SEQ_LENGTH - 1;
    }
    return nullptr;
}
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
    //clear pixels
    stepLeds->clear();
    pageLeds->clear();
    //first: figure out what page we're on
    int page = floor(currentStep / PAGE_LENGTH);  
    if (page < 0 || page > 3)
    {
        //page is out of range
    }
    //set colors for all the notes
    auto firstStepIdx = page * PAGE_LENGTH;
    for (int i = 0; i < PAGE_LENGTH; ++i)
    {
        auto& step = tracks[currentTrack].steps[firstStepIdx + i];
        if (!step.gate && (firstStepIdx + i) != currentStep)
        {
            Hsv col = {0.0f, 0.0f, 0.0f};
            stepLeds->setPixelColor(i, col.asRgb());
        }
        else if (!step.gate)
        {
            auto color = SeqColors::stepColor;
            stepLeds->setPixelColor(i, color.asRgb());
        } else if (step.gate && selectedStep == (firstStepIdx + i))
        {
            auto base = SeqColors::pitchColors[step.midiNumber % 12];
            Hsv col = {base.h, base.s, 1.0f};
            stepLeds->setPixelColor(i, col.asRgb());
        } else if (step.gate)
        {
            auto col = SeqColors::pitchColors[step.midiNumber % 12];
            stepLeds->setPixelColor(i, col.asRgb());
        } else if (selectedStep == (firstStepIdx + i))
        {
            auto col = SeqColors::selectColor;
            stepLeds->setPixelColor(i, col.asRgb());
        }
    }
    
    //send to hardware
    stepLeds->show();
    pageLeds->show();
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

void Sequence::updateMvs(TrackMvs& mvs)
{
    //TODO
}
void Sequence::setTempo(int t)
{
    tempo = t;
    periodMicros = (unsigned long)(60000000.0f / (float)tempo);
}
