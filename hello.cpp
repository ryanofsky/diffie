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


// Function:     Append(LIST1, LIST2)
//
// Purpose:      Appends LIST2 to LIST1 by returning a copy of LIST1 with its 
//               null terminator replaced by LIST2
//
// Requirements: LIST1 and LIST2 are typelists (although either or both can be
//               null)
//
// Pseudocode:   if (LIST2 == NullType) // optimization
//                 return LIST1;
//               else 
//                 return Append1(LIST1, LIST2);

// Function:     Append1(LIST1, LIST2)
// Pseudocode:   if (LIST1 != NullType)
//                 return List<LIST1::Head, Append1(LIST1::Tail, LIST2)
//               else
//                 return LIST2;
//
//              The first if (branch) isn't needed for correctness, it
//              just obviates the need for a list traversal when
//              there is nothing to append.
//
//              Run time is proportional to the size of the first
//              list, except in the special case where the the
//              second list is empty, when the runing time will
//              be constant. Generally, it is more efficient
//              to append a long list to a short list than
//              a short list to a long list.

template<class LIST1>
struct AppendImpl2
{
  template<class LIST2>
  struct apply
  {
    typedef List<LIST1::Head, AppendImpl2<LIST1::Tail>::apply<LIST2>::type > type;
  };
};

template<>
struct AppendImpl2<NullType>
{
  template<class LIST2>
  struct apply
  {
    typedef LIST2 type;
  };
};

template<class LIST2>
struct AppendImpl1
{
  template<class LIST1>
  struct apply
  {
    typedef AppendImpl2<LIST1>::apply<LIST2>::type type;
  };
};

template<>
struct AppendImpl1<NullType>
{
  template<class LIST1>
  struct apply
  {
    typedef LIST1 type;
  };
};


// Append the LIST2 to LIST1.
// Procedure: 

template<class LIST1, class LIST2>
struct Append
{
  typedef AppendImpl1<LIST1>::apply<LIST2>::type type;
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

// Function:     PolicyChainImpl<TYPE_LIST, ACTUAL>
// Purpose:
// Requirements: TYPE_LIST is a typelist (possibly NULL), ACTUAL
//               should be the type of the policy class which
//               is at the very bottom of the inheritance chain.
//               the value of ACTUAL is ignored by this function,
//               it is only passed as an argument each policy class
//               on the chain. The TYPE_LIST must consist of policy
//               classes only. All policy classes will have inner
//               templated classes that look like:
//                 
//                 template<class ACTUAL, class BASE>
//                 struct Policy : public BASE
//                 {
//                   // policies' methods, typedefs, and members
//                   // defined here...
//                 };

//////////////////////// Revision 1, No Chaining or Custom Inheritance
//
// Pseudocode:
// if (TYPE_LIST == NullType)
// {
//   NextList = PolicyChainImpl<TYPE_LIST::Tail, ACTUAL>;
//   Policy = TYPE_LIST::Head::apply<ACTUAL, Next::Head>;
//   return List<Policy, NextList>;
// }
// else
//   return List<TopClass, NullType>;
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
  typedef typename Helper::Policy type;
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
  
 /* 
  typedef PolicyClass<UpperPolicy, LowerPolicy> P;

  P p;

  cout << "********************************************" << endl << endl
       << typeid(P::UpperType).name() << endl;

  p.UpperType::shadow();
  p.LowerType::shadow();
  
  p.upperGo();
  p.lowerGo();
*/
  return 0;
}