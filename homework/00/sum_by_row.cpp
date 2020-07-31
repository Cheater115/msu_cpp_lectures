#include <iostream>
#include <cstdlib>
#include <ctime>

#include "timer.h"


int main()
{
    const int row = 1000;
    const int col = 1000;
    int sum = 0;
    int matr[row][col];

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            matr[i][j] = rand() % 100;

    Timer t;
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            sum += matr[i][j];

    std::cout << sum << "\n";

    return 0;
}