#include <iostream>
#include "src/LibAlg.h"

// Initialize a 10 element array
LA::FloatArray A = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
// Initialize a 2x2 matrix
LA::FloatMatrix B = {{1, 2},
                     {3, 4}};
// Initialize a 3x3 matrix
LA::FloatMatrix C = {{1, 2, 3},
                     {4, 5, 6},
                     {7, 8, 9}};
// Initialize a 3x5 matrix
LA::FloatMatrix D = {{1, 2, 3, 4, 5},
                     {6, 7, 8, 9, 10},
                     {11, 12, 13, 14, 15}};
// Initialize a 4x2 matrix
LA::FloatMatrix E = {{1, 2},
                     {3, 4},
                     {5, 6},
                     {7, 8}};

template <class ElemT>
void printArray(LA::Array<ElemT> &A)
{
    for (int i = 0; i < A.size; i++)
    {
        std::cout << A(i) << " ";
    }
    std::cout << std::endl;
}

template<class DataT>
void printMatrix(LA::Matrix<DataT> &A)
{
    for (int i = 0; i < A.rows; i++)
    {
        for (int j = 0; j < A.cols; j++)
        {
            std::cout << A(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    std::cout << "A: " << std::endl;
    printArray(A);
    std::cout << std::endl;

    std::cout << "B: " << std::endl;
    printMatrix(B);
    std::cout << std::endl;

    std::cout << "C: " << std::endl;
    printMatrix(C);
    std::cout << std::endl;

    std::cout << "D: " << std::endl;
    printMatrix(D);
    std::cout << std::endl;

    std::cout << "E: " << std::endl;
    printMatrix(E);
    std::cout << std::endl;

    E = {{1, 2}, {3, 4}};
    std::cout << "Modified E: " << std::endl;
    printMatrix(E);

    return 0;
}