//////////////////////////////////////////////////////////////////////
// division.cpp                                                     //
//                                                                  //
// João V. M. Andreotti                                             //
//                                                                  //
// Implements all division related functions defined in "mpnumber.h"//
//////////////////////////////////////////////////////////////////////

#include "mpnumber.h"

#define ABS(x) ((x < 0) ? -x : x)

namespace MPLib
{
    MPNumber MPNumber::divide(const MPNumber &n) const
    {
        // TODO(xlonefy): research a better algorithm, this is painfuly slow
        std::pair<MPNumber, MPNumber> range = {MPNumber(0), *this};
        range.second.positive = true;
        MPNumber range_middle;

        MPNumber n_cp = n;
        n_cp.positive = true;

        while (!range.first.equals(range.second))
        {
            range_middle = range.first.add(range.second).divide(2);

            MPNumber mul = range_middle.multiply(n_cp);
            if (mul.greater_than(*this))
            {
                range.second = range_middle.subtract(1);
            }
            else
            {
                range.first = range_middle;
                if (mul.add(n_cp).greater_than(*this))
                {
                    break;
                }
                range.first = range.first.add(1);
            }
        }

        range.first.positive = !(this->positive ^ n.positive);
        return range.first;
    }

    MPNumber MPNumber::divide(unint n) const
    {
        MPNumber quot;
        quot.reserve(get_size());

        unint carry = 0;
        for (auto i = num.rbegin(); i != num.rend(); i++)
        {
            ulint cur_num = *i;
            cur_num += static_cast<ulint>(carry) << (8 * sizeof(unint));

            quot.num.insert(quot.num.begin(), cur_num / n);
            carry = cur_num % n;
        }

        quot.shrink_to_fit();
        quot.positive = positive;
        return quot;
    }

    MPNumber MPNumber::mod(const MPNumber &n) const
    {
        throw std::runtime_error("Uninplemented");
    } // TODO(xlonefy): implement

    unint MPNumber::mod(unint n) const 
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
}