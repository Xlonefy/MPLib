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

namespace MPLib
{
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

    MPNumber MPNumber::negate() const
    {
        MPNumber res = *this;
        res.positive = !res.positive;
        return res;
    }
}