is_call_possible
================

C++03 functionality for checking for the existence of a member function with a given name and signature.

Inspired by Roman Perepelitsa's presentation from [`comp.lang.c++.moderated`](https://groups.google.com/d/msg/comp.lang.c++.moderated/T3x6lvmvvkQ/mfY5VTDJ--UJ) which was based on the [original implementation here](http://www.rsdn.ru/forum/cpp/2759773.1.aspx).

Demo
----

```c++
#include "is_call_possible.hpp"
#include <cassert>
#include <string>
#include <iostream>

// define a trait to check for the possibility of calling operator() on a given type
DEFINE_IS_CALL_POSSIBLE(is_call_possible, operator())

// test cases
struct Foo 
{ 
   void operator()(double) const {} 
   void operator()(std::string) const {} 
}; 

struct Bar : Foo
{
   int operator()(int)
   {
     return 0;
   }
};

int main() 
{ 
  // Foo has operator() which takes a double and returns void
  assert((is_call_possible<Foo, void(double)>::value)); 

  // Foo has operator() which takes int and returns void
  assert((is_call_possible<Foo, void(int)>::value)); 

  // Foo has operator() which can convert const char * to string and return void
  assert((is_call_possible<Foo, void(const char *)>::value)); 

  // Foo has no operator() which can take a void *
  assert((!is_call_possible<Foo, void(void *)>::value)); 
  
  // Bar inherits Foo's operator() which takes a double and returns void
  assert((is_call_possible<Bar, void(double)>::value)); 

  // Bar has operator() which takes int and converts its int result to void
  assert((is_call_possible<Bar, void(int)>::value)); 

  // Bar has operator() which takes int but cannot convert const char * to int
  assert((!is_call_possible<Bar, void(const char *)>::value)); 

  // Neither Bar nor Foo has an operator() which can take void *
  assert((!is_call_possible<Bar, void(void *)>::value)); 

  // Bar has operator() which takes int and returns int
  assert((is_call_possible<Bar, int(int)>::value)); 
  
  return 0;
} 
```
