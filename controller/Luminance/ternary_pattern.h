#ifndef TernaryPatternHeader
#define TernaryPatternHeader

class TernaryPattern
{
private:
	int _ledCount;
	int _generatedCount;
	int _patternLength;
	int** _pattern;
  int* _patternDecimal;
	const int _patternBase = 3;
	const int* zeroOne = new int[2] { 0, 1 };
	const int* oneTwo = new int[2] { 1, 2 };
	const int* zeroTwo = new int[2] { 0, 2 };

	void generatePattern();
	int* generateNext(const int* prev);
	int ternaryToDecimal(const int* ternary);
	int randIndex();
	int generateSingle(int upperBit);
	int generateMulti(int upperBir, int prevBit);
	bool alreadyExists(int* arr);
	bool patternsAreEquals(int* arr1, int* arr2);

	void print(int index, int* arr);
public:
	TernaryPattern(const int ledCount);
	~TernaryPattern();

  int* getPatternDecimal();
  int getFrameColor(int ledNum, int position);
};

#endif