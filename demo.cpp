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

