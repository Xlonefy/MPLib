//////////////////////////////////////////////
// multiplication_algos.cpp                 //
//                                          //
// João V. M. Andreotti                     //
//                                          //
// Implements all multiplication algorithms.//
//////////////////////////////////////////////

#include "mpnumber.h"

#define ABS(x) ((x < 0) ? -x : x)

std::ostream &operator<<(std::ostream &stream, const MPLib::MPNumber &n)
{
    stream << n.get_string();
    return stream;
}

namespace MPLib
{
    MPNumber quadratic_multiply(const MPNumber &a, const MPNumber &b)
    {
        MPNumber prod(0);
        prod.reserve(a.get_size() + b.get_size());
        prod.positive = true;

        MPNumber b_cp = b;
        b_cp.positive = true;

        for (auto i = a.num.rbegin(); i != a.num.rend(); i++)
        {
            prod.shift_left(1);    
            prod = prod.raw_add(b.multiply(*i));
        }

        prod.positive = !(a.positive ^ b.positive); 
        prod.shrink_to_fit();

        // std::cout << a << "*" << b << "==" << prod << "\n\n";
        return prod;
    }
    
    //MPNumber karatsuba_multiply(const MPNumber &a, const MPNumber &b) {}
    
    //MPNumber toom3_multiply(const MPNumber &a, const MPNumber &b) {}
}