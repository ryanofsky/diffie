#ifndef general_hpp
#define general_hpp

struct NullType;
struct EmptyType{};

template<class HEAD, class TAIL>
struct List
{
  typedef HEAD Head;
  typedef TAIL Tail;
};

#endif 