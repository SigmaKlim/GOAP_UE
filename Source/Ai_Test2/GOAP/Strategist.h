#pragma once
#include <vector>

class DataBase;

struct Strategy
{
    std::vector<size_t> GoalIds;
};


class Strategist
{
public:
    Strategist(const DataBase& data);
    /**
     * \brief Build strategy basing on goal priorities
     * \param priorities priorities[i] is i-th goal priority
     * \param [out]strategy built strategy 
     */
    void ConstructStrategy(const std::vector<float>& priorities, Strategy& strategy) const;
private:
    const DataBase& _data;
};
