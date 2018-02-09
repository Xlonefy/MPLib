//////////////////////////////////////////////////////////////////
// utils.cpp                                                    //
//                                                              //
// João V. M. Andreotti                                         //
//                                                              //
// Implements all the utility functions defined in "mpnumber.h" //
//////////////////////////////////////////////////////////////////

#include "mpnumber.h"

#define ABS(x) ((x < 0) ? -x : x)

namespace MPLib
{
    unint MPNumber::get(uint location) const
    {
        if (location < num_size())
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
        while (*num.rbegin() == 0 && num_size() > 1)
        {
            num.pop_back();
        }
    }

    void MPNumber::shift_left(unint ammount)
    {
        for (unint i = 0; i < ammount; i++)
        {
            num.insert(num.begin(), 0);
        }
    }

    unint MPNumber::get_size() const
    {
        ulint size = num_size();

        auto i = num.begin();
        while (i != num.end() && *i == 0)
        {
            size--;
            i++;
        }

        return (size) ? size : 1;
    }

    unint MPNumber::num_size() const
    {
        return num.size();
    }

    std::string MPNumber::get_string(unint base) const
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

    uint8_t MPNumber::get_byte(uint location) const
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

    bool MPNumber::check_bit(uint location) const 
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

    bool MPNumber::is_zero() const
    {
        if (get_size() == 1 && num.at(0) == 0)
        {
            return true;
        }

        return false;
    }


    bool MPNumber::greater_than(const MPNumber &n) const
    {
        ulint n_sz = n.get_size();
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

    bool MPNumber::equals(const MPNumber &n) const
    {
        ulint n_sz = n.get_size();
        ulint this_sz = get_size();

        if (n_sz != this_sz)
        {
            return false;
        }

        ulint size = n_sz;

        for (int i = 0; i < n_sz; i++)
        {
            if(get(i) != n.get(i))
            {
                return false;
            }
        }

        return true;
    }

    MPNumber MPNumber::negate() const
    {
        MPNumber res = *this;
        res.positive = !res.positive;
        return res;
    }
}