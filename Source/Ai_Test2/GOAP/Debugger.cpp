#include "Debugger.h"
#include "GController.h"

const DataBase* Debugger::DataPtr;

void Debugger::PrintValueSet(const ValueSet& vs) const
{
    for (size_t i = 0; i < vs.Size(); i++)
        if (vs.IsAffected(i))
            std::cout << *DataPtr->AttributeCatalogue.GetName(i) << ": " <<
                (*DataPtr->AttributeCatalogue.GetItem(i))->GetEnumeratorString(vs.GetProperty(i)) << "\n";
}

void Debugger::PrintPlan(const Plan& plan) const
{
    for (size_t i = 0; i < plan.ActionNames.size(); i++)
        std::cout << i + 1 << ". " << plan.ActionNames[i] << " " << plan.ActionInstances[i].StringData << "\n";
}

