#pragma once

#include <boost/python/list.hpp>

#include <array>

template< typename T, size_t N >
struct ArrayToListConverter
{

    static PyObject* convert( const std::array< T, N >& theArray )
    {
        boost::python::list result;
        for ( size_t i( 0u ); i < N; ++i )
        {
            result.append( theArray.at( i ) );
        }
        return boost::python::incref( result.ptr() );
    }
};

template< typename T, size_t Rows, size_t Columns >
struct ArrayOfArrayToListConverter
{

    static PyObject* convert( const std::array< std::array< T, Rows >,
                                                Columns >& theArray )
    {
        boost::python::list matrix;
        for ( size_t i( 0u ), j; i < Rows; ++i )
        {
            boost::python::list row;
            for ( j = 0u; j < Columns; ++j )
            {
                row.append( theArray.at( i ).at( j ) );
            }
            matrix.append( row );
        }
        return boost::python::incref( matrix.ptr() );
    }
};
