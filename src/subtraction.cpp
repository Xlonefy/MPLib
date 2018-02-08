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
    MPNumber MPNumber::subtract(const MPNumber &n)
    {
        return add(n.negate());
    }

    MPNumber MPNumber::subtract(nint n)
    {
        return add(-n);
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
}