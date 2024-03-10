#pragma once

#include "../Basic/AInt.h"

const t_value MAX_HEALTHKITS = 3;

class AHKitsLeft : public AInt
{
public:
    AHKitsLeft() : AInt(MAX_HEALTHKITS) {}
};