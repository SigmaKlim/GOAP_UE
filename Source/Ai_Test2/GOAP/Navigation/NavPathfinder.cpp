#include "NavPathfinder.h"
#include <random>

NavPathfinder::NavPathfinder(const GMatrix& distanceMatrix)
{
    _distanceMatrix = distanceMatrix;
    _numVertices = _distanceMatrix.size();
    _accumDist = 0;
    std::vector<unsigned> longRoads(_distanceMatrix.size());
    for (auto& line : _distanceMatrix)
        for (auto d : line)
            if (d < MathHelper::INFTY / 2)
                for (auto& longRoad : longRoads)
                    if (d > longRoad)
                    {
                        longRoad = d;
                        break;
                    }
    for (auto& longRoad : longRoads)
        _accumDist +=longRoad;
}

void NavPathfinder::GetNeighbors(std::vector<unsigned>& neighbors, std::vector<float>& distances, const unsigned& vertex, const unsigned& finish) const
{
    for (unsigned i = 0; i < _numVertices; i++)
        if (_distanceMatrix[vertex][i] < MathHelper::INFTY / 2)
        {
            neighbors.push_back(i);
            distances.push_back((float)_distanceMatrix[vertex][i]);
        }
}

bool NavPathfinder::Satisfies(const unsigned& vertex, const unsigned& finish) const
{
    return vertex == finish;
}

// unsigned NavPathfinder::GetId(const unsigned& vertex) const
// {
//     return vertex;
// }

float NavPathfinder::GetDistanceDenominator() const
{
    return static_cast<float>(_accumDist);
}

