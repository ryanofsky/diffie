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
    t->LowerType::shadow();
  }
  
  void shadow()
  {
    cout << "Upper Shadow" << endl;
  }
  
  void crawl()
  {
    typedef BASE Parent;
    cout << __FILE__ << ":" << __LINE__ << " " << typeid(Parent).name() << endl;
    Parent::crawl();
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
  
  void crawl56()
  {
    typedef BASE Parent;
    cout << __FILE__ << ":" << __LINE__ << " " << typeid(Parent).name() << endl;
    Parent::crawl();
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

template<class ACTUAL, class TYPE_LIST>
struct PolicyHelper;

template<class TYPE_LIST>
struct PolicyHelpImpl
{
  template<class ACTUAL>
  struct Actual
  {
    typedef PolicyHelper<ACTUAL, typename TYPE_LIST::Tail> Next;
    typedef FriendlySpec<typename TYPE_LIST::Head, ACTUAL, Next >::Result Policy;
  };
};

template<>
struct PolicyHelpImpl<NullType>
{
  template<class ACTUAL>
  struct Actual
  {
    typedef EmptyType Policy;
    typedef EmptyType Next;
  };
};

template<class ACTUAL, class TYPE_LIST>
struct PolicyHelper : PolicyHelpImpl<TYPE_LIST>::Actual<ACTUAL>::Policy
{
  typedef PolicyHelpImpl<TYPE_LIST>::Actual<ACTUAL> Impl;
  typedef Impl::Policy Policy;
  typedef Impl::Next Next;

  void crawl()
  {
    typedef Policy Parent;
    cout << __FILE__ << ":" << __LINE__ << " " << typeid(Parent).name() << endl;
    //this->crawl56();    
  
  }
};

template<class UPPER_POLICY, class LOWER_POLICY>
struct PolicyClass : 
  public 
    PolicyHelper
    <
      PolicyClass<UPPER_POLICY, LOWER_POLICY>,
      TypeList< UPPER_POLICY, TypeList<LOWER_POLICY, NullType> >
    >
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
  
  void crawl()
  {
    typedef UpperType Parent;
    cout << __FILE__ << ":" << __LINE__ << " " << typeid(Parent).name() << endl;
    Parent::crawl();
  };
};

int main()
{
  typedef PolicyClass<UpperPolicy, LowerPolicy> P;
  P p;

  p.crawl();
  
  cout << "********************************************" << endl << endl
       << typeid(P::UpperType).name() << endl;

  p.UpperType::shadow();
  p.LowerType::shadow();
  
  
  
  p.upperGo();
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
