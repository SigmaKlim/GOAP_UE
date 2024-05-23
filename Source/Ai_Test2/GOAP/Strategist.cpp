#include "Strategist.h"
#include "ValueSet.h"

#include "DataBase.h"
#include <algorithm>
#include <numeric>

#include "GoapController.h"

Strategist::Strategist(const DataBase& data, bool mustUseRegression) : _data(data)
{
    UGoapController::StrategistPtr	= this;
}

void Strategist::ConstructStrategy(const ValueSet& initState, const std::vector<float>& priorities, Strategy& strategy) const
{
    strategy.GoalIds = std::vector<size_t>(_data.GoalCatalogue.Size());
    std::iota(strategy.GoalIds.begin(), strategy.GoalIds.end(), 0);
    std::vector<size_t> bestStartegy(_data.GoalCatalogue.Size());
    float bestCost = MAX_FLT;
    do
    {
        auto nextInitState = initState;
        float strategyCost = 0.0f;
        for (auto& goalId : strategy.GoalIds)
        {
            strategyCost += GetPlanCostEstimate(nextInitState, goalId) + 
                (*_data.GoalCatalogue.GetItem(goalId))->GetTardiness(priorities[goalId], goalId);
            nextInitState = GetResultStateEstimate(nextInitState, goalId);
        }
        if (strategyCost < bestCost)
        {
            bestCost = strategyCost;
            bestStartegy = strategy.GoalIds;
        }
    } while (std::next_permutation(strategy.GoalIds.begin(), strategy.GoalIds.end()));
    strategy.GoalIds = bestStartegy;
    strategy.Cost = bestCost;
}
void Strategist::ConstructStrategy(const std::vector<float>& priorities, Strategy& strategy) const
{
    strategy.GoalIds = std::vector<size_t>(_data.GoalCatalogue.Size());
    std::iota(strategy.GoalIds.begin(), strategy.GoalIds.end(), 0);
    std::sort(strategy.GoalIds.begin(), strategy.GoalIds.end(), [&priorities](size_t i1, size_t i2)
    {
        return priorities[i1] > priorities[i2];
    });
}

float Strategist::GetPlanCostEstimate(const ValueSet& initState, int goalId) const
{
    float p = ((RowVectorXf)initState * gwp[goalId])[0];
    float b = ((RowVectorXf)initState * gwb[goalId])[0];
    float beta = (1 - BETA_MAX) * b + BETA_MAX;
    return p * beta;
}

ValueSet Strategist::GetResultStateEstimate(const ValueSet& initState, int goalId) const
{
    RowVectorXf sv = (RowVectorXf)initState * gws[goalId];
    return ValueSet(sv);
}
