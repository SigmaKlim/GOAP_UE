#pragma once
#include "ACUseDepletable.h"
#include "../../DataBase.h"
#include "../../Conditions/Special/CInSet.h"

class ACReloadFromCover : public ACUseDepletable
{
public:
    ACReloadFromCover(size_t iNumDepletables, size_t iAffectedAttribute, size_t iAtNode, size_t iIsCrouching, t_value delta, float cost) :
        ACUseDepletable(iNumDepletables, iAffectedAttribute, delta, cost, "magazine", "ammo"), _iAtNode(iAtNode), _iIsCrouching(iIsCrouching)  {}

    void ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actions, const ConditionSet& requiredConditions, const SupplementalData& userData) override
    {
        ACUseDepletable::ConstructActionInstancesPriori(actions, requiredConditions, userData);
        if (actions.empty() != true)
        {
            actions[0].Conditions.SetCondition(_iAtNode, new CInSet(DataPtr->Navigator->GetNodesByTag(EInterestTag::eCover)));
            actions[0].Conditions.SetCondition(_iIsCrouching, new CEqual(true));
        }
    }
private:
    size_t _iAtNode;
    size_t _iIsCrouching;
};