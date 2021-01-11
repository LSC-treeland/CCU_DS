#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <string.h>
#include <math.h>

using namespace std;

void init(bool **bits, int m, int r, int **a, int **b, int p)
{
    //1. Create an array with m bits for bits
    *bits = (bool *)malloc(sizeof(bool) * m);
    //2. Create an array with r elements uniformly chosen from [1, p-1] for pointer a using srand(1)
    srand(1);
    *a = (int *)malloc(sizeof(int) * r);
    for (int i = 0; i < r; i++)
    {
        *(*a + i) = rand() % (p - 1) + 1;
    }
    //3. Create an array with r elements uniformly chosen from [1, p-1] for pointer b using srand(2) (note: a[i] and b[i] should be independent)
    srand(2);
    *b = (int *)malloc(sizeof(int) * r);
    for (int i = 0; i < r; i++)
    {
        *(*b + i) = rand() % (p - 1) + 1;
    }
}

int myhash(char *str, int count, int m, int r, int p, int *a, int *b)
{
    //1. Use hash in <string> to covert str to an integer key
    // You may use class string and class hash <string>
    // note that 0 <= count <= r-1
    hash<string> hash_function;
    const size_t key = hash_function(str);

    //2. Return (a[count] * key + b[count]) % p  % m;
    return (a[count] * key + b[count]) % p % m;
}

void insert(bool *bits, int m, int r, int p, char *str, int *a, int *b)
{
    //1. Find all the mapped bits in the following positions,bits[myhash(str, count, m, r, p, a, b)] for 0 ≤ count ≤ r-1
    for (int count = 0; count < r; count++)
    {
        *(bits + myhash(str, count, m, r, p, a, b)) = true;
    }
    //2. Set all of the mapped bits above to true
}

bool query(bool *bits, int m, int r, int p, char *str, int *a, int *b)
{
    //1. Examine whether all the mapped bits are true bits[myhash(str, count, m, r, p, a, b)] for 0 ≤ count ≤ r-1 for 0 ≤ count ≤ r-1
    for (int count = 0; count < r; count++)
    {
        if (*(bits + myhash(str, count, m, r, p, a, b)) == false)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    int bits, hash_function, words, tests, prime;
    scanf("%d %d %d %d %d", &bits, &hash_function, &words, &tests, &prime);
    char *line = (char *)malloc(sizeof(char) * 128);
    char **test_case = (char **)malloc(sizeof(char *) * tests);
    bool *bits_array;
    int *a, *b;
    init(&bits_array, bits, hash_function, &a, &b, prime);

    for (int i = 0; i < words; i++)
    {
        scanf("%s", line);
        insert(bits_array, bits, hash_function, prime, line, a, b);
    }

    for (int i = 0; i < tests; i++)
    {
        scanf("%s", line);
        *(test_case + i) = strdup(line);
    }

    for (int i = 0; i < tests; i++)
    {
        if (query(bits_array, bits, hash_function, prime, *(test_case + i), a, b))
        {
            printf("%s: true\n", *(test_case + i));
        }
        else
            printf("%s: false\n", *(test_case + i));
    }
}