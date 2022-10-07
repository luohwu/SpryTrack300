#include "WrappedGeometry.hpp"

#include <algorithm>
#include <functional>

using namespace std;

WrappedGeometry::WrappedGeometry( const ftkRigidBody& geom )
    : ftkRigidBody( geom )
{}

unsigned int WrappedGeometry::getVersion() const
{
    return version;
}

void WrappedGeometry::setVersion( unsigned int value )
{
    version = value;
}


unsigned int WrappedGeometry::getGeometryId() const
{
    return geometryId;
}

void WrappedGeometry::setGeometryId( unsigned int value )
{
    geometryId = value;
}

unsigned int WrappedGeometry::getPointsCount() const
{
    return pointsCount;
}

void WrappedGeometry::setPointsCount( unsigned int value )
{
    pointsCount = value;
}

FiducialsInGeom WrappedGeometry::getPositions() const
{
    FiducialsInGeom result;
    result.reserve( pointsCount );
    for ( uint32_t i( 0u ); i < pointsCount; ++i )
    {
        result.emplace_back( fiducials[ i ] );
    }
    return result;
}

void WrappedGeometry::setPositions( const FiducialsInGeom& value )
{
    for ( size_t i( 0u );
          i < std::min( static_cast<size_t>( pointsCount ), value.size() );
          ++i )
    {
        fiducials[ i ] = value.at( i );
    }
}

unsigned int WrappedGeometry::getDivotsCount() const
{
    return divotsCount;
}

void WrappedGeometry::setDivotsCount( unsigned int value )
{
    divotsCount = value;
}

PointsInGeom WrappedGeometry::getDivots() const
{
    PointsInGeom result;
    result.reserve( divotsCount );
    for ( uint32_t i( 0u ); i < divotsCount; ++i )
    {
        result.emplace_back( divotPositions[ i ].x,  divotPositions[ i ].y, divotPositions[ i ].z );
    }
    return result;
}

void WrappedGeometry::setDivots( const PointsInGeom& value )
{
    for ( size_t i( 0u );
          i < std::min( static_cast< size_t >( pointsCount ), value.size() );
          ++i )
    {
        divotPositions[ i ] = value.at( i );
    }
}
