#include <cstdlib>

#include <gtest/gtest.h>

#include "../src/mpnumber.h"

// for (auto i = str.begin(); i != str.end; i++)
// {
//     if (*i != '+' && *i != '-' && *i != '0')
//     {
        
//     }
// }

namespace testing
{
    const unsigned int NUM_TESTS = 1000;
    const unsigned int MAX_STR_LEN = 500;
    const unsigned int MAX_INT_STR_LEN = 9;

    std::string remove_redundancies(std::string str)
    {
        bool add_negative = str.front() == '-';
        if (add_negative)
        {
            str.erase(0, 1);
        }

        while (str.front() == '+' || str.front() == '0')
        {
            str.erase(0, 1);
        }

        if (str.length() == 0)
        {
            str = "0";
            return str;
        }

        if (add_negative)
        {
            str.insert(str.begin(), '-');
        }    

        return str;
    }

    std::string random_decimal_str(unsigned int length, bool positive_only = false)
    {
        std::srand(std::time(nullptr));
        char decimal[11] = "0123456789";

        std::string rnd("");
        for (unsigned int i = 0; i < length; i++)
        {
            rnd += decimal[std::rand() % 10];
        }

        short rint = 0;
        if ((rint = (std::rand() % 3)))
        {
            char c = (rint == 1) ? '+' : '-';
            rnd.insert(rnd.begin(), (positive_only) ? '+' : c);
        }

        return rnd;
    }

    TEST(MPNumberTest, TestStringConversion)
    {
        EXPECT_THROW(MPNumber("!@#$^&"), InvalidBaseCharacterError);
        EXPECT_THROW(MPNumber("+123BATATA"), InvalidBaseCharacterError);
        EXPECT_THROW(MPNumber("-12312WRONG"), InvalidBaseCharacterError);
        EXPECT_THROW(MPNumber("--123123"), InvalidBaseCharacterError);

        std::srand(std::time(nullptr));
        for (unsigned int i = 0; i < NUM_TESTS; i++)
        {
            std::string str = random_decimal_str(std::rand() % MAX_STR_LEN);
            MPNumber a(str);
            EXPECT_EQ(remove_redundancies(str), a.get_string());
        }
    }

    TEST(MPNumberTest, TestAdditionCommutativity)
    {
        std::srand(std::time(nullptr));
        for (unsigned int i = 0; i < NUM_TESTS; i++)
        {
            std::string a_str, b_str;
            a_str = random_decimal_str(std::rand() % MAX_STR_LEN);
            b_str = random_decimal_str(std::rand() % MAX_STR_LEN);
            MPNumber a(a_str), b(b_str);

            EXPECT_EQ(a.add(b).get_string(), b.add(a).get_string());
        }
    }

    TEST(MPNumberTest, TestSingleIntAddition)
    {
        std::srand(std::time(nullptr));
        for (unsigned int i = 0; i < NUM_TESTS; i++)
        {
            std::string a_str, b_str;
            a_str = random_decimal_str(std::rand() % MAX_STR_LEN);
            b_str = random_decimal_str(std::rand() % MAX_INT_STR_LEN, true);
            MPNumber a(a_str), b(b_str);

            EXPECT_EQ(a.add(b).get_string(), a.add(b.get(0)).get_string());
        }
    }

    TEST(MPNumberTest, TestSubtract)
    {
        std::srand(std::time(nullptr));
        for (unsigned int i = 0; i < NUM_TESTS; i++)
        {
            std::string a_str, b_str;
            a_str = random_decimal_str(std::rand() % MAX_STR_LEN);
            MPNumber a(a_str);

            EXPECT_EQ("0", a.subtract(a).get_string());
        }
    }

    TEST(MPNumberTest, TestSingleIntSubtraction)
    {
        std::srand(std::time(nullptr));
        for (unsigned int i = 0; i < NUM_TESTS; i++)
        {
            std::string a_str, b_str;
            a_str = random_decimal_str(std::rand() % MAX_STR_LEN);
            b_str = random_decimal_str(std::rand() % MAX_INT_STR_LEN, true);
            MPNumber a(a_str), b(b_str);

            EXPECT_EQ(a.subtract(b).get_string(), a.subtract(b.get(0)).get_string());
        }
    }

    TEST(MPNumberTest, TestAdditionSubtraction)
    {
        std::srand(std::time(nullptr));
        for (unsigned int i = 0; i < NUM_TESTS; i++)
        {
            std::string a_str, b_str;
            a_str = random_decimal_str(std::rand() % MAX_STR_LEN);
            b_str = random_decimal_str(std::rand() % MAX_STR_LEN);
            MPNumber a(a_str), b(b_str);

            EXPECT_EQ(remove_redundancies(a_str), a.add(b).subtract(b).get_string());
            EXPECT_EQ(remove_redundancies(b_str), a.add(b).subtract(a).get_string());
        }
    }
}