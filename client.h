#ifndef client_h
#define client_h

#include <sstream>

class BufferedSocketOut : public ostringstream
{
public:
  ACE_SOCK_Stream & stream_;

  BufferedSocketOut(ACE_SOCK_Stream & stream) 
  : stream_(stream);

  ostream & flush()
  {
    ostringstream::flush();
    string s = this->str();
    if (stream.send_n(s.c_str(), s.length()) == -1)
    {
      ACE_ERROR((LM_ERROR,"%p\n","open"),-1);
    }
    this->str(s = "");
    return *this;
  }
};

#endif