#pragma once
#include "../Basic/AInt.h"

const int MAX_GRENADES = 3; 

class AGrenadesLeft : public AInt
{
public:
    AGrenadesLeft() : AInt(MAX_GRENADES) {};
};