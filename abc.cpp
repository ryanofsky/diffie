#include <new>
#include <iostream>

class EmptyClass {};

template<bool flag, typename T, typename U>
class Select
{
  template<bool flag>
  class If
  {
    typedef T result;
  };

  template<>
  class If<false>
  {
    typedef U result;
  };

public:
  typedef If<flag>::result result;
};

// if sizeof(T) <= sizeof(U) return T else return EmptyClass
template<class T, class U>
struct FitInside
{
  typedef Select<sizeof(T) <= sizeof(U), T, EmptyClass> Result;
};

template<class T>
union Aligned
{
  EmptyClass member0;
  FitInside<char, T> member1;
  FitInside<int, T> member2;
  FitInside<float, T> member3;
  FitInside<double, T> member4;
};

template<class T>
class Chalice
{
public:  
  typedef T ARG_T;
  void Construct()
  {
    void * place = data_;
    T * t = new (place) bob;
  }

  void Destruct()
  {
    T & t = *((T *)data_);
    t.~T();
  }

protected:    
  typedef Aligned<T> ArrayType;
  ArrayType data_[1 + (sizeof(T) - 1) / sizeof(ArrayType)];
};

struct bob
{
  char b;
  bob()
  {
    std::cout << "Constructing bob\n";
  }

  ~bob()
  {
    std::cout << "destructing bob\n";
  }

};

void main()
{
  Chalice<bob> b;
  b.Construct();
  b.Destruct();

  typedef Chalice<bob>::ARG_T mary;
  mary m;
}