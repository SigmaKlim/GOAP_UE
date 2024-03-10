#pragma once

#include <algorithm>

#include "../Basic/AInt.h"

const t_value MAX_AMMO = 30;

class AAmmoInMag : public AInt
{
public:
    AAmmoInMag() : AInt(MAX_AMMO) {}
};
