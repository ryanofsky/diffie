#ifndef workaround_hpp
#define workaround_hpp

#if (_MSC_VER == 1200)
#define MSVC6

template<class T>
struct TGET_never_true
{
  enum {value = false };
};

#elif (_MSC_VER == 1300)
#define MSVC7
#endif

#ifndef MSVC6

#define TGET_type(x) x::type
#define TGET_item(x) x::item
#define TGET_next(x) x::next
#define TGET_Chain(x) x::Chain
#define TGET_EmbeddedChain(x) x::EmbeddedChain
#define TGET_3Policy(x,A,B,C) x::template Policy< A,B,C >

#else

#define TGET_type(x) TGET_class_type< x >::type
#define TGET_item(x) TGET_class_item< x >::type
#define TGET_next(x) TGET_class_next< x >::type
#define TGET_Chain(x) TGET_class_Chain< x >::type
#define TGET_EmbeddedChain(x) TGET_class_EmbeddedChain< x >::type

template<class T>
struct TGET_class_type
{
  typedef T::type type;
};

template<>
struct TGET_class_type<int>
{
  typedef EmptyType type;
};

template<>
struct TGET_class_type<EmptyType>
{
  typedef EmptyType type;
};

template<>
struct TGET_class_type<NullType>
{
  typedef EmptyType type;
};

template<class T>
struct TGET_class_item
{
  typedef T::item type;
};

template<>
struct TGET_class_item<int>
{
  typedef EmptyType type;
};

template<>
struct TGET_class_item<EmptyType>
{
  typedef EmptyType type;
};

template<>
struct TGET_class_item<NullType>
{
  typedef EmptyType type;
};

template<class T>
struct TGET_class_next
{
  typedef T::next type;
};

template<>
struct TGET_class_next<int>
{
  typedef EmptyType type;
};

template<>
struct TGET_class_next<EmptyType>
{
  typedef EmptyType type;
};

template<>
struct TGET_class_next<NullType>
{
  typedef EmptyType type;
};

template<class T>
struct TGET_class_Chain
{
  typedef T::Chain type;
};

template<>
struct TGET_class_Chain<int>
{
  typedef EmptyType type;
};

template<>
struct TGET_class_Chain<EmptyType>
{
  typedef EmptyType type;
};

template<>
struct TGET_class_Chain<NullType>
{
  typedef EmptyType type;
};

template<class T>
struct TGET_class_EmbeddedChain
{
  typedef T::EmbeddedChain type;
};

template<>
struct TGET_class_EmbeddedChain<int>
{
  typedef EmptyType type;
};

template<>
struct TGET_class_EmbeddedChain<EmptyType>
{
  typedef EmptyType type;
};

template<>
struct TGET_class_EmbeddedChain<NullType>
{
  typedef EmptyType type;
};

#define TGET_3Policy(x,A,B,C) TGET_class_3Policy< x,A,B,C >::type

template<class T, class ARG1, class ARG2, class ARG3>
struct TGET_class_3Policy
{
  template<bool>
  struct Inner : T
  {};
  
  template<>
  struct Inner<true> : T
  {
    template<class A, class B, class C> struct Policy;
  };

  typedef typename Inner< TGET_never_true<T>::value >::template Policy<ARG1, ARG2, ARG3> type;
};

#endif

#ifdef MSVC7

template<typename T, typename A>
struct TGET_class_1apply
{
  typedef T::template apply<A> type;
};

#define TGET_1apply(T, A) TGET_class_1apply< T,A >::type

template<typename T, typename A, typename B, typename C>
struct TGET_class_3apply
{
  typedef T::template apply<A,B,C> type;
};

#define TGET_3apply(T, A, B, C) TGET_class_3apply< T,A,B,C >::type

#else 
#define TGET_1apply(T, A) T::template apply< A >
#define TGET_3apply(T, A, B, C) T::template apply< A,B,C >
#endif

#ifdef MSVC6

template<typename T>
struct WORKAROUND_class_InheritFrom : public T
{};

#define WORKAROUND_InheritFrom(x) WORKAROUND_class_InheritFrom<x>

#else 

#define WORKAROUND_InheritFrom(x) x

#endif

#endif