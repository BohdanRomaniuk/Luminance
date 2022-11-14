#include "ternary_pattern.h"
#include <math.h>

TernaryPattern::TernaryPattern(const int ledCount)
	: _ledCount(ledCount)
{
	_patternLength = ledCount <= 150 ? 8 : ledCount <= 850 ? 10 : 12;
	_pattern = new int* [ledCount];
	_generatedCount = 0;
	generatePattern();
}

TernaryPattern::~TernaryPattern()
{
	for (int i = 0; i < _ledCount; ++i)
	{
		if (_pattern[i] != nullptr)
		{
			delete[] _pattern[i];
		}
	}
	delete[] _pattern;
}

void TernaryPattern::generatePattern()
{
	//Initial row 0 1 2 0 1 2 0 1 2 ....
	_pattern[0] = new int[_patternLength];
	for (int i = 0; i < _patternLength; ++i)
	{
		_pattern[0][i] = i % _patternBase;
	}
	print(0, _pattern[0]);
	int failCount = 0;

	for (int i = 1; i < _ledCount; ++i)
	{
		int* pattern = generateNext(_pattern[i - 1]);
		if (alreadyExists(pattern))
		{
			delete[] pattern;
			--i;
			++failCount;
			continue;
		}

		++_generatedCount;
		_pattern[i] = pattern;
		print(i, _pattern[i]);
	}

	std::cout << "Failed count: " << failCount << std::endl;
}

int* TernaryPattern::generateNext(const int* prev)
{
	int* result = new int[_patternLength];
	result[0] = generateSingle(prev[0]);
	for (int i = 1; i < _patternLength; ++i)
	{
		result[i] = generateMulti(prev[i], result[i - 1]);
	}

	return result;
}

int TernaryPattern::randIndex()
{
	return rand() % 2; // TODO: Check how to do that in ESP-32
}

int TernaryPattern::generateSingle(int upperBit)
{
	if (upperBit == 0)
	{
		return oneTwo[randIndex()];
	}
	else if (upperBit == 1)
	{
		return zeroTwo[randIndex()];
	}

	return zeroOne[randIndex()];
}

int TernaryPattern::generateMulti(int upperBir, int prevBit)
{
	if ((upperBir == 1 && prevBit == 2)
		|| (prevBit == 1 && upperBir == 2))
	{
		return 0;
	}
	else if ((upperBir == 0 && prevBit == 2)
		|| (prevBit == 0 && upperBir == 2))
	{
		return 1;
	}
	else if ((upperBir == 0 && prevBit == 1)
		|| (prevBit == 0 && upperBir == 1))
	{
		return 2;
	}
	else if (upperBir == 0 && prevBit == 0)
	{
		return oneTwo[randIndex()];
	}
	else if (upperBir == 1 && prevBit == 1)
	{
		return zeroTwo[randIndex()];
	}

	return zeroOne[randIndex()];
}

bool TernaryPattern::alreadyExists(int* arr)
{
	for (int i = 0; i < _generatedCount; ++i)
	{
		if (patternsAreEquals(arr, _pattern[i]))
		{
			return true;
		}
	}

	return false;
}

bool TernaryPattern::patternsAreEquals(int* arr1, int* arr2)
{
	for (int i = 0; i < _patternLength; ++i)
	{
		if (arr1[i] != arr2[i])
		{
			return false;
		}
	}

	return true;
}

int TernaryPattern::ternaryToDecimal(const int* ternary)
{
	int sum = 0;
	for (int i = 0; i < _patternLength; ++i)
	{
		sum += ternary[_patternLength - 1 - i] * pow(_patternBase, i);
	}

	return sum;
}

void TernaryPattern::print(int index, int* arr)
{
	std::cout << index << "\t";
	for (int i = 0; i < _patternLength; ++i)
	{
		std::cout << arr[i];
	}
	std::cout << "\t" << ternaryToDecimal(arr) << std::endl;
}