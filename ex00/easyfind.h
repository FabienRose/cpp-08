#ifndef EASYFIND_H
# define EASYFIND_H

# include <algorithm>
# include <exception>

class NotFoundException : public std::exception
{
    public:
        virtual const char* what() const throw() 
		{
            return "Element not found in container";
        }
};

template <typename T>
typename T::const_iterator easyfind(const T& container, int value)
{
	typename T::const_iterator it = std::find(container.begin(), container.end(), value);
	if (it == container.end())
		throw NotFoundException();
	return it;
}

#endif
