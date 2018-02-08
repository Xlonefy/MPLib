/****************************************************************************
 *  mpnumber.cpp                                                            *
 *                                                                          *
 *  João V. M. Andreotti                                                    *
 *                                                                          *
 *  This file contains the implementations of all of the functions defined  *
 *  in the "MPNumber" class present in the file "mpnumber.h"                *
 *                                                                          *
 ****************************************************************************/


#include "mpnumber.h"

#define ABS(x) ((x < 0) ? -x : x)

/**********************
 *  Public Functions  *
 **********************/

MPNumber::MPNumber() 
{
    initialize_values();
}

MPNumber::MPNumber(nint n) 
{
    initialize_values();
    num.at(0) = ABS(n);
    positive = n >= 0;
}

MPNumber::MPNumber(const std::string &number, unint base) 
{
    initialize_values(); 
    std::string base_str = base_strs.at(base);
    MPNumber b(1);

    bool has_sign = false;
    bool is_positive = true; // necessary beacause setting "positive" directly causes problems with "add" on the loop
    switch (number.front())
    {
        case '-':
            is_positive = false;
            has_sign = true;
            break;
        case '+':
            has_sign = true;
            break;    
    
    }


    for (auto i = number.rbegin(); i != number.rend() - ((has_sign) ? 1 : 0); i++)
    {
        ulint digit_val = base_str.find(*i);
        if (digit_val == std::string::npos)
        {
            throw InvalidBaseCharacterError(*i, base_str);
        }

        this->num = add(b.multiply(digit_val)).num;
        b = b.multiply(base);
    }

    positive = is_positive;
}

MPNumber::~MPNumber() {}


unint MPNumber::get(uint location) const
{
    if (location < num.size())
    {
        return num.at(location);
    }
    else
    {
        return 0;
    }

}

void MPNumber::reserve(uint size)
{
    num.reserve(size);
}

void MPNumber::shrink_to_fit()
{
    while (*num.rbegin() == 0 && num.size() > 1)
    {
        num.pop_back();
    }
}

uint MPNumber::get_size()
{
    shrink_to_fit();
    return num.size();
}

std::string MPNumber::get_string(unint base)
{
    std::string base_str = base_strs.at(base);
    MPNumber number = *this;
    std::string res("");

    while (!number.is_zero())
    {
        char cur_digit = base_str.at(number.mod(base));
        res.insert(res.begin(), cur_digit);
        number = number.divide(base);
    }

    if (res.length() == 0)
    {
        res += "0";
        return res;
    }

    if (!positive)
    {
        res.insert(res.begin(), '-');
    }

    return res;
}

uint8_t MPNumber::get_byte(uint location)
{
    int index = location / sizeof(unint);
    int shift = (location % sizeof(unint)) * 8;

    return (0xFF & (num.at(index) >> shift));

}

void MPNumber::set_byte(uint location, uint8_t value) 
{
    int index = location / sizeof(unint);
    int shift = (location % sizeof(unint)) * 8;

    num.at(index) &= ~(static_cast<int>(0xFF << shift));
    num.at(index) += value << shift;
}

bool MPNumber::check_bit(uint location) 
{
    int index = location / (8 * sizeof(unint));
    int shift = location % (8 * sizeof(unint));

    return 1 & (num.at(index) >> shift);
}

void MPNumber::set_bit(uint location, bool value) 
{
    int index = location / (8 * sizeof(unint));
    int shift = location % (8 * sizeof(unint));

    num.at(index) &= ~(static_cast<unint>(1 << shift));
    num.at(index) += static_cast<unint>(value) << shift;
}

bool MPNumber::is_zero()
{
    if (get_size() == 1 && num.at(0) == 0)
    {
        return true;
    }

    return false;
}


bool MPNumber::greater_than(const MPNumber &n)
{
    ulint n_sz = n.num.size();
    ulint this_sz = get_size();
    if (this_sz > n_sz)
    {
        return true;
    }
    else if (n_sz > this_sz)
    {
        return false;
    }

    ulint size = n_sz;

    for (ulint i = size; i != 0; i--)
    {
        unint this_num = get(i - 1);
        unint n_num = n.get(i - 1);
        
        if (this_num > n_num)
        {
            return true;
        }
        else if (n_num > this_num)
        {
            return false;
        }
    }

    return false;
}

MPNumber MPNumber::multiply(const MPNumber &n)
{
    throw std::runtime_error("Uninplemented");
} // TODO(xlonefy): implement

MPNumber MPNumber::multiply(nint n)
{
    MPNumber prod;
    prod.reserve(get_size() + 1);
    prod.positive = !(positive ^ ((n < 0) ? false : true)); 

    prod.num.pop_back();

    n = ABS(n);

    unint carry = 0;
    for (auto i = num.begin(); i != num.end(); i++)
    {
        ulint cur_num = n * static_cast<ulint>(*i) + carry;

        prod.num.push_back(static_cast<unint>(cur_num));

        carry = cur_num >> (8 * sizeof(unint));
    }

    prod.num.push_back(carry);
    prod.shrink_to_fit();

    return prod;
}

MPNumber MPNumber::divide(const MPNumber &n)
{
    throw std::runtime_error("Uninplemented");
} // TODO(xlonefy): implement

MPNumber MPNumber::divide(nint n) 
{
    MPNumber quot;
    quot.reserve(get_size());
    quot.positive = !(positive ^ ((n < 0) ? false : true)); 

    n = ABS(n);

    unint carry = 0;
    for (auto i = num.rbegin(); i != num.rend(); i++)
    {
        ulint cur_num = *i;
        cur_num += static_cast<ulint>(carry) << (8 * sizeof(unint));

        quot.num.insert(quot.num.begin(), cur_num / n);
        carry = cur_num % n;
    }

    quot.shrink_to_fit();
    return quot;
}

MPNumber MPNumber::mod(const MPNumber &n)
{
    throw std::runtime_error("Uninplemented");
} // TODO(xlonefy): implement

unint MPNumber::mod(unint n) 
{
    unint carry = 0;
    for (auto i = num.rbegin(); i != num.rend(); i++)
    {
        ulint cur_num = *i;
        cur_num += static_cast<ulint>(carry) << (8 * sizeof(unint));
        carry = cur_num % n;
    }

    return carry;
}

MPNumber MPNumber::add(const MPNumber &n) 
{
    if (!n.positive)
    {
        if (positive)
        {
            return raw_subtract(n.negate());
        }
        else
        {
            return raw_add(n.negate()).negate();
        }
    }
    else
    {
        if (positive)
        {
            return raw_add(n);
        }
        else
        {
            return raw_subtract(n);
        }
    }
}

MPNumber MPNumber::add(nint n) 
{
    if (n < 0)
    {
        if (positive)
        {
            return raw_subtract(static_cast<unint>(-n));
        }
        else
        {
            return raw_add(static_cast<unint>(-n));
        }
    }
    else
    {
        if (positive)
        {
            return raw_add(static_cast<unint>(n));
        }
        else
        {
            return raw_subtract(static_cast<unint>(n));
        }
    }
}

MPNumber MPNumber::subtract(const MPNumber &n)
{
    return add(n.negate());
}

MPNumber MPNumber::subtract(nint n)
{
    return add(-n);
}

MPNumber MPNumber::pow(uint n)
{
    MPNumber accumulator(1);
    accumulator.reserve(n * get_size());

    MPNumber squared = *this;
    squared.reserve(2 * n * get_size());

    while (n)
    {
        if (n & 1)
        {
            accumulator = accumulator.multiply(squared);
        }

        squared = squared.square();
        n >>= 1;
    }

    accumulator.shrink_to_fit();
    return accumulator;
}

MPNumber MPNumber::square()
{
    return (*this).multiply(*this);
}

MPNumber MPNumber::negate() const
{
    MPNumber res = *this;
    res.positive = !res.positive;
    return res;
}

/***********************
 *  Private Functions  *
 ***********************/


void MPNumber::initialize_values()
{
    num.push_back(0);
    positive = true;

    base_strs.insert({
        {8, std::string("01234567")},
        {10, std::string("0123456789")},
        {16, std::string("0123456789abcdef")},
        {32, std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567")},
        {64, std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/")}
    });
}

MPNumber MPNumber::raw_add(const MPNumber &n)
{
    MPNumber res;
    res.positive = true;

    res.num.pop_back();

    unint this_size = get_size();
    unint n_size = n.num.size();
    unint size = std::max(this_size, n_size);

    res.reserve(size + 1);

    bool carry = false;
    for (ulint i = 0; i < size; i++)
    {
        unint sum = n.get(i) + get(i);
        res.num.push_back((carry) ? ++sum : sum);
        carry = false;

        if (sum < n.get(i) && sum < get(i))
        {
            carry = true;
        }
    }

    if (carry)
    {
        res.num.push_back(1);
    }

    res.shrink_to_fit();
    return res;
}

MPNumber MPNumber::raw_add(unint n)
{
    MPNumber res = *this;
    unint sum = n + num.at(0);
    res.num.at(0) = sum;
    if (sum < n && sum < num.at(0))
    {
        bool carry = true;
        for (auto i = res.num.begin() + 1; i != res.num.end(); i++)
        {
            unint prev_i = *i;
            
            carry = false;
            if (++(*i) < prev_i)
            {
                carry = true;
            }
            else
            {
                break;
            }
        }

        if (carry)
        {
            res.num.push_back(1);
        }
    }

    res.shrink_to_fit();
    return res;
}

MPNumber MPNumber::raw_subtract(const MPNumber &n)
{
    unint this_size = get_size();
    unint n_size = n.num.size();
    unint size = std::max(this_size, n_size);

    MPNumber res = (greater_than(n)) ? *this : n;
    const MPNumber &smallest = (greater_than(n)) ? n : *this;
    
    if (!greater_than(n))
    {
        res.positive = !positive;
    }

    bool carry = false;
    for (ulint i = 0; i < size; i++)
    {
        unint prev_val = res.get(i);
        res.num.at(i) = prev_val - smallest.get(i) - ((carry) ? 1 : 0);

        carry = false;
        if (res.num.at(i) > prev_val)
        {
            carry = true;
        }
    }

    res.shrink_to_fit();
    return res;
}

MPNumber MPNumber::raw_subtract(unint n)
{
    MPNumber res = *this;
    
    lint sub = res.num.at(0);
    if (n > sub && get_size() > 1)
    {

        for (auto i = res.num.begin() + 1; i != res.num.end(); i++)
        {
            unint prev_i = *i;

            if (--(*i) <= prev_i)
            {
                break;
            }
        }

        sub += static_cast<lint>(1) << (8 * sizeof(unint));
    }

    sub -= n;

    if (sub < 0)
    {
        res.positive = !res.positive;
        res.num.at(0) = static_cast<unint>(-sub);
    }
    else
    {
        res.num.at(0) = static_cast<unint>(sub);
    }

    res.shrink_to_fit();
    return res;
}
