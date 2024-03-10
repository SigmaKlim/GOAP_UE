#pragma once
#include <vector>
#include <utility>
#include <queue>
#include <map>
#include <cassert>
#include <string>
#include <iostream>

#include "Tools/MathHelper.h"
#include "Tools/FibonacciHeap.hpp"
#include <unordered_set>
#include <unordered_map>

#define CHECK_DISTANCE_NORMALIZED(value) if ((value) > 1.0f) std::cout << "WARNING: Normalized distance value (" << std::to_string(value) << ") is greater than 1.0f.\n" \
	
#define CHECK_HEURISTICS_NORMALIZED(value) if ((value) > 1.0f) std::cout << "WARNING: Normalized heuristics value (" << std::to_string(value) << ") is greater than 1.0f.\n" \

typedef unsigned long long ull;

template <typename t_vertex>
struct Path
{
	std::vector<t_vertex> Vertices;
	float Cost = 0;
};

//An internal class for constructing path in graph. The algorithm only uses ids to distinguish between nodes.
//Each id is associated with a concrete user-class vertex. User must override protected methods. User himself
//is responsible for dealing with vertex-object internals. 
struct Node
{
	Node(unsigned	vertexId,
		 float		distFromStart,
		 float		heuristic)
		 : VertexId(vertexId), DistFromStart(distFromStart), Heuristic(heuristic) {}
	bool operator<(const Node& right) const
	{
		return DistFromStart + Heuristic < right.DistFromStart + right.Heuristic;
	}
	bool operator>(const Node& right) const
	{
		return DistFromStart + Heuristic > right.DistFromStart + right.Heuristic;
	}
	unsigned	  VertexId;
	float DistFromStart = 0.0f;
	float Heuristic = 0.0f;
};

struct TelemetryData
{
	ull totalBytesUsed = 0;		//total amount of memory used
	unsigned expandedNum = 0;	//number of expanded vertices
	unsigned discoveredNum = 0;	//number of discovered vertices
};

template <typename t_vertex>
struct VertexKey
{
	//YOU HAVE TO EXPLICITLY PROVIDE SPECIALIZATION FOR YOUR VERTEX TYPE
	std::size_t operator()(const t_vertex& k) const
	{
		return 0;
	}
};

template <typename t_vertex>
struct VertexEqual
{
	bool operator()(const t_vertex& lhs, const t_vertex& rhs) const
	{
		return false; //we assume that two vertices, returned by GetNeighbors are always different
	}
};


//Utilizes A* pathfinding algorithm. All protected methods must be overriden in class template specialization.
//Also size_t VertexKey::operator()(const t_vertex& k) must be specialized for your vertex class.
template <typename t_vertex>
class AStarSolver
{
protected:
	virtual float GetHeuristic(const t_vertex& vertex, const t_vertex& finish = t_vertex()) const
	{
		return 0;
	}
	//Returns accessible from 'vertex' neighbors as well as distances to them 
	virtual		void		GetNeighbors	(	std::vector<t_vertex>&	neighbors, std::vector<float>& distances,
												const t_vertex& vertex, const t_vertex& finish = t_vertex()) const = 0;
	//Checks whether this node satisfies conditions of the target node.
	virtual		bool		Satisfies		(const t_vertex& vertex, const t_vertex& finish = t_vertex()) const = 0;
	//virtual		t_id		GetId			(const t_vertex& vertex) const = 0;
	//virtual		float		GetDistance		(const t_vertex& from, const t_vertex& to) const = 0;

	//Returns dDen, so that it is guaranteed that distFromStart / dDen <= 1. Used for normalizing distance.
	virtual float GetDistanceDenominator() const = 0;
	//Returns hDen, so that it is guaranteed that heuristics / hDen <= 1. Used for normalizing heuristics.
	virtual float GetHeuristicsDenominator() const
	{
		return 1;
	}
public:
							AStarSolver	()  = default;
	virtual					~AStarSolver	()  = default;
	bool		Pathfind		(Path<t_vertex>& path, t_vertex start, t_vertex finish = t_vertex(),
				                             TelemetryData* telemetryData = nullptr) const
	{

		FibonacciHeap<Node>								discQueue; //A queue of discovered nodes, used to quickly get min-vertex
		std::unordered_map<unsigned, Element<Node>*>	discovered;	//Used to access elements of queue and check if a node had been discovered (by id)
								
		std::unordered_set<unsigned>					expanded;	//A set of expanded nodes, used to check if a node had been expanded

		std::unordered_map<t_vertex, unsigned,
								VertexKey<t_vertex>, VertexEqual<t_vertex>>			vid;	//Used to map vertex object to id							
		std::vector<const t_vertex*>					idv;	//Used to map node id to vertex object ptr
								
		std::unordered_map <unsigned, unsigned>			cameFrom;	//Used to map node to its preceding node

		unsigned discoveredHeapSize = 0;
		float normalizedDistance = 0.0f;
		float normalizedHeuristics = GetHeuristic(start, finish) / GetHeuristicsDenominator();
		CHECK_HEURISTICS_NORMALIZED(normalizedHeuristics);
		Node currentNode(0, normalizedDistance, normalizedHeuristics);
		idv.push_back(&start);
		cameFrom.insert({ 0, UINT_MAX });
		Element<Node>* currentElement = discQueue.insert(currentNode);
		discoveredHeapSize++;
		discovered.insert({ 0, currentElement });
		if (telemetryData != nullptr)
			++telemetryData->discoveredNum;
				
		std::vector <t_vertex> neighborVertices;
		std::vector<float> distances;
		while (true)
		{
			int discoveredHeapSizeDelta = 0;
			if (discQueue.isEmpty() == true)
			{
				path.Vertices.clear();
				return false;
			}
			currentNode = discQueue.extractMin();
			discoveredHeapSizeDelta--;
			unsigned currentVertexId = currentNode.VertexId;
			discovered.erase(currentVertexId);
			const t_vertex currentVertex = *idv[currentVertexId];
			if (Satisfies(currentVertex, finish) == true)
				break;
			expanded.insert(currentVertexId);
			if (telemetryData != nullptr)
				++telemetryData->expandedNum;
			GetNeighbors(neighborVertices, distances, currentVertex, finish);
			for (size_t i = 0; i < neighborVertices.size(); i++)
			{
				normalizedDistance = distances[i] / GetDistanceDenominator();
				CHECK_DISTANCE_NORMALIZED(normalizedDistance);
				normalizedHeuristics = GetHeuristic(neighborVertices[i], finish) / GetHeuristicsDenominator();
				CHECK_HEURISTICS_NORMALIZED(normalizedHeuristics);
				auto discOrExpIt = vid.find(neighborVertices[i]);
				if (discOrExpIt == vid.end())
				{
					unsigned neighborId = idv.size();
					idv.push_back(&vid.insert({neighborVertices[i], neighborId}).first->first);

					Node neighborNode(	neighborId, 
										currentNode.DistFromStart + normalizedDistance, 
										normalizedHeuristics);
					auto* neighborElement = discQueue.insert({ neighborNode });
					discovered.insert({ neighborId, neighborElement });
					discoveredHeapSizeDelta++;
					assert(cameFrom.insert({neighborId, currentNode.VertexId}).second == true);
				}
				else
				{
					unsigned neighborId = discOrExpIt->second;
					Node neighborNode(	neighborId, 
										currentNode.DistFromStart + normalizedDistance, 
										normalizedHeuristics);
					auto discIt = discovered.find(neighborId);
					if (discIt != discovered.end())
					{
						auto* neighborElement = discIt->second;
						if (neighborElement->getKey().DistFromStart > neighborNode.DistFromStart)
						{
							discQueue.decreaseKey(neighborElement, neighborNode);
							cameFrom.insert_or_assign(neighborId, currentNode.VertexId).second;
						}
					}
				}
			}
			neighborVertices.clear();
			distances.clear();
			if (discoveredHeapSizeDelta > 0)
				discoveredHeapSize += discoveredHeapSizeDelta;
		}
		path.Cost = currentNode.DistFromStart * GetDistanceDenominator();
		unsigned anotherId = currentNode.VertexId;
		while (anotherId != UINT_MAX)
		{
			const t_vertex anotherVertex = *idv[anotherId];
			path.Vertices.push_back(anotherVertex);
			anotherId = cameFrom.at(anotherId);
		}
		for (u_int i = 0; i < path.Vertices.size() / 2; i++)
			std::swap(path.Vertices[i], path.Vertices[path.Vertices.size() - 1 - i]);
		const unsigned DISCOVERED_HEAP_ELEMENT_SIZE = sizeof(decltype(discQueue.getMin()));
		const unsigned DISCOVERED_MAP_ELEMENT_SIZE	= sizeof(decltype(*discovered.begin()));
		const unsigned EXPANDED_ELEMENT_SIZE		= sizeof(decltype(*expanded.begin()));
		const unsigned VERTICES_ELEMENT_SIZE		= sizeof(decltype(*idv.begin()));
		const unsigned CAME_FROM_ELEMENT_SIZE		= sizeof(decltype(*cameFrom.begin()));
		if (telemetryData != nullptr)
			telemetryData->totalBytesUsed	+= discoveredHeapSize * DISCOVERED_HEAP_ELEMENT_SIZE
											+ discovered.size() * DISCOVERED_MAP_ELEMENT_SIZE
											+ expanded.size() * EXPANDED_ELEMENT_SIZE
											+ idv.size() * VERTICES_ELEMENT_SIZE
											+ cameFrom.size() * CAME_FROM_ELEMENT_SIZE;
		return true;
	}
};

