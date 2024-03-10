#include "MathHelper.h"
#include <random>
#include <string>
#include <iostream>
#include <cmath>
#pragma optimize( "", off )
void MathHelper::MakeEmptyMatrix(Matrix& result, u_int dim)
{
	result = Matrix(dim);
	for (size_t i = 0; i < dim; i++)
		result[i] = std::vector<u_int>(dim);
}
void MathHelper::MakeRndMtrx(Matrix& result, u_int dim, u_int lower, u_int upper)
{
	MakeEmptyMatrix(result, dim);
	for (size_t i = 0; i < dim; i++)
	{
		for (size_t j = 0; j < dim; j++)
			result[i][j] = lower + u_int(rand() % upper);
	}
}

void MathHelper::MakeRndIncidenceMatrx(Matrix& result, u_int dim, float discChance)
{
	MakeEmptyMatrix(result, dim);
	int discValue = discChance * 100.0f;
	for (size_t i = 0; i < dim; i++)
	{
		for (size_t j = 0; j < dim; j++)
			result[i][j] = (rand() % 101 <= discValue || i == j) ? INFTY : 1 + u_int(rand() % 100);
	}
}

int MathHelper::PrintMtrxToFile(const Matrix& toPrint,  std::ofstream& fout, unsigned char separator)
{
	if (fout.is_open() == false)
		return -1;
	for (const auto& line : toPrint)
	{
		for (const auto& cell : line)
			fout << cell << separator;
		fout << '\n';
	}
	fout << '\n';
	return 0;
}

int MathHelper::ReadMtrxFromFile(Matrix& result, std::ifstream& fin, /*int startingPos,*/ unsigned char separator)
{
	if (fin.is_open() == false)
		return -1;
	/*fin.seekg(startingPos, std::ios_base::beg);*/
	std::string str;
	std::string nextStr = "x";
	for (fin >> nextStr; fin.eof() != true; fin >> nextStr)
		str.append("\n" + nextStr);
	//std::cout << str;
	int commaPos = 0, nextCommaPos = 0;
	int ctr = 0;
	while (commaPos < str.length())
	{
		nextCommaPos = str.find(separator, commaPos);
		int line = ctr / result.size();
		int row = ctr - line * result.size();
		result[line][row] = stoi(str.substr(commaPos, nextCommaPos - commaPos));
		commaPos = nextCommaPos + 1;
		ctr++;
	}
	return 0;
}

void MathHelper::ToAdjacencyList(Matrix& incidence, Matrix& result)
{
	auto dim = incidence.size();
	result = Matrix(dim*dim, std::vector<u_int>(3)); //from, to, dist
	for (u_int i = 0; i < dim; i++)
		for (u_int j = 0; j < dim; j++)
			result[i * dim + j] = { i, j, incidence[i][j] };		
}

unsigned MathHelper::NumDigits(int number)
{
	int digits = 1;
	int bound = 10;
	number = abs(number);
	while (bound <= number)
	{
		digits++;
		bound *= 10;
	}
	return digits;
}

bool MathHelper::SatisfiesMask(const unsigned& mask_, const unsigned& value_)
{
	return (mask_ & value_) == mask_;
}

bool MathHelper::SatisfiesMask(const unsigned long& mask_, const unsigned long& value_)
{
	return (mask_ & value_) == mask_;
}

bool MathHelper::SatisfiesMask(const unsigned long long& mask_, const unsigned long long& value_)
{
	return (mask_ & value_) == mask_;
}

