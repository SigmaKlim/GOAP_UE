#pragma once
#include <vector>
#include <ThirdParty/Eigen/Eigen/Core>

using Eigen::VectorXf;
using Eigen::MatrixXf;

class DataBase;
struct ValueSet;

struct Strategy
{
    std::vector<size_t> GoalIds;
    float Cost = 0.0f;
};


class Strategist
{
public:
    Strategist(const DataBase& data, bool mustUseRegression = false);
    /**
     * \brief Build strategy basing on goal priorities
     * \param priorities priorities[i] is i-th goal priority
     * \param [out]strategy built strategy 
     */
    void ConstructStrategy(const std::vector<float>& priorities, Strategy& strategy) const;
    void ConstructStrategy(const ValueSet& initState, const std::vector<float>& priorities, Strategy& strategy) const;
private:
    float GetPlanCostEstimate(const ValueSet& initState, int goalId) const;
    ValueSet GetResultStateEstimate(const ValueSet& initState, int goalId) const;
    const DataBase& _data;
    std::vector<VectorXf> gwp;
    std::vector<VectorXf> gwb;
    std::vector<MatrixXf> gws;

    const float BETA_MAX = 5.0f;
};
