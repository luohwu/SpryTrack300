#pragma once

#include <ftkInterface.h>

#include <vector>

class WrappedPoint : public ftk3DPoint
{
public:
    WrappedPoint( float xVal, float yVal, float zVal );

    explicit WrappedPoint( const ftk3DPoint& pt );

    WrappedPoint() = default;

    WrappedPoint( const WrappedPoint& other ) = default;

    WrappedPoint( WrappedPoint&& other ) = default;

    ~WrappedPoint() = default;

    WrappedPoint& operator=( const WrappedPoint& other ) = default;
    WrappedPoint& operator=( WrappedPoint&& other ) = default;

    bool operator==( const WrappedPoint& other ) const;
};

typedef std::vector< WrappedPoint > PointsInGeom;
