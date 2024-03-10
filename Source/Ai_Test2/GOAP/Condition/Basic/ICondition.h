#pragma once
#include <memory>
#include "../../Attributes/Basic/IAttribute.h"

#pragma region Macro

#define RESOLVE_I(newCondition) virtual ICondition* Resolve##newCondition(const struct newCondition* c##newCondition) const = 0;	\
    
#define RESOLVE_D(derivedCondition) \
ICondition* Duplicate()  const\
    {\
        return new derivedCondition(*this);\
    }\
ICondition* Resolve(const ICondition* c) const override \
{   \
    return c->Resolve##derivedCondition(this);  \
}   \

#pragma endregion 

struct CEqual;

struct ICondition
{
    ICondition() = default;
    virtual ~ICondition() = default;
    virtual ICondition* Duplicate() const = 0;
    //Evaluate the result of the condition with 'value' as input. If the condition is satisfied, returns 0.0, otherwise the distance (calculated from IAttribute::GetDifference implementation) 
    virtual float Evaluate(t_value value, const IAttribute* attributePtr, const SupplementalData& userData) const = 0;
    
    //Condition resolution

    //A series of methods for combining two conditions into a single 1.
    //Returns a pointer to allocated memory keeping a new condition. Do not override manually!
    virtual ICondition* Resolve(const ICondition* conditionPtr) const = 0;
    
    RESOLVE_I(CEqual)
    RESOLVE_I(CLarger)
    RESOLVE_I(CInSet)

protected:

    friend class Helper;
};

//How to add a new condition:
//1. Make new class derived from ICondition, provide proper constructor
//2. Implement Evaluate method based on your needs
//3. Add macros RESOLVE_I(name_of_your_new_condition_class) to ICondition class body
//4. Add macros RESOLVE_D(name_of_your_new_condition_class) to your new class body
//5. In your new class provide all necessary overrides for methods starting with 'Resolve' (not including 'Resolve' method itself)
//6. In each derived condition provide all necessary overrides for methods starting with 'Resolve' (not including 'Resolve' method itself)