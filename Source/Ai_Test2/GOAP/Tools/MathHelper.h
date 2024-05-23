#pragma once
#include <vector>
#include <fstream>

typedef unsigned int u_int;
typedef std::vector <std::vector<u_int>> GMatrix;



class MathHelper
{
public:
	static const unsigned INFTY = UINT_MAX;
	static void MakeEmptyMatrix(GMatrix& result, u_int dim);
	static void MakeRndMtrx(GMatrix& result, u_int dim, u_int lower, u_int upper);
	static void MakeRndIncidenceMatrx(GMatrix& result, u_int dim, float discChance = 0.0f);
	static int PrintMtrxToFile(const GMatrix& toPrint, std::ofstream& fout, unsigned char separator = ' ');
	static int ReadMtrxFromFile(GMatrix& result, std::ifstream& fin,/* int startingPos,*/ unsigned char separator = ',');
	static void ToAdjacencyList(GMatrix& incidence, GMatrix& result);
	static unsigned NumDigits(int number);
	static bool SatisfiesMask(const unsigned& mask_, const unsigned& value_);
	static bool SatisfiesMask(const unsigned long& mask_, const unsigned long& value_);
	static bool SatisfiesMask(const unsigned long long& mask_, const unsigned long long& value_);
	template <typename T>
	static void CombineHash(size_t& seed, const T& v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
	}
	static float UpdateMean(float prevMean, float newValue, unsigned n)
	{
		return (prevMean * n + newValue) / (n + 1);
	}
	};

