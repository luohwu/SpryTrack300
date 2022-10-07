#pragma once

#include <boost/iostreams/concepts.hpp>  // boost::iostreams::source
#include <boost/iostreams/stream.hpp>

#include <boost/python.hpp>

/// @brief Type that implements the Boost.IOStream's Sink and Flushable
/// concept for writing data to Python object that support:
/// n = object.write(str) # n = None or bytes written
/// object.flush()        # if flush exists, then it is callable
///
/// \see https://stackoverflow.com/questions/26033781/converting-python-io-object-to-stdostream-when-using-boostpython
class PythonOutputDevice
{
public:

    // This class models both the Sink and Flushable concepts.
    struct category
        : boost::iostreams::sink_tag, boost::iostreams::flushable_tag
    {};

    explicit
    PythonOutputDevice( boost::python::object object );

// Sink concept.

    typedef char char_type;

    std::streamsize write( const char* buffer, std::streamsize buffer_size );

// Flushable concept.

    bool flush();

private:
    boost::python::object object_;
};
