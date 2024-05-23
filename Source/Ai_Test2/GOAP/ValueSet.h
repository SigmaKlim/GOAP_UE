#pragma once
#include "Tools/Catalogue.h"
#include "Tools/PropertyList.h"
#include <ThirdParty/Eigen/Eigen/Core>

using Eigen::RowVectorXf;

struct ValueSet : PropertyList<int>
{
    ValueSet() : PropertyList() {};
    ValueSet(size_t size) : PropertyList(size) {}
    ValueSet(const RowVectorXf& vector) : PropertyList(vector.cols())
    {
        for (int i = 0; i < vector.cols(); i++)
            SetValue(i, vector[i]);
    }
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

    operator RowVectorXf() const
    {
        RowVectorXf s(Size());
        for (int i = 0; i < Size(); i++)
            s[i] = GetValue(i);
        return s;
    }
};