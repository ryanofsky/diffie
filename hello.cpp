#include <iostream>
#include <typeinfo>

#include "general.hpp"
#include "policy.hpp"

using namespace std;

struct P1
{
  typedef NullType Policies;
  typedef NullType Inherits;
  
  template<class ACTUAL, class BASE, class CHAIN>           
  struct Policy : public BASE                  
  {
    void p1Go()
    {
      ACTUAL * t = (ACTUAL *) this;
      t->print("Hello from P1");
      t->c2::p2Go();
    };
  };                                           
};

struct P2
{
  typedef NullType Policies;
  typedef NullType Inherits;

  template<class ACTUAL, class BASE, class CHAIN>           
  struct Policy : public BASE                  
  {
    void p2Go()
    {
      ACTUAL * t = (ACTUAL *) this;
      t->print("Hello from P2");
    };                                             
  };                                           
};

template<class ACTUAL, class POLICIES>
struct PolicyHelper : PolicyChainImpl<POLICIES>::apply<ACTUAL, NullType, NullType>::Chain::Head
{
  typedef PolicyChainImpl<POLICIES>::apply<ACTUAL, NullType, NullType>::Chain Chain;
};

struct Bottom : PolicyHelper<Bottom, List< P1, List<P2, NullType> > >
{
  typedef Chain::Head c1;
  typedef Chain::Tail::Head c2;
  void print(const char * str)
  {
    cout << str << endl;
  }
};

int main()
{
  Bottom b;
  b.p1Go();
  return 0;
}