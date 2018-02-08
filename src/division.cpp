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
}