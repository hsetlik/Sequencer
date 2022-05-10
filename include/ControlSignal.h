#ifndef CONTROLSIGNAL_H
#define CONTROLSIGNAL_H
#include <Arduino.h>
//definitions for control signal bits
#define ISBUTTON_BIT 6
#define DIR_BIT 7
//==============================
enum ButtonId
{
    A,
    B,
    C,
    D,
    Left,
    Right,
    Trk1,
    Trk2,
    Trk3,
    Trk4
};
struct ControlSignal
{
public:
    ControlSignal();
    ControlSignal(byte d);
    bool isButton;
    bool dir;
    uint8_t idx;
    byte asByte();
 private:
    //basic bit manipulation functions
    static byte setBit(byte bits, uint8_t idx, bool value);
    static uint8_t byteIdx(byte bits);
    static bool bitValue(byte bits, uint8_t idx);
    static void printByte(byte bits);

};



#endif