//////////////////////////////////////////////////////////////////////////
// mpnumber.h                                                           //
//                                                                      //
// João V. M. Andreotti                                                 //
//                                                                      //
// Class definition for both MPNumber and InstanceBaseCharacterException//
//////////////////////////////////////////////////////////////////////////

#ifndef MPNUM_H
#define MPNUM_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace MPLib
{
    typedef uint64_t ulint;
    typedef int64_t lint;
    typedef uint32_t unint;
    typedef int32_t nint;

    // TODO(xlonefy): make it inherit from std::runtime_error
    class InvalidBaseCharacterError : public std::exception
    {
        private:
            char oc_;
            std::string basechars_;
            std::string error_;
        
        public:
            InvalidBaseCharacterError(char offending_char, std::string base_chars)
                : oc_(offending_char), basechars_(base_chars) 
            { // TODO(xlonefy): pass this implementation to a different file
                error_ += "The character \'";
                error_ += oc_;
                error_ += "\' is not contained in the list of supported characters for the base (\"";
                error_ += basechars_;
                error_ += "\").";
            }

            const char* what() const noexcept
            {
                return error_.c_str();
            }
    };

    class MPNumber
    {
        public:
            MPNumber();
            MPNumber(int n);
            MPNumber(const std::string &number, unint base = 10);
            ~MPNumber();
            
            unint get(ulint location) const;

            std::pair<MPNumber, MPNumber> split_at(ulint location) const;

            void reserve(ulint size);
            void shrink_to_fit();
            void shift_left(unint ammount);
            void push_back(unint i);

            unint get_size() const;
            unint num_size() const;

            std::string get_string(unint base = 10) const;
            
            uint8_t get_byte(ulint location) const;
            void set_byte(ulint location, uint8_t value);

            bool check_bit(ulint location) const;
            void set_bit(ulint location, bool value);

            // TODO(xlonefy): do a compare function which can be called by these
            bool is_zero() const;
            bool greater_than(const MPNumber &n) const;
            bool lower_than(const MPNumber &n) const;
            bool equals(const MPNumber &n) const;

            MPNumber multiply(const MPNumber &n) const;
            MPNumber multiply(unint n) const;

            MPNumber divide(const MPNumber &n) const;
            MPNumber divide(unint n) const;

            MPNumber mod(const MPNumber &n) const;
            unint mod(unint n) const;

            MPNumber add(const MPNumber &n) const;
            MPNumber add(nint n) const;

            MPNumber subtract(const MPNumber &n) const;
            MPNumber subtract(nint n) const;

            MPNumber pow(ulint n) const;

            MPNumber square() const;

            MPNumber negate() const;

            friend MPNumber quadratic_multiply(const MPNumber &a, const MPNumber &b);
            friend MPNumber karatsuba_multiply(const MPNumber &a, const MPNumber &b);
            friend MPNumber toom3_multiply(const MPNumber &a, const MPNumber &b);

        private:
            std::vector<unint> num;
            bool positive;

            std::unordered_map<ulint, std::string> base_strs;

            void initialize_values();

            MPNumber raw_add(const MPNumber &n) const;
            MPNumber raw_add(unint n) const;

            MPNumber raw_subtract(const MPNumber &n) const;
            MPNumber raw_subtract(unint n) const;

    };

    MPNumber quadratic_multiply(const MPNumber &a, const MPNumber &b);
    MPNumber karatsuba_multiply(const MPNumber &a, const MPNumber &b);
}

#endif /* MPNUM_H */