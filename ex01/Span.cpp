#include "Span.h"

// TODO: implement the Orthodox Canonical Form
Span::Span(unsigned int N) : _maxSize(N)
{
}

Span::Span(const Span &other) : _maxSize(other._maxSize), _numbers(other._numbers)
{
}

Span &Span::operator=(const Span &other)
{
	if (this != &other)
	{
		_maxSize = other._maxSize;
		_numbers = other._numbers;
	}
	return *this;
}

Span::~Span()
{
}

// TODO: implement addNumber()
void Span::addNumber(int number)
{
	if (_numbers.size() >= _maxSize)
		throw std::runtime_error("Span is full");
	_numbers.push_back(number);
}

// TODO: implement shortestSpan()
int Span::shortestSpan()
{
	if (_numbers.size() < 2)
		throw std::runtime_error("Not enough numbers");
	
	int minSpan = std::numeric_limits<int>::max();
	for (size_t i = 0; i < _numbers.size(); ++i)
	{
		for (size_t j = i + 1; j < _numbers.size(); ++j)
		{
			int span = std::abs(_numbers[i] - _numbers[j]);
			if (span < minSpan)
				minSpan = span;
		}
	}
	return minSpan;
}

// TODO: implement longestSpan()
int Span::longestSpan()
{
	if (_numbers.size() < 2)
		throw std::runtime_error("Not enough numbers");

	int maxSpan = std::numeric_limits<int>::min();
	for (size_t i = 0; i < _numbers.size(); ++i)
	{
		for (size_t j = i + 1; j < _numbers.size(); ++j)
		{
			int span = std::abs(_numbers[i] - _numbers[j]);
			if (span > maxSpan)
				maxSpan = span;
		}
	}
	return maxSpan;
}
