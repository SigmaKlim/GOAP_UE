#pragma once
#include "DataBase.h"
#include "ValueSet.h"

struct Plan;
class GController;

class Debugger
{
public:
    Debugger() = default;
    void PrintValueSet(const ValueSet& vs) const;
    void PrintPlan(const Plan& plan) const;
    
    static const DataBase* DataPtr;

    friend class GController;
};
