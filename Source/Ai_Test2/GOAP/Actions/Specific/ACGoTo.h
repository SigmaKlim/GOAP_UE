#pragma once
#include "../Basic/IAction.h"
#include "../../Attributes/Special/AAtNode.h"
#include "../../Conditions/Special/CEqual.h"
#include "../../Conditions/Special/CInSet.h"
#include "../../Attributes/Special/AEnemyStatus.h"

//In order to use, AAtNode must anly be under InSet condition
class AcGoTo : public IAction
{
public:
    AcGoTo()
    {
        _iAtNode        = DataPtr->GetAttributeId("atNode");
        _iEnemyStatus   = DataPtr->GetAttributeId("enemyStatus");
        _iIsCrouching   = DataPtr->GetAttributeId("isCrouching");
    }
    void ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actions, const ConditionSet& requiredConditions, const SupplementalData& userData) override;
    ActionInstanceData ConstructActionInstancePosteriori(const ValueSet& prevState, const ActionInstanceData& prioriActionInstance) override;
    float GetMaxCost() const override;

private:
    size_t _iAtNode;
    size_t _iIsCrouching;
    size_t _iEnemyStatus;
};

inline void AcGoTo::ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actions, const ConditionSet& requiredConditions,
    const SupplementalData& userData)
{
    if (requiredConditions.IsAffected(_iAtNode) == true)
    {
        std::vector<int> destinations;
        auto* inSet = static_cast<const CInSet*>(requiredConditions.GetProperty(_iAtNode).get());
        for (auto& destination : inSet->Values)
            destinations.push_back(destination);
        for (auto& destinationNode : destinations)
        {
            ConditionSet cs(DataPtr->GetNumAttributes());
            cs.SetCondition(_iIsCrouching, new CEqual(false)); //set isCrouching condition equal to false
            ValueSet vs(DataPtr->GetNumAttributes());                                      
            vs.SetValue(_iAtNode, destinationNode);            //pick atNode attribute value from required conditions and set its effect accordingly
            vs.SetValue(_iEnemyStatus, EAVEnemyStatus::eNonVisible);
            float newCost = DataPtr->Navigator.GetDistance(userData.initNode, destinationNode);
            if (userData.futureGoToDestinationNode != -1)
            {
                newCost +=  DataPtr->Navigator.GetDistance(destinationNode, userData.futureGoToDestinationNode) -
                            DataPtr->Navigator.GetDistance(userData.futureGoToDestinationNode, userData.initNode); //compensate our previous lack of knowledge of future GoTo departure point
            }
            SupplementalData newData(userData);
            newData.futureGoToDestinationNode = destinationNode;
            std::string stringData = "-> " + std::to_string(destinationNode) + ";";
            ActionInstanceData action(cs, vs, newCost, newData, stringData);//calculate cost of movement via navigator
            actions.push_back(action);
        }
    }
}

inline ActionInstanceData AcGoTo::ConstructActionInstancePosteriori(
    const ValueSet& prevState, const ActionInstanceData& prioriActionInstance)
{
    auto posterioriActionInstance = IAction::ConstructActionInstancePosteriori(prevState, prioriActionInstance);
    int destinationNode = prioriActionInstance.Effects.GetProperty(_iAtNode);
    int trueDepartureNode = prevState.GetProperty(_iAtNode); //now we know it
    posterioriActionInstance.Cost = DataPtr->Navigator.GetDistance(trueDepartureNode, destinationNode); //true cost of goto action
    return posterioriActionInstance;
}


inline float AcGoTo::GetMaxCost() const
{
    return DataPtr->Navigator.GetMaxDistance();
}
