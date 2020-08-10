#pragma once

class Matrix
{
private:
    int rows_;
    int cols_;
    int *data_;
    class Proxy
    {
        int* data_;
        int size_;
    public:
        Proxy(int* ptr, int size) : data_(ptr), size_(size){}
        int& operator[](int j);
        const int& operator[](int j) const;
    };
public:
    Matrix(int rows, int cols);
    ~Matrix();

    int getRows() const;
    int getColumns() const;

    const Matrix& operator*=(const int mul);

    Proxy operator[](int i);
    const Proxy operator[](int i) const;

    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;

    void printInfo()
    {
        std::cout << "Info: rows = " << rows_
            << "; cols = " << cols_
            << "; address = " << data_ << "\n";
    }
};
