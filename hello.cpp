#pragma warning(disable:4786)
#pragma warning(disable:4503)

#include <iostream>
using namespace std;

#include "general.hpp"
#include "policy.hpp"

struct P1
{
  typedef NullType Policies;
  typedef NullType Inherits;
  
  template<class ACTUAL, class BASE, class CHAIN>           
  struct Policy : public WORKAROUND_InheritFrom(BASE)
  {
    void p1Go()
    {
      ACTUAL * t = (ACTUAL *) this;
      t->print("Hello from P1");
      //t->c2::p2Go();
    };
  };                                           
};

struct Stupid
{
  void stupidGo()
  {
    cout << "Helo from stupid" << endl;
  }
};

struct Dumb
{
  void dumbGo()
  {
    cout << "Helo from dumb" << endl;
  }
};

struct Slow
{
  void slowGo()
  {
    cout << "Helo from slow" << endl;
  }
};

struct P4
{
  typedef NullType Policies;
  typedef List<Slow, List<Dumb, List<Stupid, NullType> > > Inherits;

  template<class ACTUAL, class BASE, class CHAIN>           
  struct Policy : public WORKAROUND_InheritFrom(BASE)
  {
    void p4Go()
    {
      ACTUAL * t = (ACTUAL *) this;
      t->print("Hello from P4");
      t->topGo();
      
      cout << "TOP: Here's the class that was inherited "
        << typeid(InheritFrom<Inherits>::type).name() << endl;        
	  //t->Dumb::dumbGo();
    };                                             
  };                                           
};

struct P3
{
  typedef List<P4, NullType> Policies;
  typedef NullType Inherits;
  
  template<class ACTUAL, class BASE, class CHAIN>           
  struct Policy : public WORKAROUND_InheritFrom(BASE)
  {
    typedef CHAIN::Head p4;
    void p3Go()
    {
      ACTUAL * t = (ACTUAL *) this;
      t->print("Hello from P3");
      t->p4::p4Go();
    };                                             
  };                                           
};

struct P2
{
  typedef List<P3, NullType> Policies;
  typedef NullType Inherits;

  template<class ACTUAL, class BASE, class CHAIN>           
  struct Policy : public WORKAROUND_InheritFrom(BASE)
  {
    typedef CHAIN::Head p3;
    void p2Go()
    {
      ACTUAL * t = (ACTUAL *) this;
      t->print("Hello from P2");
      t->p3::p3Go();
    };                                             
  };                                           
};

#define CALL_PolicyChain(POLICIES, ACTUAL) PolicyChainImpl<POLICIES>::template apply<ACTUAL, NullType, NullType>

template<class ACTUAL, class POLICIES>
struct PolicyHelper 
  : public TGET_Head(TGET_Chain(CALL_PolicyChain(POLICIES, ACTUAL)))
{
  typedef TGET_Chain(CALL_PolicyChain(POLICIES, ACTUAL)) Chain;
};

struct Bottom : public PolicyHelper<Bottom, List< P1, List<P2, NullType> > >
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
  b.p2Go();
  return 0;
}
