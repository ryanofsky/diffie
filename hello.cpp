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

// this level of indirection to the Specialize class 
// is need by VC6 to avoid error C2510
template<class POLICY, class ACTUAL, class BASE>
struct FriendlySpec
{
  typedef Specialize<typename POLICY::Identity>::Params<typename POLICY::Params, ACTUAL, BASE>::Result Result;
};

template<class ACTUAL, class TYPE_LIST>
struct PolicyHelper;

template<class TYPE_LIST>
struct PolicyHelpImpl
{
  template<class ACTUAL>
  struct Actual
  {
    typedef PolicyHelper<ACTUAL, typename TYPE_LIST::Tail> NextImpl;
    typedef FriendlySpec<typename TYPE_LIST::Head, ACTUAL, NextImpl::Policy >::Result PolicyImpl;
  };
};

template<>
struct PolicyHelpImpl<NullType>
{
  template<class ACTUAL>
  struct Actual
  {
    typedef EmptyType PolicyImpl;
    typedef EmptyType NextImpl;
  };
};

template<class ACTUAL, class TYPE_LIST>
struct PolicyHelper 
{
  typedef PolicyHelpImpl<TYPE_LIST>::Actual<ACTUAL> Impl;
  typedef typename Impl::PolicyImpl Policy;
  typedef typename Impl::NextImpl Next;
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
    t->LowerType::shadow();
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

template<>
struct Specialize<UpperPolicy>
{
  template<class PARAMS, class ACTUAL, class BASE>
  struct Params
  {
    typedef UpperPolicyImpl<ACTUAL, BASE> Result;
  };
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
struct Specialize<LowerPolicy>
{
  template<class PARAMS, class ACTUAL, class BASE>
  struct Params
  {
    typedef LowerPolicyImpl<ACTUAL, BASE> Result;
  };
};

template<class UPPER_POLICY, class LOWER_POLICY>
struct PolicyClass : 
  public 
    PolicyHelper
    <
      PolicyClass<UPPER_POLICY, LOWER_POLICY>,
      TypeList< UPPER_POLICY, TypeList<LOWER_POLICY, NullType> >
    >::Policy
{
  typedef
    PolicyHelper
    <
      PolicyClass<UPPER_POLICY, LOWER_POLICY>,
      TypeList< UPPER_POLICY, TypeList<LOWER_POLICY, NullType> >
    > Helper;

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

int main()
{
  typedef PolicyClass<UpperPolicy, LowerPolicy> P;
  P p;

  cout << "********************************************" << endl << endl
       << typeid(P::UpperType).name() << endl;

  p.UpperType::shadow();
  p.LowerType::shadow();
  
  p.upperGo();
  p.lowerGo();

  return 0;
}



