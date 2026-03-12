# Exercise 02 — MutantStack: Step-by-Step Guide

---

## Step 1 — Understand what `std::stack` is (and why it can't be iterated)

### Why
Before writing anything, you need to understand the problem you're solving. `std::stack` is a **container adaptor** — it wraps another container (by default `std::deque<T>`) and exposes only a restricted interface: `push`, `pop`, `top`, `size`, `empty`. It deliberately hides the underlying container to enforce stack semantics (LIFO). There is **no** `begin()` or `end()`, so you cannot use range-for, iterators, or STL algorithms on it.

Your goal: add iterator support **without reimplementing a stack from scratch**.

### Key insight
`std::stack` stores its elements in a **protected** member named `c` (the underlying container). Because it is `protected`, a subclass can access it directly. This is the trick that makes the whole exercise possible in C++98.

---

## Step 2 — Design `MutantStack.hpp`

### Why
`MutantStack` should behave exactly like a `std::stack` — so **inherit from it publicly**. You get all its member functions for free. The only thing to add is iterator access through the protected `c` member.

Because `MutantStack` is a template class, everything goes in a single header (or in a `.tpp` included at the bottom of the header). There is no `.cpp` file.

### What to do
In `MutantStack.hpp`:
- `#include <stack>` (gives you `std::stack` and its protected `c`).
- Define `template <typename T> class MutantStack : public std::stack<T>`.
- Implement the **Orthodox Canonical Form**: default constructor, copy constructor, copy-assignment operator, destructor — all delegating to the base class.
- Add `iterator` and `const_iterator` typedefs by reaching into the underlying container type:
  ```cpp
  typedef typename std::stack<T>::container_type::iterator       iterator;
  typedef typename std::stack<T>::container_type::const_iterator const_iterator;
  ```
- Add four methods: `begin()`, `end()`, `begin() const`, `end() const` — each forwarding to `std::stack<T>::c.begin()`/`c.end()`.

> **Why `container_type`?** `std::stack<T>` exposes a typedef `container_type` for the underlying container (the `std::deque<T>` by default). Using it makes your iterator type correct regardless of which container `std::stack` uses internally.

<details>
<summary>💡 Solution — MutantStack.hpp</summary>

```cpp
#pragma once

#include <stack>

template <typename T>
class MutantStack : public std::stack<T>
{
public:
    MutantStack() {}
    MutantStack(const MutantStack &other) : std::stack<T>(other) {}
    MutantStack &operator=(const MutantStack &other)
    {
        if (this != &other)
            std::stack<T>::operator=(other);
        return *this;
    }
    ~MutantStack() {}

    typedef typename std::stack<T>::container_type::iterator       iterator;
    typedef typename std::stack<T>::container_type::const_iterator const_iterator;

    iterator begin() { return std::stack<T>::c.begin(); }
    iterator end()   { return std::stack<T>::c.end();   }

    const_iterator begin() const { return std::stack<T>::c.begin(); }
    const_iterator end()   const { return std::stack<T>::c.end();   }
};
```

</details>

---

## Step 3 — Implement the Orthodox Canonical Form

### Why
Even though `MutantStack` adds no new data members (the base class holds everything), the OCF is required by the 42 norm. Each special member simply delegates to the base class.

- **Default constructor**: calls `std::stack<T>()`  implicitly (empty body is fine).
- **Copy constructor**: explicitly call `std::stack<T>(other)` in the initializer list — this copies the underlying `c` container.
- **Copy-assignment operator**: call `std::stack<T>::operator=(other)` — the base assignment copies `c`. Guard against self-assignment.
- **Destructor**: nothing to do; `std::stack` manages its own container.

---

## Step 4 — Expose iterators via the protected `c` member

### Why
`std::stack` makes its underlying container available as the **protected** member `c`. Protected members are accessible inside a derived class. So from inside `MutantStack`, `std::stack<T>::c` is the raw `std::deque<T>` (or whatever container was chosen), and you can call `.begin()` and `.end()` on it directly.

### What to do
- Add `iterator begin()` returning `std::stack<T>::c.begin()`.
- Add `iterator end()` returning `std::stack<T>::c.end()`.
- Add the two `const` overloads for read-only iteration.

> **Iterator direction**: `c.begin()` points to the **bottom** of the stack, `c.end()` is past the **top**. Iteration goes bottom → top, which matches `std::list` — that's why the outputs of both containers will be identical in the test.

---

## Step 5 — Write your tests in `main.cpp`

### Why
The subject requires you to show that `MutantStack` output is **identical** to a reference container (`std::list` is the natural choice since both store in insertion order and expose `begin()`/`end()`). Two separate test functions make this easy to compare side by side.

### What to do
Write two functions that perform the exact same logical operations:

**`testMutantStack()`** — uses `MutantStack<int>`:
- `push(5)`, `push(17)`, print `top()`, `pop()`, print `size()`.
- `push(3)`, `push(5)`, `push(737)`, `push(0)`.
- Get `begin()` and `end()` iterators, do `++it; --it;`, then iterate and print each value.
- Copy into a plain `std::stack<int>` to prove the constructor is compatible: `std::stack<int> s(mstack);`.

**`testList()`** — same logic using `std::list<int>`:
- Replace `push` → `push_back`, `top()` → `back()`, `pop()` → `pop_back()`.
- Iterate the same way.

The two outputs must be identical.

<details>
<summary>💡 Solution — main.cpp</summary>

```cpp
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
    ++it;
    --it;
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
    ++it;
    --it;
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
```

</details>

---

## Step 6 — Write the Makefile

### Why
Same structure as the previous exercises. Since `MutantStack` is header-only (no `.cpp`), only `main.cpp` needs to be compiled.

### What to do
- `SRCS = ./main.cpp` — no `MutantStack.cpp` needed.
- Standard flags: `-std=c++98 -Wall -Wextra -Werror`.
- Standard rules: `all`, `clean`, `fclean`, `re`.

---

## Expected output

Running `./mutantstack` should print:

```
=== MutantStack ===
17
1
5
3
5
737
0
=== std::list ===
17
1
5
3
5
737
0
```

Both blocks are identical — that's the proof that `MutantStack` iterates identically to a standard sequential container.
