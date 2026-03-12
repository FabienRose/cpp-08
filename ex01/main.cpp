#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Span.h"

int main(void)
{
    // --- Subject example ---
    std::cout << "=== Subject example ===" << std::endl;
    Span sp = Span(5);
    sp.addNumber(6);
    sp.addNumber(3);
    sp.addNumber(17);
    sp.addNumber(9);
    sp.addNumber(11);
    std::cout << "Shortest: " << sp.shortestSpan() << std::endl; // 2
    std::cout << "Longest:  " << sp.longestSpan()  << std::endl; // 14

    // --- Exception: span full ---
    std::cout << "\n=== Exception: span full ===" << std::endl;
    try { sp.addNumber(42); }
    catch (const std::exception& e)
    { std::cout << "Caught: " << e.what() << std::endl; }

    // --- Exception: too few numbers ---
    std::cout << "\n=== Exception: too few numbers ===" << std::endl;
    Span sp2(1);
    sp2.addNumber(5);
    try { sp2.shortestSpan(); }
    catch (const std::exception& e)
    { std::cout << "shortestSpan: " << e.what() << std::endl; }
    try { sp2.longestSpan(); }
    catch (const std::exception& e)
    { std::cout << "longestSpan:  " << e.what() << std::endl; }

    // --- Exception: empty span ---
    std::cout << "\n=== Exception: empty span ===" << std::endl;
    Span sp3(5);
    try { sp3.shortestSpan(); }
    catch (const std::exception& e)
    { std::cout << "shortestSpan: " << e.what() << std::endl; }

    // --- Large test with addRange ---
    std::cout << "\n=== Large test: 10000 numbers ===" << std::endl;
    std::srand(std::time(NULL));
    std::vector<int> nums;
    for (int i = 0; i < 10000; i++)
        nums.push_back(std::rand());
    Span big(10000);
    big.addRange(nums.begin(), nums.end());
    std::cout << "Shortest span: " << big.shortestSpan() << std::endl;
    std::cout << "Longest span:  " << big.longestSpan()  << std::endl;

    return 0;
}