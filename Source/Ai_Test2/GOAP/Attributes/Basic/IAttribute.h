#pragma once
#include <memory>
#include <string>
#include "../../SupplementalData.h"
#include "../../Tools/AssertMacro.h"

class DataBase;
typedef int t_value;
struct ICondition;

class IAttribute
{
public:
    IAttribute() = default;
    virtual ~IAttribute() = default;

    //Returns non-normalized difference between this attribute value and 'value'. Used to calculate World State heuristics
    virtual float GetDifference(t_value value1, t_value value2, const SupplementalData& userData) const = 0;
    //Returns maximal possible difference between any two values of this attribute. Used to normalize difference.
    virtual float GetMaxDifference() const = 0;
    virtual std::string GetEnumeratorString(t_value value) const = 0;
    //num is total previous number of values in series
    virtual float MakeMeanString(t_value newVal, int num, std::string& stringData) const = 0;

    static const DataBase* DataPtr;
};