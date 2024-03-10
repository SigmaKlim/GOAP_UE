#pragma once

#include "../Basic/AInt.h"

const t_value MAX_MAGS = 3;

class AMagsLeft : public AInt
{
public:
    AMagsLeft() : AInt(MAX_MAGS) {}
};