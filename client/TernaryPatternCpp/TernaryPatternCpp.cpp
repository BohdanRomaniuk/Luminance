#include <iostream>
#include <vector>

const int patternBase = 3;
const int length = 8;
const int count = 100;
std::vector<int*> lst;

const int* zeroOne = new int[2] { 0, 1};
const int* oneTwo = new int[2] { 1, 2};
const int* zeroTwo = new int[2] { 0, 2};

int ternaryToDecimal(int* ternary)
{
	int sum = 0;
	for (int i = 0; i < length; ++i)
	{
		sum += ternary[length - 1 - i] * pow(patternBase, i);
	}

	return sum;
}

void print(int index, int* arr)
{
	std::cout << index << "\t";
	for (int i = 0; i < length; ++i)
	{
		std::cout << arr[i];
	}
	std::cout << "\t" << ternaryToDecimal(arr) << std::endl;
}

int randIndex()
{
	return rand() % 2;
}

int generateSingle(int upperBit)
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

int generateMulti(int upperBir, int prevBit)
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

int* generateNext(int* prev)
{
	int* result = new int[length];
	result[0] = generateSingle(prev[0]);
	for (int i = 1; i < length; ++i)
	{
		result[i] = generateMulti(prev[i], result[i - 1]);
	}

	return result;
}

bool areEquals(int* arr1, int* arr2)
{
	for (int i = 0; i < length; ++i)
	{
		if (arr1[i] != arr2[i])
		{
			return false;
		}
	}

	return true;
}

bool alreadyExists(int* arr)
{
	for (int* item : lst)
	{
		if (areEquals(arr, item))
		{
			return true;
		}
	}

	return false;
}

int main()
{
	srand(time(NULL));
	int* initial = new int[length];
	for (int i = 0; i < length; ++i)
	{
		initial[i] = i % patternBase;
	}
	lst.push_back(initial);
	print(0, initial);

	int failCount = 0;

	for (int i = 1; i < count; ++i)
	{
		int* next = generateNext(initial);
		if (alreadyExists(next))
		{
			--i;
			++failCount;
			continue;
		}

		lst.push_back(next);
		initial = next;
		print(i, initial);
	}

	std::cout << "Failed count: " << failCount << std::endl;
}