#pragma once

namespace LA
{

// Array class for element storage
template<class ElemT = float>
class Array
{
public:
    typedef ElemT elem_t;
    elem_t* mem;

    int rows, cols;
    int size;

    Array(int _rows, int _cols = 1)
    {
        rows = _rows;
        cols = _cols;
        size = rows * cols;
        mem = new elem_t[size];
    };
    Array(elem_t* starting_data, int _rows, int _cols = 1)
    {
        rows = _rows;
        cols = _cols;
        size = rows * cols;
        mem = new elem_t[size];

        for (int i = 0; i < size; i++)
        {
            mem[i] = starting_data[i];
        }
    };
    Array(const Array<elem_t> &rhs)
    {
        rows = rhs.rows;
        cols = rhs.cols;
        size = rows * cols;
        mem = new elem_t[size];

        for (int i = 0; i < size; i++)
        {
            mem[i] = rhs.mem[i];
        }
    }
    ~Array() { delete[] mem; }

    Array<elem_t> & operator=(const Array<elem_t> &rhs)
    {
        if (this != &rhs)
        {
            rows = rhs.rows;
            cols = rhs.cols;
            size = rows * cols;
            elem_t* tempMem = new elem_t[size];

            for (int i = 0; i < size; i++)
            {
                tempMem[i] = rhs.mem[i];
            }

            delete[] mem;
            mem = tempMem;
        }

        return *this;
    }
    Array<elem_t> & operator=(const elem_t *arr)
    {
        for (int i = 0; i < size; i++)
        {
            mem[i] = arr[i];
        }

        return *this;
    }

    elem_t & operator()(int row, int col = 0) { return mem[row * cols + col]; };
    elem_t operator()(int row, int col = 0) const { return mem[row * cols + col]; };
};

template<class MemT = Array<float>>
class Matrix
{
public:
    typedef MemT mem_t;
    typedef typename MemT::elem_t data_t;
    mem_t mem;

    int rows, cols;

    Matrix(int _rows, int _cols) : mem(_rows, _cols), rows(_rows), cols(_cols) {};

    Matrix<mem_t> & operator=(const data_t *arr)
    {
        mem = arr;

        return *this;
    }

    data_t &operator()(int row, int col) { return mem(row, col); };

    Matrix<mem_t> & operator+=(const Matrix<mem_t> &rhs)
    {
        if (rows != rhs.rows || cols != rhs.cols) return *this; // Addition requires same size matrices

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                mem(r, c) += rhs.mem(r, c);
            }
        }

        return *this;
    };
    Matrix<mem_t> & operator-=(const Matrix<mem_t> &rhs)
    {
        if (rows != rhs.rows || cols != rhs.cols) return *this; // Subtraction requires same size matrices

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                mem(r, c) -= rhs.mem(r, c);
            }
        }

        return *this;
    };
    Matrix<mem_t> & operator*=(const data_t &rhs)
    {
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                mem(r, c) *= rhs;
            }
        }

        return *this;
    }
    Matrix<mem_t> & operator/=(const data_t &rhs)
    {
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                mem(r, c) /= rhs;
            }
        }

        return *this;
    }

    const Matrix<mem_t> operator+(const Matrix<mem_t> &rhs) const
    {
        Matrix<mem_t> ret = *this;
        ret += rhs;
        return ret;
    };
    const Matrix<mem_t> operator-(const Matrix<mem_t> &rhs) const
    {
        Matrix<mem_t> ret = *this;
        ret -= rhs;
        return ret;
    };
    const Matrix<mem_t> operator*(const Matrix<mem_t> &rhs) const
    {
        if (cols != rhs.rows) return *this; // Multiplication requires right hand to have same rows as left hand has columns

        Matrix<mem_t> ret(rows, rhs.cols);

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < rhs.cols; c++)
            {
                data_t sum = 0;
                for (int n = 0; n < cols; n++)
                {
                    sum += mem(r, n) * rhs.mem(n, c);
                }
                ret(r, c) = sum;
            }
        }

        return ret;
    };
    const Matrix<mem_t> operator*(const data_t &rhs) const
    {
        Matrix<mem_t> ret = *this;
        ret *= rhs;
        return ret;
    };
    const Matrix<mem_t> operator/(const data_t &rhs) const
    {
        Matrix<mem_t> ret = *this;
        ret /= rhs;
        return ret;
    };

    const Matrix<mem_t> transposed() const
    {
        Matrix<mem_t> ret(cols, rows);

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                ret(c, r) = mem(r, c);
            }
        }

        return ret;
    }
    const Matrix<mem_t> operator~() const
    {
        return transposed();
    }

    const bool swapRows(int origRow, int destRow)
    {
        if (origRow < 0 || destRow < 0 || origRow >= rows || destRow >= rows) return false;

        data_t temp;
        for (int c = 0; c < cols; c++)
        {
            temp = mem(destRow, c);
            mem(destRow, c) = mem(origRow, c);
            mem(origRow, c) = temp;
        }

        return true;
    }

    // This function is literally black magic
    // Don't even try to understand it
    // https://en.wikipedia.org/wiki/LU_decomposition#C_code_example
    const Matrix<mem_t> inverse(data_t tol, bool &output) const
    {
        if (rows != cols)
        {
            output = false;
            return *this;
        }

        Matrix<mem_t> A = *this;
        int N = rows;

        // Unit permutation matrix
        Array<int> P(N + 1);
        for (int i = 0; i <= N; i++)
        {
            P(i) = i;
        }

        // LU Decompose
        int i, j, k, imax;
        data_t maxA, absA;

        for (i = 0; i < N; i++)
        {
            maxA = 0;
            imax = i;

            for (k = i; k < N; k++)
            {
                if ((absA = abs(A(k, i))) > maxA)
                {
                    maxA = absA;
                    imax = k;
                }
            }

            if (maxA < tol)
            {
                output = false;
                return *this; // Matrix is degenerate
            }

            if (imax != i)
            {
                // Pivoting P
                j = P(i);
                P(i) = P(imax);
                P(imax) = j;

                // Pivoting rows of A
                A.swapRows(i, imax);
                
                // Counting pivots starting from N (for determinant)
                P(N)++;
            }

            for (j = i + 1; j < N; j++)
            {
                A(j, i) /= A(i, i);

                for (k = i + 1; k < N; k++)
                {
                    A(j, k) -= A(j, i) * A(i, k);
                }
            }
        }

        // Decomposition done
        output = true;

        // Now, invert
        Matrix<mem_t> IA(A.rows, A.cols); // These should be the same technically but whatever
        
        for (j = 0; j < N; j++)
        {
            for (i = 0; i < N; i++)
            {
                IA(i, j) = P(i) == j ? 1.f : 0.f;

                for (k = 0; k < i; k++)
                {
                    IA(i, j) -= A(i, k) * IA(k, j);
                }
            }

            for (i = N - 1; i >= 0; i--)
            {
                for (k = i + 1; k < N; k++)
                {
                    IA(i, j) -= A(i, k) * IA(k, j);
                }

                IA(i, j) /= A(i, i);
            }
        }

        // Inverse complete
        return IA;
    }
};

}