#include <iostream>
#include "matrix.h"

Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols)
{
    data_ = new int[rows_ * cols_];
}


Matrix::~Matrix()
{
    delete[] data_;
}


int Matrix::getRows() const
{
    return rows_;
}


int Matrix::getColumns() const
{
    return cols_;
}


const Matrix& Matrix::operator*=(const int mul)
{
    for (int i = rows_ * cols_ - 1; i >= 0; --i)
    {
        data_[i] *= mul;
    }
    return *this;
}


Matrix::Proxy Matrix::operator[](int i)
{
    if (i < 0 || i >= rows_)
    {
        throw std::out_of_range("");
    }
    return Proxy(data_ + i * cols_, cols_);
}


const Matrix::Proxy Matrix::operator[](int i) const
{
    if (i < 0 || i >= rows_)
    {
        throw std::out_of_range("");
    }
    return Proxy(data_ + i * cols_, cols_);
}


bool Matrix::operator==(const Matrix& other) const
{
    if (this == &other)
    {
        return true;
    }
    if (other.getRows() != rows_ || other.getColumns() != cols_)
    {
        return false;
    }
    for (int i = rows_ * cols_ - 1; i >= 0; --i)
    {
        if (data_[i] != other.data_[i])
        {
            return false;
        }
    }
    return true;
}


bool Matrix::operator!=(const Matrix& other) const
{
    return !(*this == other);
}


int& Matrix::Proxy::operator[](int j)
{
    if (j < 0 || j >= size_)
    {
        throw std::out_of_range("");
    }
    return data_[j];
}


const int& Matrix::Proxy::operator[](int j) const
{
    if (j < 0 || j >= size_)
    {
        throw std::out_of_range("");
    }
    return data_[j];
}
