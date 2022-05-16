# Ultra 64- 4 channel Eurorack step sequencer

The Ultra 64 is a powerful step sequencer built to Eurorack specifications. Each of its four channels contains 64 steps which can contain notes of variable pitch and gate length.

## Hardware

### Front Panel

- 16 RGB LEDs represent a 16 step section of the current sequence. The color of the LED on active steps reflects the step's pitch.
- 4 LEDs indicate which 16 step section is currently selected
- 4 LEDs and matching buttons select track to edit
- Left Encoder toggles between controlling pitch and gate length. In quantize mode (see below), the left encoder control the root of the quantizing scale.
- Center encoder selects step and toggles gate
- Right encoder toggles between control of tempo and quantize type

### Guts

The ESP32 microcontroller is at the core of the sequencer. It controls the DACs for 1v/octave output, drives the LEDs and the OLED display, and manages all the sequencing logic. Input data from the buttons and encoders on the front panel is handled by a second microcontroller-- an Arduino Nano-- and sent to the ESP32 over I2C.

## How it works

### Input Arduino

The four rotary encoders are each connected to a pair of digital pins with external pullup resistors. The integrated push button of each encoder is wired to its own digital pin, making use of the built-in pullup resistor. The remaining push-buttons are set up as an analog button array: a voltage divider where the volgate at each node of the divider can be connected to an analog input pin on the Arduino. Via an `analogRead(pin)`, the input voltage can be checked against a maximum and minimun voltage for each button in the array.

#### I2C Communication

Any user input signal from the buttons and encoders is communicated to the core ESP32 via the I2C bus. Each movement is be expressed as a single byte-- providing fast and efficient handling of input signals. The movements are encoded like so:

### Byte encoding

| 7 | 6 | 5  4  3  2  1  0 |
|---|---|---|
|Whether the signal has come from an encoder or a button| The direction of the encoder OR the press type of the button (long or short) | The remaining six bits represent the ID of the input as an integer 0-63 |

For example, imagine that the encoder with id = 2 is turned one detent counterclockwise. Working from left to right, bit 7 is high because the signal is coming from an encoder, bit 6 is low because the encoder turned conterclockwise, and bits 5-0 are the last 6 bits of an 8 bit integer ID. </br> Therefore the output byte looks like: </br> ***1000010***
