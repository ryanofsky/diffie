#ifndef SimpleString_hpp
#define SimpleString_hpp

struct SimpleString
{
  typedef ::boost::mpl::null_node Inherits;
  typedef ::boost::mpl::null_node Policies;
  
  template<class ACTUAL, class BASE, class CHAIN>
  struct Policy : WORKAROUND_InheritFrom(BASE)
  {
    char * buffer;
    size_t capacity;
    
    SimpleString() : buffer(0), capacity(0) {}
    
    bool IsNull()
    {
      return capacity == 0;
    }
    
    void SetNull()
    {
      capacity = 0;
    }
    
    void Swallow(char const * cstring)
    {
      ACTUAL & self = *(ACTUAL*)this;
      DiscardResource(self);
      self.buffer = const_cast<char *> cstring;
      self.capacity = 0;
    }
    
    void Swallow(char * cstring, int capacity)
    {
      ACTUAL & self = *(ACTUAL*)this;
      DiscardResource(self);
      self.buffer = cstring;
      self.capacity = capacity;
      self.Own();
    }
    
    void Resize(int capacity)
    {
      ACTUAL & self = *(ACTUAL*)this;
      
      if (self.capacity <= capacity)
      {
        TakeOwnership(capacity);
      }
    }
    
    void TakeOwnership(int capacity = 0)
    {
      ACTUAL & self = *(ACTUAL*)this;
      if (!capacity)
      {
        capacity = self.capacity;
        if (!capacity)
        {
          if (self.buffer)
          {
            capacity = strlen(self.buffer);
            if (!capacity) return;
          }
          else
            return;
        }
      }          
      
      // xxx: this needs to be protected from exceptions
      char * buffer = new char[capacity];  
      memcpy(self.buffer, buffer, self.capacity);
      DiscardResource(self);
      self.capacity = capacity;
      self.buffer = buffer;
    };
   
   

    
  };
  
};

#endif