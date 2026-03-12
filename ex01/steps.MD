# Exercise 01 — Span: Step-by-Step Guide

---

## Step 1 — Design the class in `Span.h`

### Why
Before writing a single line of implementation, you need to think about **what data the class needs to hold** and **what its public interface will look like**. This is fundamental OOP discipline: separate declaration from definition, and always plan the shape of a class before coding it.

### What to do
In `Span.h`:
- Add the necessary `#include` directives (`<vector>`, `<algorithm>`, `<stdexcept>`).
- Declare two **private** member variables:
  - An `unsigned int` for the maximum capacity (`_maxSize`).
  - A `std::vector<int>` to store the numbers (`_numbers`).
- Declare the **Orthodox Canonical Form** (constructor, copy constructor, copy assignment operator, destructor) in the `public` section.
- Declare `addNumber(int)`, `shortestSpan()`, and `longestSpan()`.
- Add the `addRange()` template **directly in the header** (templates must be fully defined where they are declared).

> **Key rule:** `std::vector` is your best friend here. It has `size()`, `push_back()`, iterators, and works with `std::sort`, `std::min_element`, and `std::max_element`.

<details>
<summary>💡 Solution — Span.h</summary>

```cpp
#ifndef SPAN_H
# define SPAN_H

# include <vector>
# include <algorithm>
# include <stdexcept>

class Span
{
private:
    unsigned int        _maxSize;
    std::vector<int>    _numbers;

public:
    Span(unsigned int n);
    Span(const Span& other);
    Span& operator=(const Span& other);
    ~Span();

    void    addNumber(int n);

    template <typename Iterator>
    void    addRange(Iterator begin, Iterator end)
    {
        for (Iterator it = begin; it != end; ++it)
            addNumber(*it);
    }

    int     shortestSpan() const;
    int     longestSpan() const;
};

#endif
```

</details>

---

## Step 2 — Implement the Orthodox Canonical Form in `Span.cpp`

### Why
The **Orthodox Canonical Form** (constructor, copy constructor, copy-assignment operator, destructor) is mandatory in C++98 for any class that manages resources. Even when the compiler could generate them for free (because `std::vector` handles its own memory), you are required to define them explicitly in 42 projects. It also teaches you to think about **ownership and deep copying**.

### What to do
In `Span.cpp`:
- **Constructor**: initialize `_maxSize` with the parameter and leave `_numbers` empty (the vector default-constructs itself).
- **Copy constructor**: copy both `_maxSize` and `_numbers` from the other object.
- **Copy-assignment operator**: guard against self-assignment, then copy both members; return `*this`.
- **Destructor**: nothing to do (vector cleans itself), but define it.

<details>
<summary>💡 Solution — OCF in Span.cpp</summary>

```cpp
#include "Span.h"

Span::Span(unsigned int n) : _maxSize(n) {}

Span::Span(const Span& other)
    : _maxSize(other._maxSize), _numbers(other._numbers) {}

Span& Span::operator=(const Span& other)
{
    if (this != &other)
    {
        _maxSize = other._maxSize;
        _numbers = other._numbers;
    }
    return *this;
}

Span::~Span() {}
```

</details>

---

## Step 3 — Implement `addNumber()`

### Why
You need to understand **how to guard a container against overflow using exceptions**. In C++, the standard way to signal an unrecoverable error at runtime is to `throw` an exception. `std::runtime_error` (from `<stdexcept>`) is the idiomatic choice for "something went wrong at run time".

> Compare with `std::vector::at()` which throws `std::out_of_range` when you exceed bounds — same idea.

### What to do
- Check if `_numbers.size()` has already reached `_maxSize`.
- If yes, throw a `std::runtime_error` with a descriptive message.
- Otherwise, `push_back` the new number.

> Note: `_numbers.size()` returns `size_t` (unsigned). Compare it with `_maxSize` carefully — both are unsigned so no sign mismatch.

<details>
<summary>💡 Solution — addNumber()</summary>

```cpp
void Span::addNumber(int n)
{
    if (_numbers.size() >= _maxSize)
        throw std::runtime_error("Span is full: cannot add more numbers");
    _numbers.push_back(n);
}
```

</details>

---

## Step 4 — Implement `longestSpan()`

### Why
`longestSpan()` is the **easier** of the two span functions. It teaches you to use `std::min_element` and `std::max_element` — standard algorithms that work on iterators. You'll also practice throwing exceptions when the preconditions aren't met (fewer than 2 elements).

### What to do
- If `_numbers.size()` is less than 2, throw a `std::runtime_error`.
- Use `std::max_element` and `std::min_element` on `_numbers` to find the biggest and smallest values.
- Return the difference: `max - min`.

> The returned iterators must be dereferenced with `*` to get the integer value.

<details>
<summary>💡 Solution — longestSpan()</summary>

```cpp
int Span::longestSpan() const
{
    if (_numbers.size() < 2)
        throw std::runtime_error("Not enough numbers to compute a span");

    int min = *std::min_element(_numbers.begin(), _numbers.end());
    int max = *std::max_element(_numbers.begin(), _numbers.end());
    return max - min;
}
```

</details>

---

## Step 5 — Implement `shortestSpan()`

### Why
`shortestSpan()` is the **harder** function. You can't just compare every pair of numbers — that's $O(n^2)$ and slow on 10,000 numbers. The key insight: **if you sort the array, the shortest gap between any two numbers must be between two adjacent elements**. This reduces the problem to a single $O(n \log n)$ sort followed by an $O(n)$ scan.

This teaches you to:
- Sort a **copy** of the vector (never mutate state in a `const` method).
- Use `std::sort` with iterators.
- Think algorithmically before brute-forcing.

### What to do
- Guard against fewer than 2 elements (throw).
- Copy `_numbers` into a local `std::vector<int> sorted`.
- Call `std::sort(sorted.begin(), sorted.end())`.
- Iterate through `sorted` and track the minimum difference between consecutive elements.
- Return that minimum.

<details>
<summary>💡 Solution — shortestSpan()</summary>

```cpp
int Span::shortestSpan() const
{
    if (_numbers.size() < 2)
        throw std::runtime_error("Not enough numbers to compute a span");

    std::vector<int> sorted(_numbers);
    std::sort(sorted.begin(), sorted.end());

    int shortest = sorted[1] - sorted[0];
    for (size_t i = 2; i < sorted.size(); i++)
    {
        int span = sorted[i] - sorted[i - 1];
        if (span < shortest)
            shortest = span;
    }
    return shortest;
}
```

</details>

---

## Step 6 — Add `addRange()` (template in `Span.h`)

### Why
Adding 10,000 numbers one by one with `addNumber()` is tedious. The subject hints at an **iterator-range** interface: a function that accepts any pair of iterators and adds all values between them. This is exactly how the STL itself works (`std::vector::insert`, `std::copy`, etc.), and learning this pattern is a major goal of CPP08.

> **Why in the header?** Templates must be fully defined at the point of instantiation. Because the compiler generates code for each type `Iterator` you use, the full function body must be visible in the `.h` — you cannot put it inside `.cpp`.

### What to do
- In `Span.h`, inside the class body, define a `template <typename Iterator>` member function `addRange(Iterator begin, Iterator end)`.
- Loop from `begin` to `end` and call `addNumber(*it)` for each element.
- This automatically reuses your overflow guard from Step 3.

> This is already in your `Span.h` skeleton — you just need to fill in the body.

<details>
<summary>💡 Solution — addRange() template</summary>

```cpp
// Inside the class in Span.h:
template <typename Iterator>
void addRange(Iterator begin, Iterator end)
{
    for (Iterator it = begin; it != end; ++it)
        addNumber(*it);
}
```

</details>

---

## Step 7 — Write your tests in `main.cpp`

### Why
Writing tests is not optional — it's how you **prove** your code is correct. A good test suite covers:
1. **Normal path**: expected output.
2. **Edge cases**: 0 elements, 1 element, exactly N elements.
3. **Exception paths**: what happens when you break the rules.
4. **Scale**: 10,000+ numbers to ensure performance.

### What to do
Write a `main()` that:
1. Replicates the subject's example: `Span(5)` with `{6, 3, 17, 9, 11}` → shortest `2`, longest `14`.
2. Tests the "span full" exception.
3. Tests the "too few numbers" exception on both `shortestSpan()` and `longestSpan()`.
4. Creates a `Span(10000)`, generates 10,000 random integers into a `std::vector<int>`, then calls `addRange()` to fill the Span, and prints both spans.

> Use `std::srand(std::time(NULL))` and `std::rand()` for random numbers (C++98 compatible).

<details>
<summary>💡 Solution — main.cpp</summary>

```cpp
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
```

</details>

---

## Step 8 — Build and verify

### Why
Compiling with `-Wall -Wextra -Werror` ensures your code is **warning-free**. Any sloppy code (unused variables, signed/unsigned comparisons, etc.) becomes a hard error. This discipline is enforced at 42.

### What to do
```bash
cd ex01/
make
./span
```

Expected output for the subject example:
```
Shortest: 2
Longest:  14
```

Run with sanitizers to check for memory issues:
```bash
make sanitize
./span
```

---

## Summary checklist

| Step | File | Done? |
|------|------|-------|
| 1 | `Span.h` — class skeleton + includes | ☐ |
| 2 | `Span.cpp` — OCF | ☐ |
| 3 | `Span.cpp` — `addNumber()` | ☐ |
| 4 | `Span.cpp` — `longestSpan()` | ☐ |
| 5 | `Span.cpp` — `shortestSpan()` | ☐ |
| 6 | `Span.h` — `addRange()` template | ☐ |
| 7 | `main.cpp` — tests | ☐ |
| 8 | Build & verify | ☐ |
