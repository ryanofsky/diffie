#include <iostream>
#include <typeinfo>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// GENERAL STUFF

struct NullType;
struct EmptyType{};

template<class HEAD, class TAIL>
struct List
{
  typedef HEAD Head;
  typedef TAIL Tail;
};

template<class INHERIT>
class TopType
{
  void topGo()
  {
    cout << "TOP: Here's the list of classes that will be inherited "
         << typeid(INHERIT).name() << endl;
  };
};


// Function:     AppendImpl(LIST1, LIST2)
//
// Purpose:      Appends LIST2 to LIST1 by returning a copy of LIST1 with its 
//               null terminator replaced by LIST2
//
// Requirements: LIST1 and LIST2 are typelists (although either or both can be
//               null)
//
// Pseudocode:   if (LIST1 != NullType)
//                 return List<LIST1::Head, AppendImpl(LIST1::Tail, LIST2)
//               else
//                 return LIST2;

template<class LIST1>
struct AppendImpl
{
  template<class LIST2>
  struct apply
  {
    typedef List<LIST1::Head, AppendImpl<LIST1::Tail>::apply<LIST2>::type > type;
  };
};

template<>
struct AppendImpl<NullType>
{
  template<class LIST2>
  struct apply
  {
    typedef LIST2 type;
  };
};

// Append the LIST2 to LIST1.
// Procedure: 

template<class LIST1, class LIST2>
struct Append
{
  typedef AppendImpl<LIST1>::apply<LIST2>::type type;
};

template<class HEAD, class TAIL>
struct TypeList
{
  typedef HEAD Head;
  typedef TAIL Tail;
};

// put vc++ workaround here
template<class POLICY, class ACTUAL, class BASE>
struct GetPolicy2
{
  typedef typename POLICY::template Policy<ACTUAL, BASE> type;
};

//

template<class TYPE_LIST>
struct PolicyChainImpl
{  
  template<class ACTUAL>
  struct apply
  {
    typedef PolicyChainImpl<typename TYPE_LIST::Tail>::template apply<ACTUAL> Next;
    typedef GetPolicy2<typename TYPE_LIST::Head, ACTUAL, typename Next::Policy>::type Policy;
  };
};

template<>
struct PolicyChainImpl<NullType>
{
  template<class ACTUAL>
  struct apply
  {
    typedef EmptyType Next;
    typedef EmptyType Policy;
  };
};

template<class ACTUAL, class TYPE_LIST>
struct PolicyChain
{
  typedef PolicyChainImpl<TYPE_LIST>::apply<ACTUAL> Helper;
  typedef typename Helper::Policy Policy;
  typedef Helper::Next Next;
};

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


int main()
{
  typedef List<int, List<char, NullType> > List1;
  typedef List<double, List< float, NullType> > List2;
  typedef Append<List1, List2>::type Result;
  
  cout << "Result is: " << typeid(Result).name() << endl;
  
  return 0;
  
  
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

