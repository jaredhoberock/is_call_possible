#include "is_call_possible.hpp"
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <iostream>

// define a trait to check for the possibility of calling operator() on a given type
DEFINE_IS_CALL_POSSIBLE(is_call_possible, operator())

// define a trait to check for the possibility of calling baz on a given type
DEFINE_IS_CALL_POSSIBLE(is_baz_possible, baz);

// test cases
struct foo 
{ 
   void operator()(double) const {} 
   void operator()(std::string) const {} 
}; 

struct bar : foo
{
   int operator()(int)
   {
     return 0;
   }

   float baz(std::vector<int>)
   {
     return 13;
   }
};

int main() 
{ 
  // foo has operator() which takes a double and returns void
  assert((is_call_possible<foo, void(double)>::value)); 

  // foo has operator() which takes int and returns void
  assert((is_call_possible<foo, void(int)>::value)); 

  // foo has operator() which can convert const char * to string and return void
  assert((is_call_possible<foo, void(const char *)>::value)); 

  // foo has no operator() which can take a void *
  assert((!is_call_possible<foo, void(void *)>::value)); 
  
  // bar inherits foo's operator() which takes a double and returns void
  assert((is_call_possible<bar, void(double)>::value)); 

  // bar has operator() which takes int and converts its int result to void
  assert((is_call_possible<bar, void(int)>::value)); 

  // bar has operator() which takes int but cannot convert const char * to int
  assert((!is_call_possible<bar, void(const char *)>::value)); 

  // neither bar nor foo has an operator() which can take void *
  assert((!is_call_possible<bar, void(void *)>::value)); 

  // bar has operator() which takes int and returns int
  assert((is_call_possible<bar, int(int)>::value)); 

  // foo has no baz member function
  assert((!is_baz_possible<foo, float(std::vector<int>)>::value));

  // bar has member function baz which takes std::vector<int> and returns float
  assert((is_baz_possible<bar, float(std::vector<int>)>::value));

  // bar::baz can't be called with std::list<int>
  assert((!is_baz_possible<bar, float(std::list<int>)>::value));
  
  return 0;
} 

