#include "Strategist.h"
#include "DataBase.h"
#include <algorithm>
#include <numeric>

#include "GoapController.h"

Strategist::Strategist(const DataBase& data) : _data(data)
{
    UGoapController::StrategistPtr	= this;
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
