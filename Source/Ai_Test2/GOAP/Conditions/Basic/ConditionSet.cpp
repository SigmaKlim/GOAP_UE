#include "ConditionSet.h"

#include "../../DataBase.h"

const DataBase* ConditionSet::DataPtr;

bool ConditionSet::HasConflicts(const ConditionSet& other) const
{
    for (unsigned i = 0; i < Size(); i++)
        if (IsAffected(i) == true && other.IsAffected(i) == true)
        {
            auto* resolvedConditionPtr = _properties[i]->Resolve(other._properties[i].get());
            if (resolvedConditionPtr == nullptr)
                return true;
            delete resolvedConditionPtr;
        }
    return false;
}

bool ConditionSet::Reduce(const ValueSet& world, ConditionSet& reducedConditionSet, SupplementalData userData) const
{
    reducedConditionSet = *this;
    bool wasAtLeast1ConditionSatisfied = false;
    for (unsigned i = 0; i < Size(); i++)
    {
        if (IsAffected(i) == true && world.IsAffected(i) == true)
        {
            if(_properties[i]->Evaluate(world.GetProperty(i), DataPtr->AttributeCatalogue.GetItem(i)->get(), userData) != 0.0f)
                    return false;
            reducedConditionSet.ClearValue(i);
            wasAtLeast1ConditionSatisfied = true;
        }
    }
    return wasAtLeast1ConditionSatisfied;
}

bool ConditionSet::Merge(const ConditionSet& other, ConditionSet& mergedConditionSet) const
{
    for (unsigned i = 0; i < Size(); i++)
    {
        if (IsAffected(i) == true && other.IsAffected(i) == true)
        {
            const auto* newConditionPtr = _properties[i]->Resolve(other._properties[i].get());
            if (newConditionPtr == nullptr)
                return false;
            mergedConditionSet.SetCondition(i, newConditionPtr);
        }
        else if (other.IsAffected(i) == true)
            mergedConditionSet.SetCondition(i, other._properties[i]->Duplicate());
        else if (IsAffected(i) == true)
            mergedConditionSet.SetCondition(i, _properties[i]->Duplicate());
    }
    return true;
}
