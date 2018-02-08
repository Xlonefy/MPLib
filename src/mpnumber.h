#ifndef MPNUM_H
#define MPNUM_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

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

        uint get_size();

        std::string get_string(unint base = 10);
        
        uint8_t get_byte(uint location);
        void set_byte(uint location, uint8_t value);

        bool check_bit(uint location);
        void set_bit(uint location, bool value);

        bool is_zero();
        bool greater_than(const MPNumber &n);

        MPNumber multiply(const MPNumber &n);
        MPNumber multiply(nint n);

        MPNumber divide(const MPNumber &n);
        MPNumber divide(nint n);

        MPNumber mod(const MPNumber &n);
        unint mod(unint n);

        MPNumber add(const MPNumber &n);
        MPNumber add(nint n);

        MPNumber subtract(const MPNumber &n);
        MPNumber subtract(nint n);

        MPNumber pow(uint n);

        MPNumber square();

        MPNumber negate() const;

    private:
        std::vector<unint> num;
        bool positive;

        std::unordered_map<uint, std::string> base_strs;

        void initialize_values();

        MPNumber raw_add(const MPNumber &n);
        MPNumber raw_add(unint n);

        MPNumber raw_subtract(const MPNumber &n);
        MPNumber raw_subtract(unint n);

};

#endif /* MPNUM_H */