var length = 8; // 8 - if count <=300, 9 - (300-600), 10 - >= 600
var random = new Random();
var count = 100;
var list = new List<int[]>();
var failCount = 0;
for (int i = 0; i < count; ++i)
{
    var item = generateRandom();
    if (!isUnique(item) || (list.Any() && (!areDifferent(list.Last(), item) || list.Any(x => areEquals(x, item)))))
    {
        --i;
        ++failCount;
        continue;
    }
    Console.Write($"{i} ");
    print(item);
    list.Add(item);
}
Console.WriteLine($"Failed count: {failCount}");

int[] generateRandom()
{
    var result = new int[length];
    for (int i = 0; i < length; ++i)
    {
        result[i] = random.Next(0, 3);
    }

    return result;
}

bool areEquals(int[] a, int[] b)
{
    for (int i = 0; i < length; ++i)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }

    return true;
}

bool areDifferent(int[] a, int[] b)
{
    for (int i = 0; i < length; ++i)
    {
        if (a[i] == b[i])
        {
            return false;
        }
    }

    return true;
}

bool isUnique(int[] a)
{
    for (int i = 1; i < a.Length; ++i)
    {
        if (a[i - 1] == a[i])
        {
            return false;
        }
    }

    return true;
}

void print(int[] array)
{
    for (int i = 0; i < array.Length; ++i)
    {
        Console.Write(array[i]);
    }
    Console.Write($"   {binaryToDecimal(array)}");
    Console.WriteLine();
}

int binaryToDecimal(int[] array)
{
    var sum = 0;
    for (int i = 0; i < length; ++i)
    {
        sum += array[length - 1 - i]*(int)Math.Pow(3, i);
    }

    return sum;
}