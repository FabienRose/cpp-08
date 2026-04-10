#include <iostream>
#include <vector>
#include <list>
#include "easyfind.h"

template <typename T>
void testFind(const T& container, int value)
{
	try
	{
		typename T::const_iterator it = easyfind(container, value);
		std::cout << "Found " << value << " at position "
		          << std::distance(container.begin(), it) << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << " (searched for " << value << ")" << std::endl;
	}
}

int main(void)
{
	std::cout << "=== vector ==" << std::endl;
	std::vector<int> vec;
	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(30);
	vec.push_back(40);
	vec.push_back(50);
	testFind(vec, 30);   // found at index 2
	testFind(vec, 99);   // not found

	std::cout << "\n=== list ===" << std::endl;
	std::list<int> lst;
	lst.push_back(1);
	lst.push_back(2);
	lst.push_back(3);
	testFind(lst, 2);    // found at index 1
	testFind(lst, 0);    // not found

	return 0;
}
