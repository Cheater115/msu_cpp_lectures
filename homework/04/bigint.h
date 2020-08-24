#pragma once
#include <iostream>


class BigInt
{
private:
    static int base_s;

    size_t size_;
    size_t count_;
    bool isNegative_;
    int64_t* data_;

    bool isAbsDataLess_(const BigInt& other) const;
    size_t addData_(const BigInt& op1, const BigInt& op2);
    size_t subData_(const BigInt& op1, const BigInt& op2);
public:
    BigInt();
    BigInt(int64_t);
    BigInt(const BigInt& copied);

    friend std::ostream& operator<< (std::ostream& out, const BigInt& value);
    //friend std::istream& operator>> (std::iostream& in, BigInt& value);

    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;

    BigInt& operator=(const BigInt& copied);

    BigInt operator-() const;
    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    //BigInt operator*(const BigInt& other) const;
    //BigInt operator/(const BigInt& other) const;
};

