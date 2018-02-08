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

    class InvalidBaseCharacterError : public std::exception
    {
        private:
            char oc_;
            std::string basechars_;
            std::string error_;
        
        public:
            InvalidBaseCharacterError(char offending_char, std::string base_chars)
                : oc_(offending_char), basechars_(base_chars) 
            {
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
            
            unint get(uint location) const;

            void reserve(uint size);
            void shrink_to_fit();

            uint get_size() const;

            std::string get_string(unint base = 10) const;
            
            uint8_t get_byte(uint location) const;
            void set_byte(uint location, uint8_t value);

            bool check_bit(uint location) const;
            void set_bit(uint location, bool value);

            bool is_zero() const;
            bool greater_than(const MPNumber &n) const;

            MPNumber multiply(const MPNumber &n) const;
            MPNumber multiply(nint n) const;

            MPNumber divide(const MPNumber &n) const;
            MPNumber divide(nint n) const;

            MPNumber mod(const MPNumber &n) const;
            unint mod(unint n) const;

            MPNumber add(const MPNumber &n) const;
            MPNumber add(nint n) const;

            MPNumber subtract(const MPNumber &n) const;
            MPNumber subtract(nint n) const;

            MPNumber pow(uint n) const;

            MPNumber square() const;

            MPNumber negate() const;

        private:
            std::vector<unint> num;
            bool positive;

            std::unordered_map<uint, std::string> base_strs;

            void initialize_values();

            MPNumber raw_add(const MPNumber &n) const;
            MPNumber raw_add(unint n) const;

            MPNumber raw_subtract(const MPNumber &n) const;
            MPNumber raw_subtract(unint n) const;

    };
}

#endif /* MPNUM_H */