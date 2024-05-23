#pragma once

#include "../../Attributes/Special/AAtNode.h"
#include "../Basic/IAction.h"
#include "../../Attributes/Special/AEnemyStatus.h"
#include "../../Conditions/Special/CEqual.h"

class AcPatrol : public IAction
{
public:
    AcPatrol(size_t iEnemyStatus, size_t iIsCrouching, size_t iAtNode, float cost) :
        _iEnemyStatus(iEnemyStatus), _iIsCrouching(iIsCrouching), _iAtNode(iAtNode), _cost(cost) {}

    void ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actions, const ConditionSet& requiredConditions, const SupplementalData& userData) override;
    ActionInstanceData ConstructActionInstancePosteriori(const ValueSet& prevState, const ActionInstanceData& prioriActionInstance) override;
    float GetMaxCost() const override;
private:
    size_t _iEnemyStatus;
    size_t _iAtNode;
    size_t _iIsCrouching;
    float _cost;
};

inline void AcPatrol::ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actions, const ConditionSet& requiredConditions,
    const SupplementalData& userData)
{
    ConditionSet cs(DataPtr->GetNumAttributes());
    cs.SetCondition(_iEnemyStatus, new CEqual((int)EAVEnemyStatus::eNonVisible));
    cs.SetCondition(_iIsCrouching, new CEqual(false));
    ValueSet vs(DataPtr->GetNumAttributes());
    vs.SetValue(_iEnemyStatus, (int)EAVEnemyStatus::eVisible);
    vs.SetValue(_iAtNode, -1);  //we do not know in advance where the agent will appear when the search is over
    float projectedCost = _cost;
    if (userData.futureGoToDestinationNode != -1)
        projectedCost += _cost - DataPtr->Navigator->GetDistance(userData.initNode, userData.futureGoToDestinationNode, userData.agentPtr);
    //we update cost according to the information that we will arrive to futureGoToDestinationNode not from
    //initNode but from some unknown node.
    SupplementalData newUserData(userData);
    newUserData.futureGoToDestinationNode = -1;
    actions.push_back({cs, vs, projectedCost, newUserData});
}

inline ActionInstanceData AcPatrol::ConstructActionInstancePosteriori(
    const ValueSet& prevState, const ActionInstanceData& prioriActionInstance)
{
    auto posterioriActionInstance = IAction::ConstructActionInstancePosteriori(prevState, prioriActionInstance);
    int trueDepartureNode = prevState.GetValue(_iAtNode); //now we know it
    posterioriActionInstance.Effects.SetValue(_iAtNode, -1);
    ///implement!!!
    posterioriActionInstance.Cost = _cost;
    return posterioriActionInstance;
}

inline float AcPatrol::GetMaxCost() const
{
    return _cost;
}
