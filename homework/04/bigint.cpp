#include "bigint.h"



int BigInt::base_s = 1000000000;

BigInt::BigInt()
{
    size_ = 4;
    count_ = 1;
    isNegative_ = false;
    data_ = new int64_t[size_];
    for (size_t i = 0; i < size_; ++i)
        data_[i] = 0;
}

BigInt::BigInt(int64_t num)
{
    size_ = 4;
    count_ = 1;
    if (num < 0)
    {
        isNegative_ = true;
        num = -num;
    }
    else
    {
        isNegative_ = false;
    }
    data_ = new int64_t[size_];
    size_t i = 0;
    while (num)
    {
        data_[i++] = num % base_s;
        num /= base_s;
    }
    count_ = i > count_ ? i : count_;
}

BigInt::BigInt(const BigInt& copied)
        : size_(copied.size_)
        , count_(copied.count_)
        , isNegative_(copied.isNegative_)
        , data_(new int64_t[copied.size_])
{
    std::copy(copied.data_, copied.data_ + size_, data_);
}

std::ostream& operator<< (std::ostream& out, const BigInt& value)
{
    if (value.isNegative_)
    {
        out << "-";
    }
    for (size_t i = 1; i <= value.count_; ++i)
    {
        out << value.data_[value.count_ - i];
    }
    return out;
}

bool BigInt::operator==(const BigInt& other) const
{
    if (this == &other)
    {
        return true;
    }
    if (isNegative_ != other.isNegative_ || count_ != other.count_)
    {
        return false;
    }

    for (size_t i = 0; i < count_; ++i)
    {
        if (data_[i] != other.data_[i])
        {
            return false;
        }
    }
    return true;
}

bool BigInt::operator!=(const BigInt& other) const
{
    return !(*this == other);
}

bool BigInt::operator<(const BigInt& other) const
{
    if (this == &other)
    {
        return false;
    }
    if (isNegative_ && !other.isNegative_)
    {
        return true;
    }
    if (!isNegative_ && other.isNegative_)
    {
        return false;
    }
    bool less = isAbsDataLess_(other);
    if (!isNegative_)
    {
        return less;
    }
    if (less)
    {
        return false;
    }
    return *this != other;
}

bool BigInt::operator>(const BigInt& other) const
{
    if (this == &other)
    {
        return false;
    }
    if (isNegative_ && !other.isNegative_)
    {
        return false;
    }
    if (!isNegative_ && other.isNegative_)
    {
        return true;
    }
    bool less = isAbsDataLess_(other);
    if (isNegative_)
    {
        return less;
    }
    if (less)
    {
        return false;
    }
    return *this != other;
}

bool BigInt::operator<=(const BigInt& other) const
{
    return !(*this > other);
}

bool BigInt::operator>=(const BigInt& other) const
{
    return !(*this < other);
}

bool BigInt::isAbsDataLess_(const BigInt& other) const
{
    if (count_ < other.count_)
    {
        return true;
    }
    if (count_ > other.count_)
    {
        return false;
    }
    for (size_t i = 1; i <= count_; ++i)
    {
        if (data_[count_ - i] < other.data_[count_ - i])
        {
            return true;
        }
        if (data_[count_ - i] > other.data_[count_ - i])
        {
            return false;
        }
    }
    return true;
}

BigInt& BigInt::operator=(const BigInt& copied)
{
    if (this == &copied)
    {
        return *this;
    }
    int64_t* ptr = new int64_t[copied.size_];
    delete data_;
    data_ = ptr;
    size_ = copied.size_;
    count_ = copied.count_;
    isNegative_ = copied.isNegative_;
    std::copy(copied.data_, copied.data_ + size_, data_);
    return *this;
}

BigInt BigInt::operator-() const
{
    BigInt tmp(*this);
    if (tmp.count_ != 1 || tmp.data_[0] != 0)
    {
        tmp.isNegative_ = !tmp.isNegative_;
    }
    return tmp;
}

size_t BigInt::addData_(const BigInt& op1, const BigInt& op2)
{
    size_t i, border = op1.count_ > op2.count_ ? op1.count_ : op2.count_;
    int64_t tr = 0;
    for (i = 0; i < border; ++i)
    {
        int64_t left = i < op1.count_ ? op1.data_[i] : 0;
        int64_t right = i < op2.count_ ? op2.data_[i] : 0;
        int64_t sum = left + right + tr;
        data_[i] = sum % base_s;
        tr = sum / base_s;
    }

    if (tr)
    {
        data_[i++] = tr;
    }
    return i;
}

size_t BigInt::subData_(const BigInt& op1, const BigInt& op2)
{
    size_t i, border = op1.count_ > op2.count_ ? op1.count_ : op2.count_;
    int64_t tr = 0;
    for (i = 0; i < border; ++i)
    {
        int64_t left = i < op1.count_ ? op1.data_[i] : 0;
        int64_t right = i < op2.count_ ? op2.data_[i] : 0;
        int64_t sub = left - right - tr;
        if (sub < 0)
        {
            tr = 1;
            sub += base_s;
        }
        else
        {
            tr = 0;
        }
        data_[i] = sub;
    }

    while (i > 1 && data_[i - 1] == 0)
    {
        --i;
    }
    return i;
}

BigInt BigInt::operator+(const BigInt& other) const
{
    BigInt tmp;
    tmp.size_ = (count_ > other.count_ ? count_ : other.count_) + 1;
    delete tmp.data_;
    tmp.data_ = new int64_t[tmp.size_];

    if (isNegative_ == other.isNegative_)
    {
        tmp.isNegative_ = isNegative_;
        tmp.count_ = tmp.addData_(*this, other);
    }
    else if (!isAbsDataLess_(other))
    {
        tmp.isNegative_ = isNegative_;
        tmp.count_ = tmp.subData_(*this, other);
    }
    else
    {
        tmp.isNegative_ = other.isNegative_;
        tmp.count_ = tmp.subData_(other, *this);
    }
    if (tmp.count_ == 1 && tmp.data_[0] == 0)
    {
        tmp.isNegative_ = false;
    }

    return tmp;
}

BigInt BigInt::operator-(const BigInt& other) const
{
    BigInt tmp;
    tmp.size_ = (count_ > other.count_ ? count_ : other.count_) + 1;
    delete tmp.data_;
    tmp.data_ = new int64_t[tmp.size_];

    if (isNegative_ != other.isNegative_)
    {
        tmp.isNegative_ = isNegative_;
        tmp.count_ = tmp.addData_(*this, other);
    }
    else if (!isAbsDataLess_(other))
    {
        tmp.isNegative_ = isNegative_;
        tmp.count_ = tmp.subData_(*this, other);
    }
    else
    {
        tmp.isNegative_ = !isNegative_;
        tmp.count_ = tmp.subData_(other, *this);
    }
    if (tmp.count_ == 1 && tmp.data_[0] == 0)
    {
        tmp.isNegative_ = false;
    }

    return tmp;
}
