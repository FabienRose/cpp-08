
#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <limits>

class Span
{
private:

	unsigned int		_maxSize;
	std::vector<int> 	_numbers;

public:

	Span(unsigned int N);
	Span(const Span &other);
	Span &operator=(const Span &other);
	~Span();

	void addNumber(int number);
	int shortestSpan();
	int longestSpan();

	template <typename iterator>
	void addRange(iterator begin, iterator end)
	{
		for (iterator it = begin; it != end; ++it)
		{
			addNumber(*it);
		}
	}

};
