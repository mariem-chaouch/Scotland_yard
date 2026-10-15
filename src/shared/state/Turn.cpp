#include "Turn.h"

state::Turn::Turn ()
{
    number = turn_count;
    turn_count++;
}