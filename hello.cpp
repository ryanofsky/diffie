#include <iostream>
#include <boost/type_traits.hpp>

// Workaround for the "heinous VC typedef dependent template parameter bug"
// mentioned in:
// 
//   http://lists.boost.org/MailArchives/boost/msg13628.php
//   http://lists.boost.org/MailArchives/boost/msg20915.php
//   http://lists.boost.org/MailArchives/boost/msg21642.php
//   http://lists.boost.org/MailArchives/boost/msg21647.php
//
//



template<class T>
struct false_c
{
  enum { value = false };
};

struct Prototype
{
  template<class a, class b> struct Policy;
};

template<typename POLICY, typename PROTOTYPE>
struct Workaround
{
  template<bool>
  struct Helper;

  template<>
  struct Helper<true> : Prototype
  {
  };

  template<>
  struct Helper<false> : public POLICY
  {
  };  



  typedef Helper<false_c<POLICY>::value> type;
};

struct LowerPolicy
{
  template<class ACTUAL, class Base>
  struct Policy
  {
    void go()
    {
      cout << typeid(ACTUAL).name() << endl;
    }
  };
};

using namespace std;

template<class T>
struct abc
{
  typedef Workaround<T, Prototype>::type t2;
  typedef t2::template Policy<T, int> dfg;
};

void main()
{
  typedef abc<LowerPolicy>::dfg sdf;
  sdf k;
  k.go();
};

/*

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

template<class P>
struct GetP
{
  typedef P::Policy Result;
};

template<>
struct GetP<int>
{
  typedef EmptyType Result;
};

template<>
struct GetP<EmptyType>
{
  typedef EmptyType Result;
};

template<class P>
struct GetN
{
  typedef P::Next Result;
};

template<>
struct GetN<int>
{
  typedef EmptyType Result;
};


template<class TYPE_LIST>
struct PolicyHelper
{
  template<class ACTUAL>
  struct Actual
  {
    typedef PolicyHelper<typename TYPE_LIST::Tail>::Actual<ACTUAL> Next;
    typedef FriendlySpec<typename TYPE_LIST::Head, ACTUAL, GetP<Next>::Result >::Result Policy;
  };
};

template<>
struct PolicyHelper<NullType>
{
  template<class ACTUAL>
  struct Actual
  {
    typedef EmptyType Next;
    typedef EmptyType Policy;
  };
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


template<class ACTUAL, class TYPE_LIST>
struct FriendlyPolicyHelper
{
  typedef PolicyHelper<TYPE_LIST>::Actual<ACTUAL> Helper;
  //typedef typename Helper::Policy Policy;
  typedef GetP<Helper>::Result Policy;

  //typedef Helper::Next Next;
  typedef GetN<Helper>::Result Next;
};



template<class UPPER_POLICY, class LOWER_POLICY>
struct PolicyClass : public 
  FriendlyPolicyHelper
  <
    PolicyClass<UPPER_POLICY, LOWER_POLICY>,
    TypeList< UPPER_POLICY, TypeList<LOWER_POLICY, NullType> >
  >::Policy
{
  typedef
    FriendlyPolicyHelper
    <
      PolicyClass<UPPER_POLICY, LOWER_POLICY>,
      TypeList< UPPER_POLICY, TypeList<LOWER_POLICY, NullType> >
    >::Helper Helper;
  
  
  //typedef Helper::Policy UpperType;
  typedef GetP<Helper>::Result UpperType;

  //typedef Helper::Next::Policy LowerType;
  typedef GetP<GetN<Helper>::Result>::Result LowerType;

  void UpperMethod()
  {
    cout << "Hello from PolicyClass for UpperPolicy" << endl;
  }

  void LowerMethod()
  {
    cout << "Hello from PolicyClass for LowerPolicy" << endl;
  }
};

*/
/*
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

*/

