#include "auxiliaryFunctions.h"

bool is_prime(int n)
{
    if (n <= 2)
        return false;
    if (n % 2 == 0)
        return false;
    for (int i = 3; i*i <= n; i += 2)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

int find_next_prime(int lastPrime)
{
    if (lastPrime <= 1)
        return 2;
    if (lastPrime == 2)
        return 3;
    int nextPrime = lastPrime;
    nextPrime += 2;
    if (nextPrime % 2 == 0)
        nextPrime++;
    while (true)
    {
        if (is_prime(nextPrime))
            return nextPrime;
        nextPrime += 2;
    }
}