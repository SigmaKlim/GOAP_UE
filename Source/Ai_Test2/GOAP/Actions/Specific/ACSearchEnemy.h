#pragma once

#include "../../Attributes/Special/AAtNode.h"
#include "../Basic/IActionConstructor.h"
#include "../../Attributes/Special/Enums/EAVEnemyStatus.h"
#include "../../Condition/Special/CEqual.h"

class ACSearchEnemy : public IActionConstructor
{
public:
    ACSearchEnemy(size_t iEnemyStatus, size_t iAtNode, size_t iIsCrouching, float cost) :
    _iEnemyStatus(iEnemyStatus), _iAtNode(iAtNode), _iIsCrouching(iIsCrouching), _cost(cost) {}

    void ConstructActions(std::vector<Action>& actions, const ConditionSet& requiredConditions, const SupplementalData& userData) override;
    float GetMaxCost() const override;
private:
    size_t _iEnemyStatus;
    size_t _iAtNode;
    size_t _iIsCrouching;
    float _cost;
};

inline void ACSearchEnemy::ConstructActions(std::vector<Action>& actions, const ConditionSet& requiredConditions,
    const SupplementalData& userData)
{
    ConditionSet cs(numAttributes);
    cs.SetCondition(_iEnemyStatus, new CEqual(EAVEnemyStatus::eNonVisible));
    cs.SetCondition(_iIsCrouching, new CEqual(false));
    ValueSet vs(numAttributes);
    vs.SetValue(_iEnemyStatus, EAVEnemyStatus::eVisible);
    vs.SetValue(_iAtNode, -1);  //we do not know in advance where the agent will appear when the search is over
    float projectedCost = _cost;
    if (userData.futureGoToDestinationNode != -1)
        projectedCost += _cost - AAtNode::navigator.GetDistance(userData.initNode, userData.futureGoToDestinationNode);
    //we update cost according to the information that we will arrive to futureGoToDestinationNode not from
    //initNode but from some unknown node.
    SupplementalData newUserData(userData);
    newUserData.futureGoToDestinationNode = -1;
    actions.push_back({cs, vs, projectedCost, newUserData});
}

inline float ACSearchEnemy::GetMaxCost() const
{
    return _cost * 2;
}
