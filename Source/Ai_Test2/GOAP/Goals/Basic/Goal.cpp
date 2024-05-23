#include "Goal.h"
#include "../../DataBase.h"

const DataBase* Goal::DataPtr;
Goal::Goal() : _conditions(DataPtr->GetNumAttributes())
{

}

ValueSet Goal::OnGoalCompleted(const ValueSet& resultState)
{
    auto overridenState(resultState);
    auto iAtNode = DataPtr->GetAttributeId("AAtNode");
    if (resultState.GetValue(iAtNode) == -1) //-1 -> position unknown
        overridenState.SetValue(iAtNode, 0); // 0 -> position is retrieved from GetWorldLocation
    return overridenState;
}

ValueSet Goal::OnGoalTaken(const ValueSet& resultState)
{
    auto overridenState(resultState);
    auto iAtNode = DataPtr->GetAttributeId("AAtNode");
    if (resultState.GetValue(iAtNode) == -1) //-1 -> position unknown
        overridenState.SetValue(iAtNode, 0); // 0 -> position is retrieved from GetWorldLocation
    return overridenState;
}