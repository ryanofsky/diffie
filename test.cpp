#include <iostream>

class Def;

#define GCC
#ifdef GCC
#define CPP_FORCE_PARTIAL_TARG  , typename EXTRA
#define CPP_FORCE_PARTIAL_ARG   , EXTRA
#define CPP_FORCE_PARTIAL       typename EXTRA
#else
#define CPP_FORCE_PARTIAL_TARG
#define CPP_FORCE_PARTIAL_ARG
#define CPP_FORCE_PARTIAL
#endif

template<typename T>
class Abc
{
public:

   void hello()
   {
     T t;
     cout << t;  
   }

  template<typename U CPP_FORCE_PARTIAL_TARG>
  class Ok
  {
public:
    void hello()
    {
      T t;
      U u;
      cout << t << u;  
    }
  };
  
  template<CPP_FORCE_PARTIAL>
  class Ok<Def CPP_FORCE_PARTIAL_ARG>
  {
public:
    void hello()
    {
      T t;
      cout << t;  
    }
  };
};

void main()
{
  //Invader<int> abc;
  //abc.go();
};