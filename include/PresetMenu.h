#ifndef PRESETMENU_H
#define PRESETMENU_H
#include "Sequence.h"
#include <Adafruit_SSD1306.h>



class PresetMenu
{
public:
    PresetMenu(Sequence* s, Adafruit_SSD1306* disp);
private:
    Sequence* const sequence;
    Adafruit_SSD1306* const display;
};

#endif