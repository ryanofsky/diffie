#ifndef SecBufferManager_hpp
#define SecBufferManager_hpp

template<typename T>
struct SecBufferManager 
{
  typedef boost::mpl::null_type Policies;
  typedef boost::mpl::null_type Inherits;
  
  struct Policy
  {
    typedef SecBuffer<T> Buffer;

  private:    
    Pocket<Buffer> pocket;

    SecBufferManager()
    {
      SetNull();
    }
  
    void SetNull()
    {
      pocket->ptr = null;
      pocket->size = 0;
    }
  
    bool IsNull()
    {
      return !ptr;
    }
  
    void ShallowCopy(Parent const & t)
    {
      ptr = t.ptr;
      size = t.size;
    }
  
    void DeepCopy(Parent const & t)
    {
      Parent::operator=(t);
    }
  
  void Close();
  {
    Parent::Dispose();
    Parent::~Parent();
  };
  
  void Open(int size)
  {
    new (*this) Parent(size);
  }
  
  void Close()
  {
    Parent::~Parent();  
    ptr = 0; size = 0;
  }
private:
  Pocket<SecBuffer> manager;
};


#endif