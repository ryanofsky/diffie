#ifndef policy_hpp
#define policy_hpp

#include "general.hpp"
#include "append.hpp"

#ifdef SIMPLE

//////////////////////// Revision 1, No Chaining or Custom Inheritance

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

#else

////////////////////// Revision 2, Chaining and halfway to Custom Inheritance

template<class INHERIT>
struct TopType
{
  void topGo()
  {
    cout << "TOP: Here's the list of classes that will be inherited "
         << typeid(INHERIT).name() << endl;
  };
};

template<>
struct TopType<NullType>
{
  void topGo()
  {
    std::cout << "TOP: Nothing will be inherited " << std::endl;
  };
};

// put vc++ workaround here
template<class POLICY, class ACTUAL, class BASE, class CHAIN>
struct GetPolicy3
{
  typedef typename POLICY::template Policy<ACTUAL, BASE, CHAIN> type;
};


// 
//  Function:   PolicyChainImpl<POLICIES, ACTUAL, EMBEDDED_POLICIES = NullType, INHERITS = NullType>
//  Arguments:  POLICIES is a type-list of policy classes, ACTUAL is the
//              the type of the composite object. The function generates an
//              inheritance chain of instantiated policies having each policy
//              in POLICIES inherit from the policy after it. The return value
//              'Chain' is a list of types on this hierarchy. The
//              last two arguments and the other return value 'EmbeddedChain',
//              hold state and only used on recursive calls.
//
//              returns a list of the types that compose the inheritance chain
//
//              INHERIT_DATA is just big list of plain types to multiply inherit from
//              ChainData of a typelist of typelists of uninstantiated policies
//
// Pseudocode:
//
// if (POLICIES != NullType)
// { /* 1 */
//   policy = POLICIES::Head;
//
//   next = 
//     PolicyChainImpl
//     < POLICIES::Tail,
//       ACTUAL,
//       List<policy:Policies, EMBEDDED_POLICIES>,
//       Append<policy::Inherits, INHERITS>::type
//     >;
//   
//   // instantiate policy
//   object = policy::apply<ACTUAL, next::Chain::Head, next::EmbeddedChain::Head>;
//   
//   // return values
//   Chain = List<object, next::Chain>;
//   EmbeddedChain = next::EmbeddedChain::Tail;
// }
// else
// { /* 2 */
//   if (EMBEDDED_POLICIES == NullType) // cast off thy chains
//   { /* 3 */
//     Chain = List<TopClass<INHERITS>, NullType>;
//     EmbeddedChain = List<NullType, NullType>; // 1-list containing a zero list
//   }
//   else
//   { /* 4 */
//     next = 
//       PolicyChainImpl
//       <
//         EMBEDDED_POLICIES::Head,
//         ACTUAL,
//         EMBEDDED_POLICIES::Tail,
//         INHERITS
//       >;
//
//     // return values
//     Chain = next::Chain;
//     EmbeddedChain = List<next::Chain, next::EmbeddedChain>;
//   }
// }

template<class POLICIES>
struct PolicyChainImpl /* 1 */
{
   template<class ACTUAL, class EMBEDDED_POLICIES, class INHERITS>
   struct apply
   {
     typedef typename POLICIES::Head policy;

     typedef PolicyChainImpl
       < typename POLICIES::Tail                     /* POLICIES */
       >::apply
       < ACTUAL,                                     /* ACTUAL */
         List                                        /* EMBEDDED_POLICIES */
         < typename policy::Policies, 
           EMBEDDED_POLICIES
         >, 
         typename Append<typename policy::Inherits, INHERITS>::type /* INHERITS */
       > next;

     // instantiate policy
     typedef GetPolicy3
       < policy,
         ACTUAL,
         next::Chain::Head,
         next::EmbeddedChain::Head
       >::type object;

     // return values
     typedef List<object, typename next::Chain> Chain;
     typedef typename next::EmbeddedChain::Tail EmbeddedChain;
   };
};

template<class EMBEDDED_POLICIES>
struct PolicyChainImplEnd /* 4 */
{
  template<class ACTUAL, class INHERITS>
  struct apply
  {
    typedef PolicyChainImpl
      < typename EMBEDDED_POLICIES::Head  /* POLICIES */
      >::apply
      < ACTUAL,                           /* ACTUAL */
        typename EMBEDDED_POLICIES::Tail, /* EMBEDDED_POLICIES */
        INHERITS                          /* INHERITS */
      > next;

     // return values
     typedef typename next::Chain Chain;
     typedef List<typename next::Chain, typename next::EmbeddedChain> EmbeddedChain;
  };
};

template<>
struct PolicyChainImplEnd<NullType> /* 3 */
{
  template<class ACTUAL, class INHERITS>
  struct apply
  {
     typedef List<TopType<INHERITS>, NullType> Chain;
     typedef List<NullType, NullType> EmbeddedChain; // 1-list containing a zero list    
  };
};

template<>
struct PolicyChainImpl<NullType> /* 2 */
{
  template<class ACTUAL, class EMBEDDED_POLICIES, class INHERITS>
  struct apply
  {
    typedef PolicyChainImplEnd
      < EMBEDDED_POLICIES        /* EMBEDDED_POLICIES */
      >::apply
      < ACTUAL,                  /* ACTUAL */
        INHERITS                 /* INHERITS */
      > next;
    typedef next::Chain Chain;
    typedef next::EmbeddedChain EmbeddedChain;
  };
};

#endif             

#endif