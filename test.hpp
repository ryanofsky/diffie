#ifndef test_hpp
#define test_hpp

#include <iostream>

/////////////////////////////////////////////////////////////////////////////
// Testing Stuff

struct UpperPolicy
{
  typedef NullType Inherit;
  typedef NullType Chain;

  template<class ACTUAL, class BASE>
  struct Policy : public BASE
  {
    void upperGo()
    {
      ACTUAL * t = (ACTUAL *) this;
      t->UpperMethod();
      t->LowerType::shadow();
    }
    
    void shadow()
    {
      cout << "Upper Shadow" << endl;
    }
  };
};

struct MiddlePolicy
{
  typedef NullType Inherit;
  typedef NullType Chain;
  
  template<class ACTUAL, class BASE>
  struct Policy : public BASE
  {
    void middleGo()
    {
      cout << "Hello from middle policy" << endl;
    };  
  };
};

struct LowerPolicy
{
  typedef NullType Inherit;
  typedef NullType Chain;

  template<class ACTUAL, class BASE>
  struct Policy : public BASE
  {
    void lowerGo()
    {
      ACTUAL * t = (ACTUAL *) this;
      cout << "Lower: My BASE is " << typeid(BASE).name() << endl;
      t->LowerMethod();
    }
    
    void shadow()
    {
      cout << "Lower Shadow" << endl;
    }
  };
};

/////////////////////////////////////////////////////////////////////////////
// More Testing Stuff

template<class UPPER_POLICY, class LOWER_POLICY>
struct PolicyClass : public 
  PolicyChain
  <
    PolicyClass<UPPER_POLICY, LOWER_POLICY>,
    TypeList< UPPER_POLICY, TypeList<LOWER_POLICY, NullType> >
  >::Policy
{
  typedef
    PolicyChain
    <
      PolicyClass<UPPER_POLICY, LOWER_POLICY>,
      TypeList< UPPER_POLICY, TypeList<LOWER_POLICY, NullType> >
    >::Helper Helper;
  
  typedef Helper::Policy UpperType;
  typedef Helper::Next::Policy LowerType;

  void UpperMethod()
  {
    cout << "Hello from PolicyClass for UpperPolicy" << endl;
  }

  void LowerMethod()
  {
    cout << "Hello from PolicyClass for LowerPolicy" << endl;
  }
};

using namespace std;

void test()
{
  typedef PolicyClass<UpperPolicy, LowerPolicy> P;

  P p;

  cout << "********************************************" << endl << endl
       << typeid(P::UpperType).name() << endl;

  p.UpperType::shadow();
  p.LowerType::shadow();
  
  p.upperGo();
  p.lowerGo();
  
};

#endif