#pragma once
#include "Tools/Catalogue.h"
#include "Tools/PropertyList.h"

struct ValueSet : PropertyList<int>
{
    ValueSet(size_t size) : PropertyList(size) {}
    void SetValue(size_t index, int value)
    {
        SetProperty(index, value);
    }
};