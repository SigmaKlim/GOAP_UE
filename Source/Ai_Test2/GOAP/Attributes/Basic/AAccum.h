#pragma once
//
// #include "AInt.h"
// #include "../../Condition/Special/CLarger.h"
// //A special subclass of EInt which is used for attributes which can be increased/ decreased by delta (health, ammo, number of health kits).
// //Only supports CLarger condition
// class AAccum : public AInt
// {
//     AAccum(t_value maxValue) : AInt(maxValue) {}
//
//     bool CheckAllowedConditions(const ICondition* cPtr) override
//     {
//         return dynamic_cast<const CLarger*>(cPtr) != nullptr;
//     }
// };