#include "PythonOutputDevice.hpp"

PythonOutputDevice::PythonOutputDevice( boost::python::object object )
    : object_( object )
{}

std::streamsize PythonOutputDevice::write( const char*     buffer,
                                           std::streamsize buffer_size )
{
    namespace python = boost::python;
    // Copy the buffer to a python string.
    python::str data( buffer, buffer_size );

    // Invoke write on the python object, passing in the data.  The following
    // is equivalent to:
    // n = object_.write(data)
    python::extract< std::streamsize > bytes_written(
        object_.attr( "write" )( data ) );

    // Per the Sink concept, return the number of bytes written.  If the
    // Python return value provides a numeric result, then use it.  Otherwise,
    // such as the case of a File object, use the buffer_size.
    return bytes_written.check()
           ? bytes_written
           : buffer_size;
}

bool PythonOutputDevice::flush()
{
    // If flush exists, then call it.
    boost::python::object flush = object_.attr( "flush" );
    if ( ! flush.is_none() )
    {
        flush();
    }

    // Always return true.  If an error occurs, an exception should be thrown.
    return true;
}
