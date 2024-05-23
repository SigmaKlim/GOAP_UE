#pragma once

#include "../Basic/AInt.h"

const t_value MAX_HEALTH = 100;

class AHealth : public AInt
{
public:
    AHealth() : AInt(MAX_HEALTH) {}
};
