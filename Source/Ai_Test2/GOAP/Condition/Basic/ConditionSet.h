#pragma once
#include <memory>

#include "ICondition.h"
#include "../../Tools/PropertyList.h"
#include "../../ValueSet.h"


class ConditionSet : public PropertyList<std::shared_ptr<const ICondition>>
{
public:
    ConditionSet(size_t size) : PropertyList<std::shared_ptr<const ICondition>>(size) {}
    template <typename T_Condition>
    void SetCondition(size_t index, const T_Condition* condition);
    //Check if this CS conflicts the other
    bool HasConflicts(const ConditionSet& other) const;
    //Remove all conditions satisfied in 'world', if there are no such conditions, return false
    bool Reduce(const ValueSet& world, ConditionSet& reducedConditionSet, SupplementalData userData) const;
    //Resolve all colliding conditions, if there is a pair of conflicting conditions, return false
    bool Merge(const ConditionSet& other, ConditionSet& mergedConditionSet) const;

private:
    static const Catalogue<IAttribute*>* _attributeCataloguePtr;
    friend class Planner;
};

template <typename T_Condition>
void ConditionSet::SetCondition(size_t index, const T_Condition* condition)
{
    //assert(dynamic_cast<const ICondition*>(condition));
    SetProperty(index, std::shared_ptr<const ICondition>(condition));
}
