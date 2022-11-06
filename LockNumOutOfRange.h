#ifndef LOCKNUMOUTOFRANGE
#define LOCKNUMOUTOFRANGE

#include <stdexcept>
#include <string>

using namespace std;

class LockNumOutOfRange : public out_of_range {
  public:
   LockNumOutOfRange(const string & message = "")
      : out_of_range(message.c_str())
    { }  // end constructor
}; // end LockNumOutOfRange

#endif


