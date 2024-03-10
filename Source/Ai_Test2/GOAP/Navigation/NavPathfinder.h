#pragma once
#include "../AStar.h"
#include "../Tools/MathHelper.h"

class NavPathfinder : public AStarSolver<unsigned>
{
public:    
    NavPathfinder(const Matrix& distanceMatrix);
    
private:
    void GetNeighbors(std::vector<unsigned>& neighbors, std::vector<float>& distances, const unsigned& vertex, const unsigned& finish = 0) const override;
    bool Satisfies(const unsigned& vertex, const unsigned& finish = 0) const override;
    float GetDistanceDenominator() const override;
    
    unsigned _numVertices;
    Matrix _distanceMatrix;

    //sum of first N long roads length, where N is number of vertices; used for normalization 
    unsigned _accumDist;
};

