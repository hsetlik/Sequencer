#include "ControlSignal.h"

ControlSignal::ControlSignal() : isButton(true), dir(true), idx(0)
{

}

ControlSignal::ControlSignal(byte d)
{
    idx = byteIdx(d);
    isButton = bitValue(d, ISBUTTON_BIT);
    dir = bitValue(d, DIR_BIT);
}
uint8_t ControlSignal::byteIdx(byte bits)
{
    //set the 7th and 8th bit low such that the byte represents only the 8 bit index integer
    auto out = setBit(bits, ISBUTTON_BIT, false);
    out = setBit(out, DIR_BIT, false);
    return (uint8_t)out;
}
bool ControlSignal::bitValue(byte bits, uint8_t idx)
{
    auto val = bits >> (idx - 1);
    return (val & 1);
}

byte ControlSignal::asByte()
{
    //start with just the 255 bit IDX
    byte out = idx;
    //An idx between 0 and 9 only needs four bits, so the leftmost two bits can be used to store the isEncoder and direction variables
    if (isButton)
    {
        out = setBit(out, ISBUTTON_BIT, true);
        //set the dir bit low since it isn't used
        out = setBit(out, DIR_BIT, false);
    }
    else 
    {
        //set isButton bit low
        out = setBit(out, ISBUTTON_BIT, false);
        //set direction bit
        out = setBit(out, DIR_BIT, dir);
    }
    return out;
}

byte ControlSignal::setBit(byte bit, uint8_t idx, bool value)
{
    if (value)
        return (bit | (1 << (idx - 1)));
    return (bit & (~(1 << (idx - 1))));
}

void ControlSignal::printByte(byte bits)
{
    for (auto i = 0; i < 8; ++i)
    {
        Serial.print((bitValue(bits, i) ? "1" : "0"));
    }
    Serial.println("\n");
}