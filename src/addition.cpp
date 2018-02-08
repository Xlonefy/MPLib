//////////////////////////////////////////////////////////////////////
// addition.cpp                                                     //
//                                                                  //
// João V. M. Andreotti                                             //
//                                                                  //
// Implements all addition related functions defined in "mpnumber.h"//
//////////////////////////////////////////////////////////////////////

#include "mpnumber.h"

#define ABS(x) ((x < 0) ? -x : x)

namespace MPLib
{
    MPNumber MPNumber::add(const MPNumber &n) const
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

    MPNumber MPNumber::add(nint n) const 
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

    MPNumber MPNumber::raw_add(const MPNumber &n) const
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

    MPNumber MPNumber::raw_add(unint n) const
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
}