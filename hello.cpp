#include <iostream>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// GENERAL STUFF

#define GCC
#ifdef GCC
#define CPP_FORCE_PARTIAL_TARG  , typename EXTRA = Unspecified
#define CPP_FORCE_PARTIAL_ARG   , EXTRA
#define CPP_FORCE_PARTIAL       typename EXTRA
#else
#define CPP_FORCE_PARTIAL_TARG
#define CPP_FORCE_PARTIAL_ARG
#define CPP_FORCE_PARTIAL
#endif

struct NullType;
struct EmptyType{};

struct ThisClassShouldNeverBeAParameter
{
  typedef ThisClassShouldNeverBeAParameter Result;
};

template<class IDENTITY>
struct Specialize
{
  template<class PARAMS, class ACTUAL, class BASE>
  struct Params
  {
    typedef ThisClassShouldNeverBeAParameter Result;
  };
};

template<class HEAD, class TAIL>
struct TypeList
{
  typedef HEAD Head;
  typedef TAIL Tail;
};

/////////////////////////////////////////////////////////////////////////////
// Testing Stuff

template<class ACTUAL, class BASE>
struct UpperPolicyImpl : public BASE
{
  void upperGo()
  {
    ACTUAL * t = (ACTUAL *) this;
    t->UpperMethod();
    t->LowerPolicy::shadow();
  }
  
  void shadow()
  {
    cout << "Upper Shadow" << endl;
  }
};

struct UpperPolicy
{
  typedef UpperPolicy Identity;
  typedef EmptyType Params;
};

template<class ACTUAL, class BASE>
struct LowerPolicyImpl : public BASE
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

struct LowerPolicy
{
  typedef LowerPolicy Identity;
  typedef EmptyType Params;
};

template<>
struct Specialize<UpperPolicy>
{
  template<class PARAMS, class ACTUAL, class BASE>
  struct Params
  {
    typedef UpperPolicyImpl<ACTUAL, BASE> Result;
  };
};

template<>
struct Specialize<LowerPolicy>
{
  template<class PARAMS, class ACTUAL, class BASE>
  struct Params
  {
    typedef LowerPolicyImpl<ACTUAL, BASE> Result;
  };
};

// testing shit

struct Base
{
  void baseGo()
  {
    cout << "Hello from Base" << endl;
  }
};

// this level of indirection to the Specialize class 
// is need by VC6 to avoid error C2510
template<class POLICY, class ACTUAL, class BASE>
struct FriendlySpec
{
  typedef Specialize<typename POLICY::Identity>::Params<typename POLICY::Params, ACTUAL, BASE>::Result Result;
};

template<class UPPER_POLICY, class LOWER_POLICY>
struct PolicyClass : 
  public 

    FriendlySpec
    <
      LOWER_POLICY,
      PolicyClass<UPPER_POLICY, LOWER_POLICY>,
      FriendlySpec
      <
        UPPER_POLICY,     
        PolicyClass<UPPER_POLICY, LOWER_POLICY>,
        Base
      >::Result
    >::Result
{
  typedef
    FriendlySpec
    <
      UPPER_POLICY,  
      PolicyClass<UPPER_POLICY, LOWER_POLICY>,
      Base
    >::Result
    UpperPolicy;

  typedef FriendlySpec
    <
      LOWER_POLICY,
      PolicyClass<UPPER_POLICY, LOWER_POLICY>,
      UpperPolicy
    >::Result
    LowerPolicy;
    
  //typedef UpperPolicy;
  
  void UpperMethod()
  {
    cout << "Hello from PolicyClass for UpperPolicy" << endl;
  }

  void LowerMethod()
  {
    cout << "Hello from PolicyClass for LowerPolicy" << endl;
  }
};

int main()
{
  typedef PolicyClass<UpperPolicy, LowerPolicy> P;
  P p;
  
  p.UpperPolicy::shadow();
  p.LowerPolicy::shadow();
  
  
  
  p.upperGo();
  p.baseGo();
  p.lowerGo();
  
/*
  typedef Specialize<Policy>::Params<NullType, Base1>::Result PolicyBase1;
  typedef Specialize<Policy>::Params<NullType, Base2>::Result PolicyBase2;  

  PolicyBase1 a;
  PolicyBase2 b;

  cout << typeid(a).name() << endl;
*/
  
  //cout << typeid(p1).name() << endl;  
  
  return 0;
}
