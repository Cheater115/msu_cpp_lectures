#include <iostream>
#include "numbers.dat"
// Data - array
// Size - size


int binFindLow(const int* arr, int n, int val)
{
    int left = 0, right = n - 1;
    while (left < right)
    {
        int mid = (left + right) / 2;
        if (arr[mid] > val)
        {
            right = mid - 1;
        }
        else if (arr[mid] < val)
        {
            left = mid + 1;
        }
        else
        {
            right = mid;
        }
    }
    return arr[left] == val ? left : -1;
}


int binFindUp(const int* arr, int n, int val)
{
    int left = 0, right = n - 1;
    while (left < right)
    {
        int mid = (left + right + 1) / 2;
        if (arr[mid] > val)
        {
            right = mid - 1;
        }
        else if (arr[mid] < val)
        {
            left = mid + 1;
        }
        else
        {
            left = mid;
        }
    }
    return arr[left] == val ? left : -1;
}


bool isPrime(int val)
{
    if (val == 0 || val == 1)
        return false;
    if (val == 2 || val == 3)
        return true;
    
    for (int i = 2; i * i <= val; i++)
    {
        if (val % i == 0)
            return false;
    }
    return true;
}


int countPrimes(const int* arr, int li, int ri)
{
    int cnt = 0;
    for (int i = li; i <= ri; i++)
    {
        if (isPrime(arr[i]))
        {
            cnt++;
        }
    }
    return cnt;
}


int main(int argc, char* argv[])
{
    if (argc < 3 || argc % 2 != 1)
        return -1;

    int count_primes = 0;

    for (int i = 2; i < argc; i += 2)
    {
        int left = std::atoi(argv[i - 1]);
        int right = std::atoi(argv[i]);

        int leftInd, rightInd;

        if ((leftInd = binFindLow(Data, Size, left)) == -1
        || (rightInd = binFindUp(Data, Size, right)) == -1)
        {
            std::cout << 0 << std::endl;
        }

        std::cout << countPrimes(Data, leftInd, rightInd) << std::endl;;
    }

    return 0;
}
