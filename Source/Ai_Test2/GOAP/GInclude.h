#pragma once

#include "Strategist.h"
#include "Planner.h"
#include "Helper.h"

#include "Actions/Basic/AcSimple.h"
#include "Actions/Specific/AcGoTo.h"
#include "Actions/Specific/AcPickupDepletable.h"
#include "Actions/Specific/AcSearchEnemy.h"
#include "Actions/Specific/AcShoot.h"
#include "Actions/Specific/ACUseDepletable.h"

#include "Attributes/Basic/ABool.h"
#include "Attributes/Basic/AEnum.h"
#include "Attributes/Special/AHealth.h"
#include "Attributes/Special/AHKitsLeft.h"
#include "Attributes/Special/AAmmoInMag.h"
#include "Attributes/Special/AAtNode.h"
#include "Attributes/Special/AEnemyStatus.h"
#include "Attributes/Special/AGrenadesLeft.h"
#include "Attributes/Special/AMagsLeft.h"

#include "Goals/Specific/GTest.h"
#include "Goals/Specific/GKill.h"

#include "Conditions/Special/CEqual.h"
#include "Conditions/Special/CGreater.h"
#include "Conditions/Special/CInSet.h"
#include "Navigation/Navigator.h"


#include "Actions/Performers/IActionPerformer.h"
#include "Actions/Performers/APTest.h"