#ifndef Pocket_hpp
#define Pocket_hpp

#include <new>

// if sizeof(T) <= sizeof(U) return T; else return EmptyBase
template<class T, class U>
struct FitInside
{
  typedef ::boost::mpl::if_<sizeof(T) <= sizeof(U), T, EmptyBase>::type t1;
};

template<class T>
union Aligned
{
  EmptyClass member0;
  FitInside<char, T>::type member1;
  FitInside<int, T>::type member2;
  FitInside<float, T>::type member3;
  FitInside<double, T>::type member4;
};

template<class T>
class Pocket
{
public:
  
  void Construct()
  {
    void * place = data_;
    T * t = new (place) bob;
  }

  template<typename A1>
  void Construct(A1 & a1)
  {
    void * place = data_;
    T * t = new (place) T(a1);
  }

  template<typename A1, typename A2>
  void Construct(A1 & a1, A2 & a2)
  {
    void * place = data_;
    T * t = new (place) T(a1, a2);
  }

  void Destruct()
  {
    T & t = *((T *)data_);
    t.~T();
  }

  T & operator*()
  {
    return *((T *)data_);
  }

  T * operator->()
  {
    return (T *)data_;
  }

protected:
  typedef Aligned<T> ArrayType;
  ArrayType data_[1 + (sizeof(T) - 1) / sizeof(ArrayType)];

  Pocket(Pocket const &);
  Pocket & operator=(Pocket const &);
};

#endif
