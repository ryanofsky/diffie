#ifndef policy_hpp
#define policy_hpp

#include "typelist.hpp"
#include "append.hpp"

template<typename A, typename B>
struct DoubleInherit : public A, public B
{};

//  Function:   InheritFrom<LIST>
//  Arguments:  LIST is a type-list of classes or structs to inherit from.
//  Return:     The type returned will be an empty struct that inherits from
//              all the types on the list. If the list is only one element
//              long, single inheritance will be used.//
// Pseudocode:  function InheritFromList(HEAD, TAIL)
//              {
//                if (TAIL == NULL)
//                  return HEAD;
//                else
//                  return DoubleInherit<A, InheritFromList(TAIL::item, TAIL::next)>
//              }
//
//              if (LIST == NULL)
//                return EmptyType;
//              else
//                return InheritFromList(LIST::item, LIST::next)

template<typename TAIL>
struct InheritFromList
{
  template<typename HEAD>
  struct apply
  {
    typedef typename TGET_1apply(InheritFromList<typename TAIL::next>, typename TAIL::item) next;
    typedef DoubleInherit<HEAD, typename next::type> type;
  };
};

template<>
struct InheritFromList<NullType>
{
  template<typename HEAD>
  struct apply
  {
    typedef HEAD type;
  };
};

template<typename LIST>
struct InheritFrom
{
  typedef typename InheritFromList<typename LIST::next>::template apply<typename LIST::item>::type type;
};

template<>
struct InheritFrom<NullType>
{
  typedef EmptyType type;
};

template<class INHERIT>
struct TopType : public InheritFrom<INHERIT>::type
{
  void topGo()
  {
    cout << "TOP: Here's the list of classes that will be inherited "
         << typeid(INHERIT).name() << endl;

    cout << "TOP: Here's the class that was inherited "
         << typeid(typename InheritFrom<INHERIT>::type).name() << endl;
  };
};

template<>
struct TopType<NullType>
{
  void topGo()
  {
    cout << "TOP: Nothing will be inherited " << std::endl;
  };
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
//   policy = POLICIES::item;
//
//   next =
//     PolicyChainImpl
//     < POLICIES::next,
//       ACTUAL,
//       List<policy:Policies, EMBEDDED_POLICIES>,
//       Append<policy::Inherits, INHERITS>::type
//     >;
//
//   // instantiate policy
//   object = policy::apply<ACTUAL, next::Chain::item, next::EmbeddedChain::item>;
//
//   // return values
//   Chain = List<object, next::Chain>;
//   EmbeddedChain = next::EmbeddedChain::next;
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
//         EMBEDDED_POLICIES::item,
//         ACTUAL,
//         EMBEDDED_POLICIES::next,
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
     typedef typename POLICIES::item policy;
     typedef List<typename policy::Policies, EMBEDDED_POLICIES> nextEmbeddedPolicies;
     typedef typename Append<typename policy::Inherits, INHERITS>::type nextInherits;
     typedef typename TGET_3apply
       ( PolicyChainImpl<typename POLICIES::next>,
         ACTUAL,
         nextEmbeddedPolicies,
         nextInherits
       ) next;

     // instantiate policy

     typedef typename TGET_3Policy(
       policy,
       ACTUAL,
       typename TGET_item(next::Chain),
       typename TGET_item(next::EmbeddedChain)
     ) object;
     // return values
     typedef List<object, typename next::Chain> Chain;
     typedef typename TGET_next(next::EmbeddedChain) EmbeddedChain;
   };
};

template<class EMBEDDED_POLICIES>
struct PolicyChainImplEnd /* 4 */
{
  template<class ACTUAL, class INHERITS>
  struct apply
  {
    typedef typename PolicyChainImpl
      < typename EMBEDDED_POLICIES::item  /* POLICIES */
      >::template apply
      < ACTUAL,                           /* ACTUAL */
        typename EMBEDDED_POLICIES::next, /* EMBEDDED_POLICIES */
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
    typedef typename PolicyChainImplEnd
      < EMBEDDED_POLICIES        /* EMBEDDED_POLICIES */
      >::template apply
      < ACTUAL,                  /* ACTUAL */
        INHERITS                 /* INHERITS */
      > next;
    typedef typename TGET_Chain(next) Chain;
    typedef typename TGET_EmbeddedChain(next) EmbeddedChain;
  };
};

#define CALL_PolicyChain(POLICIES, ACTUAL, INHERITS) PolicyChainImpl<POLICIES>::template apply<ACTUAL, NullType, INHERITS>

template<class ACTUAL, class POLICIES, class INHERITS = NullType>
struct PolicyAssembler
  : public TGET_item(TGET_Chain(CALL_PolicyChain(POLICIES, ACTUAL, INHERITS)))
{
  typedef typename TGET_Chain(CALL_PolicyChain(POLICIES, ACTUAL, INHERITS)) Chain;
};

#endif
