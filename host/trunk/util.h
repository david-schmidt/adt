#ifndef ADT_STR_STREAM_H
#define ADT_STR_STREAM_H

#include <string>
#include <sstream>
#include <stdexcept>

namespace adt
{

class str_stream
{
  public:
    str_stream() : mStreamOut() { }

    // This copy constructor is necessary because stringstream does
    // not provide a copy constructor.  gcc 3.4 transparently copies
    // str_streams, and hence triggers this situation.
    str_stream(const str_stream & s) : mStreamOut(s.mStreamOut.str()) { }

    std::stringstream & underlying_stream() const
    { return mStreamOut; }

    operator std::string() const
    {
        return mStreamOut.str();
    }

  private:
    mutable std::stringstream mStreamOut;
};

template <class type>
    const str_stream & operator<< (const str_stream & out, const type & value)
{
    out.underlying_stream() << value;
    return out;
}

void inline cxx_assert(bool condition, std::string message)
{
    if (!condition)
    {
        std::string what = "Assertion failed: " + message;
        throw std::runtime_error(what);
    }
}

#define CXX_ASSERT(condition) \
    cxx_assert(condition, #condition)
    

}

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
