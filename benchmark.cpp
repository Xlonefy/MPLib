#include <iostream>
#include <random>
#include <functional>
#include <vector>

#include <sys/time.h>
#include <sys/resource.h>

#include "src/mpnumber.h"

typedef unsigned long long ull_int;

class TestCase
{
    public:
        TestCase(std::function<void(const MPLib::MPNumber &, const MPLib::MPNumber &)> test, std::string name)
                : test_(test), name_(name) {}
        
        void run(const MPLib::MPNumber &a, const MPLib::MPNumber &b)
        {
            test_(a, b);
        }

        std::string name()
        {
            return name_;
        }

    protected:
        std::function<void(const MPLib::MPNumber &, const MPLib::MPNumber &)> test_;
        std::string name_;
};

int main()
{
    std::mt19937 mt_random;
    mt_random.seed(2147483647);

    ull_int num_iters = 0;
    std::cout << "\x1B[1;34mIterations per test: \x1B[0m";
    std::cin >> num_iters;

    ull_int size = 0;
    std::cout << "\x1B[1;34mSize of numbers to test: \x1B[0m";
    std::cin >> size;

    std::cout << std::endl;

    MPLib::MPNumber a, b;

    for (ull_int i = 0; i < size; i++)
    {
        a.push_back(mt_random());
        b.push_back(mt_random());
    }

    std::vector<TestCase> tests = {{MPLib::karatsuba_multiply, "Karatsuba"},       
                                   {MPLib::quadratic_multiply, "Quadratic"},
                                   {[](const MPLib::MPNumber &a, const MPLib::MPNumber &b){
                                       a.pow(3);
                                   }, "Pow 3"},
                                   {[](const MPLib::MPNumber &a, const MPLib::MPNumber &b){
                                       a.square();
                                   }, "Square"},
                                   {[](const MPLib::MPNumber &a, const MPLib::MPNumber &b){
                                       a.add(b);
                                   }, "Add"},
                                   {[](const MPLib::MPNumber &a, const MPLib::MPNumber &b){
                                       a.subtract(b);
                                   }, "Subtract"}};

    for (TestCase &test : tests)
    {
        std::cout << "\x1B[1;34mRunning: \x1B[0m" << test.name() << "\n";

        rusage before;
        getrusage(RUSAGE_SELF, &before);
        try
        {
            for (ull_int i = 0; i < num_iters; i++)
            {
                test.run(a, b);
            }
        }
        catch (std::runtime_error e)
        {
            std::cout << "\x1B[1;91mError! Exception thrown during test.\nwhat(): " << e.what() << "\x1B[0m\n\n";
            continue;
        }
        rusage after;
        getrusage(RUSAGE_SELF, &after);

        ull_int time_delta = after.ru_utime.tv_usec - before.ru_utime.tv_usec + 1000000 * (after.ru_utime.tv_sec - before.ru_utime.tv_sec);
        double rate = 1000000.0 * static_cast<double>(num_iters) / static_cast<double>(time_delta);

        std::cout << "\x1B[1;32mTest Complete\x1B[0m\n\x1B[1;34mCPU time taken: \x1B[0m" << time_delta << "us\n"
                << "\x1B[1;34mRate: \x1B[0m" << rate << " iters / s\n\n";
    }

}