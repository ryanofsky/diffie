// LIMITING DESIGN DESCISIONS
// 
// 1) For simplicity and consitency, calls to Swallow() will never result in ownership
// of the swallowed pointer or primitive handle. Primitive handles can still be
// owned by in an open() method or custom method ("alexandrescu enriched interface").

#ifndef simple_buffer_hpp
#define simple_buffer_hpp

template<class T, class ACTUAL = Unspecified, class BASE = Unspecified>
class BufferManager
{

public:

  typedef BufferManager<Unspecified> Identity;
  typedef T Parameterization;

protected:

  T * ptr;
  size_t capacity;
  
  void Open(size_t capacity)
  {
    ptr = new T[capacity];
  };
  
  void Close()
  {
    delete [] ptr;
    Disown();
  }
  
  void SetNull()
  {
    ptr = Null;
    Disown();
  }
  
  bool IsNull()
  {
    return !ptr;
  }
  
  bool IsOwned
  {
    return capacity > 0;
  }
  
  void Disown()
  {
    capacity = 0;
  }

  // DeepCopy stage 1
  void Reallocate(BufferManager const & bm)
  {
    if (capacity < bm.capacity)
    {
      DisOwn();
      Close();
      Open(bm.capacity);
    }
  }
  
  void DeepCopy(BufferManager const & bm)
  {
    for(int i = 0; i < bm.capacity; ++i)
    {
      ptr[i] = bm.ptr[i];
    }
  }
  
  void Swallow(T * tptr)
  {
    ptr = tptr;
  }
};

template<typename PARAMETERIZATION, class ACTUAL, class BASE>
class Specify< BufferManager<Unspecified> >
{
  typedef BufferManager<PARAMETERIZATION, ACTUAL, BASE> BufferManagerResult;
};

#endif