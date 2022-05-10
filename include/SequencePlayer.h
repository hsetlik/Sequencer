#ifndef SEQUENCEPLAYER_H
#define SEQUENCEPLAYER_H
#include "Sequence.h"
#include <string>
#include <memory>
//Handles I/O for sequence
class SequencePlayer
{
public:
    SequencePlayer();
    //Access to the underlying sequence object
    Sequence& seq() {return *sequence; }
    //Loads the sequence with the specified name and returns true if successful
    bool loadSequence(std::string name);
    //Saves the current sequence with the specified name. Returns true if successful
    bool saveSequence(std::string name);
private:
    std::unique_ptr<Sequence> sequence;

};
/*
DUMMY JSON OBJECTS:

STEP:
{
    "midiNumber": 69,
    "gate": false,
    "length": 80
}
=============================
SEQUENCE:
{
    "name": "sequence name",
    "tempo": 120,
    "tracks": [
        [
        {
    "midiNumber": 69,
    "gate": false,
    "length": 80
    }

        ]
    ]

}


*/

#endif