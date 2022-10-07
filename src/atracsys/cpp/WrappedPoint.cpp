#include "WrappedPoint.hpp"

#include <cmath>

WrappedPoint::WrappedPoint( float xVal, float yVal, float zVal )
    : ftk3DPoint()
{
    x = xVal;
    y = yVal;
    z = zVal;
}

WrappedPoint::WrappedPoint( const ftk3DPoint& pt )
    : ftk3DPoint( pt )
{
}

bool WrappedPoint::operator==( const WrappedPoint& other ) const
{
    return std::abs( x - other.x ) < 1.e-7 && std::abs( y - other.y ) < 1.e-7 &&
           std::abs( z - other.z ) < 1.e-7;
}
