#include <iostream>
#include <list>
#include "MutantStack.hpp"

static void testMutantStack()
{
	std::cout << "=== MutantStack ===" << std::endl;
	MutantStack<int> mstack;

	mstack.push(5);
	mstack.push(17);
	std::cout << mstack.top() << std::endl;
	mstack.pop();
	std::cout << mstack.size() << std::endl;

	mstack.push(3);
	mstack.push(5);
	mstack.push(737);
	mstack.push(0);

	MutantStack<int>::iterator it  = mstack.begin();
	MutantStack<int>::iterator ite = mstack.end();
	std::cout << "Iterating through MutantStack:" << std::endl;
	++it;
	std::cout << "After incrementing iterator once: " << *it << std::endl;
	--it;
	std::cout << "After decrementing iterator once: " << *it << std::endl;
	while (it != ite)
	{
		std::cout << *it << std::endl;
		++it;
	}
	std::stack<int> s(mstack);
}

static void testList()
{
	std::cout << "=== std::list ===" << std::endl;
	std::list<int> mstack;

	mstack.push_back(5);
	mstack.push_back(17);
	std::cout << mstack.back() << std::endl;
	mstack.pop_back();
	std::cout << mstack.size() << std::endl;

	mstack.push_back(3);
	mstack.push_back(5);
	mstack.push_back(737);
	mstack.push_back(0);

	std::list<int>::iterator it  = mstack.begin();
	std::list<int>::iterator ite = mstack.end();
	std::cout << "Iterating through std::list:" << std::endl;
	++it;
	std::cout << "After incrementing iterator once: " << *it << std::endl;
	--it;
	std::cout << "After decrementing iterator once: " << *it << std::endl;
	while (it != ite)
	{
		std::cout << *it << std::endl;
		++it;
	}
}

int main()
{
	testMutantStack();
	testList();
	return 0;
}