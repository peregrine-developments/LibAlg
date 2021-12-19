#include <iostream>
#include "src/LibAlg.h"

LA::FloatArray A = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
LA::FloatMatrix B = {{1, 2},
                     {3, 4}};

int main()
{
    std::cout << A.rows << std::endl;

    for(int i = 0; i < A.rows; i++)
    {
        std::cout << A(i) << std::endl;
    }

    std::cout << B.rows << std::endl;
    std::cout << B.cols << std::endl;

    for(int i = 0; i < B.rows; i++)
    {
        for(int j = 0; j < B.cols; j++)
        {
            std::cout << B(i, j) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}