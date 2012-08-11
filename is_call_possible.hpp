#pragma once

// inspired by Roman Perepelitsa's presentation from comp.lang.c++.moderated
// based on the implementation here: http://www.rsdn.ru/forum/cpp/2759773.1.aspx

#define DEFINE_HAS_MEMBER_FUNCTION(trait_name, member_function_name)          \
template <typename T>                                                         \
class trait_name                                                              \
{                                                                             \
   class yes { char m; };                                                     \
   class no { yes m[2]; };                                                    \
   struct base_mixin                                                          \
   {                                                                          \
     void member_function_name();                                             \
   };                                                                         \
   struct base : public T, public base_mixin {};                              \
   template <typename U, U t>  class helper{};                                \
   template <typename U>                                                      \
   static no deduce(U*, helper<void (base_mixin::*)(), &U::operator()>* = 0); \
   template <typename U> static yes deduce(...);                              \
public:                                                                       \
   static const bool value = sizeof(yes) == sizeof(deduce<T>(0));             \
}; 

DEFINE_HAS_MEMBER_FUNCTION(has_call_operator, operator())

namespace is_call_possible_detail
{ 
   template <typename T> class void_exp_result {}; 

   template <typename T, typename U> 
   U const& operator,(U const&, void_exp_result<T>); 

   template <typename T, typename U> 
   U& operator,(U&, void_exp_result<T>); 

   template <typename src_type, typename dest_type> 
   struct clone_constness 
   { 
     typedef dest_type type; 
   }; 

   template <typename src_type, typename dest_type> 
   struct clone_constness<const src_type, dest_type> 
   { 
     typedef const dest_type type; 
   }; 
} 

#define DEFINE_IS_CALL_POSSIBLE(trait_name, member_function_name)                                                       \
namespace trait_name##_detail                                                                                           \
{                                                                                                                       \
DEFINE_HAS_MEMBER_FUNCTION(has_member, member_function_name)                                                            \
}                                                                                                                       \
                                                                                                                        \
template <typename T, typename Signature>                                                                               \
struct trait_name                                                                                                       \
{                                                                                                                       \
  private:                                                                                                              \
   class yes {};                                                                                                        \
   class no { yes m[2]; };                                                                                              \
   struct derived : public T                                                                                            \
   {                                                                                                                    \
     using T::member_function_name;                                                                                     \
     no member_function_name(...) const;                                                                                \
   };                                                                                                                   \
                                                                                                                        \
   typedef typename is_call_possible_detail::clone_constness<T, derived>::type derived_type;                            \
                                                                                                                        \
   template <typename U, typename Result>                                                                               \
   struct return_value_check                                                                                            \
   {                                                                                                                    \
     static yes deduce(Result);                                                                                         \
     static no deduce(...);                                                                                             \
     static no deduce(no);                                                                                              \
     static no deduce(is_call_possible_detail::void_exp_result<T>);                                                     \
   };                                                                                                                   \
                                                                                                                        \
   template <typename U>                                                                                                \
   struct return_value_check<U, void>                                                                                   \
   {                                                                                                                    \
     static yes deduce(...);                                                                                            \
     static no deduce(no);                                                                                              \
   };                                                                                                                   \
                                                                                                                        \
   template <bool has_the_member_of_interest, typename F>                                                               \
   struct impl                                                                                                          \
   {                                                                                                                    \
     static const bool value = false;                                                                                   \
   };                                                                                                                   \
                                                                                                                        \
   template <typename Result, typename Arg>                                                                             \
   struct impl<true, Result(Arg)>                                                                                       \
   {                                                                                                                    \
     static derived_type &test_me;                                                                                      \
     static Arg &arg;                                                                                                   \
                                                                                                                        \
     static const bool value =                                                                                          \
       sizeof(                                                                                                          \
            return_value_check<T, Result>::deduce(                                                                      \
             (test_me.member_function_name(arg), is_call_possible_detail::void_exp_result<T>())                         \
                         )                                                                                              \
            ) == sizeof(yes);                                                                                           \
   };                                                                                                                   \
                                                                                                                        \
   template <typename Result, typename Arg1, typename Arg2>                                                             \
   struct impl<true, Result(Arg1,Arg2)>                                                                                 \
   {                                                                                                                    \
     static derived_type &test_me;                                                                                      \
     static Arg1 &arg1;                                                                                                 \
     static Arg2 &arg2;                                                                                                 \
                                                                                                                        \
     static const bool value =                                                                                          \
       sizeof(                                                                                                          \
            return_value_check<T, Result>::deduce(                                                                      \
             (test_me.member_function_name(arg1,arg2), is_call_possible_detail::void_exp_result<T>())                   \
                         )                                                                                              \
            ) == sizeof(yes);                                                                                           \
   };                                                                                                                   \
                                                                                                                        \
   template <typename Result, typename Arg1, typename Arg2, typename Arg3>                                              \
   struct impl<true, Result(Arg1,Arg2,Arg3)>                                                                            \
   {                                                                                                                    \
     static derived_type &test_me;                                                                                      \
     static Arg1 &arg1;                                                                                                 \
     static Arg2 &arg2;                                                                                                 \
     static Arg3 &arg3;                                                                                                 \
                                                                                                                        \
     static const bool value =                                                                                          \
       sizeof(                                                                                                          \
            return_value_check<T, Result>::deduce(                                                                      \
             (test_me.member_function_name(arg1,arg2,arg3), is_call_possible_detail::void_exp_result<T>())              \
                         )                                                                                              \
            ) == sizeof(yes);                                                                                           \
   };                                                                                                                   \
                                                                                                                        \
   template <typename Result, typename Arg1, typename Arg2, typename Arg3, typename Arg4>                               \
   struct impl<true, Result(Arg1,Arg2,Arg3,Arg4)>                                                                       \
   {                                                                                                                    \
     static derived_type &test_me;                                                                                      \
     static Arg1 &arg1;                                                                                                 \
     static Arg2 &arg2;                                                                                                 \
     static Arg3 &arg3;                                                                                                 \
     static Arg4 &arg4;                                                                                                 \
                                                                                                                        \
     static const bool value =                                                                                          \
       sizeof(                                                                                                          \
            return_value_check<T, Result>::deduce(                                                                      \
             (test_me.member_function_name(arg1,arg2,arg3,arg4), is_call_possible_detail::void_exp_result<T>())         \
                         )                                                                                              \
            ) == sizeof(yes);                                                                                           \
   };                                                                                                                   \
                                                                                                                        \
  public:                                                                                                               \
    static const bool value = impl<trait_name##_detail::has_member<T>::value, Signature>::value; \
}; 

