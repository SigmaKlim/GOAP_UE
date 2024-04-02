#pragma once
#include "Tools/Catalogue.h"
#include "Tools/PropertyList.h"

struct ValueSet : PropertyList<int>
{
    ValueSet() : PropertyList() {};
    ValueSet(size_t size) : PropertyList(size) {}
    
    void Modify(const ValueSet& affectingValueSet)
    {
        MY_ASSERT(Size() == affectingValueSet.Size());
        for (size_t i = 0; i < Size(); i++)
            if (affectingValueSet.IsAffected(i))
                SetValue(i, affectingValueSet.GetProperty(i));
    }
    void SetValue(size_t index, int value)
    {
        SetProperty(index, value);
    }
    int GetValue(size_t index) const
    {
        return GetProperty(index);
    }
};