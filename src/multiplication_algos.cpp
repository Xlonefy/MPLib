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

        return prod;
    }
    
    MPNumber karatsuba_multiply(const MPNumber &a, const MPNumber &b)
    {
        // return quadratic_multiply(a, b);
        ulint size = std::max(a.get_size(), b.get_size());

        MPNumber z[3];
        for (MPNumber &n : z)
        {
            n.reserve(size);
        }
        std::pair<MPNumber, MPNumber> a_ = a.split_at(size / 2);
        std::pair<MPNumber, MPNumber> b_ = b.split_at(size / 2);

        z[0] = a_.first.multiply(b_.first);
        z[2] = a_.second.multiply(b_.second);
        z[1] = a_.first.add(a_.second)
                 .multiply(b_.first.add(b_.second))
                 .subtract(z[0])
                 .subtract(z[2]);
        
        MPNumber product;
        product.reserve(3 * (size / 2) + 1);
        for (nint i = 2; i != -1; i--)
        {
            product.shift_left(size / 2);
            product = product.add(z[i]);
        }
        product.shrink_to_fit();
        //std::cout << a << "*" << b << "==" << product << "\n\n";

        return product;
    }
    
    //MPNumber toom3_multiply(const MPNumber &a, const MPNumber &b) {}
}