const int patternSize = 10;
const int count = 100;
var prev = new int[patternSize] { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0 };
var next = new int[patternSize];
var random = new Random();

print(prev);
for (int i = 1; i < count; ++i)
{
    for (int j = 0; j < patternSize; ++j)
    {
        next[j] = generateNextPattern(prev[j]);
        prev[j] = next[j];
    }
    print(prev);
}

int generateNextPattern(int number)
{
    var rndNumber = random.Next(0, 3);

    return rndNumber == number ? generateNextPattern(number) : rndNumber;
}

void print(int[] array)
{
    for (int i = 0; i < array.Length; ++i)
    {
        Console.Write(array[i]);
    }
    Console.WriteLine();
}
