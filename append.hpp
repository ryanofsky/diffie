#ifndef append_hpp
#define append_hpp

#include "workaround.hpp"

// Function:     Append(LIST1, LIST2)
//
// Purpose:      Appends LIST2 to LIST1 by returning a copy of LIST1 with its
//               null terminator replaced by LIST2
//
// Requirements: LIST1 and LIST2 are typelists (although either or both can be
//               null)
//
// Pseudocode:   if (LIST1 != NullType)
//                 return List<LIST1::item, Append(LIST1::next, LIST2)
//               else
//                 return LIST2;
//
//               Run time is proportional to the size of the first list. As a
//               result, it is more efficient to append a long list to a short
//               list than a short list to a long list.

template<class LIST1>
struct AppendImpl
{
  template<class LIST2>
  struct apply
  {
    typedef typename TGET_1apply(AppendImpl<typename LIST1::next>, LIST2) next;
    typedef List
      <
        typename LIST1::item,
        typename TGET_type(next)
      > type;
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

template<class LIST1, class LIST2>
struct Append
{
  typedef typename AppendImpl<LIST1>::template apply<LIST2>::type type;
};

// Function:     EfficientAppend(LIST1, LIST2)
//
// Purpose:      Checks if LIST2 is empty, if so returns LIST1, otherwise
//               it returns Append(LIST1, LIST2)
//
// Pseudocode:   if (LIST2 == NullType)
//                 return LIST1;
//               else
//                 return Append(LIST1, LIST2);
//
//              This function is the same as Append(), but it prevents a list
//              traversal when LIST2 is null.

template<class LIST2>
struct EfficientAppendImpl
{
  template<class LIST1>
  struct apply
  {
    typedef AppendImpl<LIST1>::apply<LIST2> partial;
    typedef typename TGET_type(partial) type;
  };
};

template<>
struct EfficientAppendImpl<NullType>
{
  template<class LIST1>
  struct apply
  {
    typedef LIST1 type;
  };
};

template<class LIST1, class LIST2>
struct EfficientAppend
{
  typedef typename EfficientAppendImpl<LIST1>::apply<LIST2>::type type;
};

#endif
