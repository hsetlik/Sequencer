#ifndef SEQUENCEPLAYER_H
#define SEQUENCEPLAYER_H
#include "Sequence.h"
#include "etl/memory.h"
#include "etl/string.h"
//Handles I/O for sequence
class SequencePlayer
{
public:
    SequencePlayer();
    etl::unique_ptr<Sequence> seq;
    void saveCurrentAs(etl::string_base name);
    void loadSequence(etl::string_base name);
    
};

#endif