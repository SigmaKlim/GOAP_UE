#pragma once
#include <vector>
#include <fstream>

typedef unsigned int u_int;
typedef std::vector <std::vector<u_int>> Matrix;



class MathHelper
{
public:
	static const unsigned INFTY = UINT_MAX;
	static void MakeEmptyMatrix(Matrix& result, u_int dim);
	static void MakeRndMtrx(Matrix& result, u_int dim, u_int lower, u_int upper);
	static void MakeRndIncidenceMatrx(Matrix& result, u_int dim, float discChance = 0.0f);
	static int PrintMtrxToFile(const Matrix& toPrint, std::ofstream& fout, unsigned char separator = ' ');
	static int ReadMtrxFromFile(Matrix& result, std::ifstream& fin,/* int startingPos,*/ unsigned char separator = ',');
	static void ToAdjacencyList(Matrix& incidence, Matrix& result);
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
	};

