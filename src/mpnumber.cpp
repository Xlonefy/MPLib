////////////////////////////////////////////////////////////////////////////
// mpnumber.cpp                                                           //
//                                                                        //
// João V. M. Andreotti                                                   //
//                                                                        //
// Implements all instantiation related functions defined in "mpnumber.h" //
////////////////////////////////////////////////////////////////////////////

#include "mpnumber.h"

#define ABS(x) ((x < 0) ? -x : x)

namespace MPLib
{
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

    MPNumber::MPNumber(const std::vector<unint> &num)
    {
        initialize_values();
        this->num = num;
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
        shrink_to_fit();
    }

    MPNumber::~MPNumber() {}

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
}