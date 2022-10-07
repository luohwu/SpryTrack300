#include "WrappedFiducial.hpp"

#include <cmath>

WrappedFiducial::WrappedFiducial( float xVal, float yVal, float zVal, float normX, float normY, float normZ,
                                  ftkFiducialType fidType, float angleOfView )
    : ftkFiducial()
{
    position.x = xVal;
    position.y = yVal;
    position.z = zVal;

    normalVector.x = normX;
    normalVector.y = normY;
    normalVector.z = normZ;

    fiducialInfo.type = fidType;
    fiducialInfo.angleOfView = angleOfView;
}

WrappedFiducial::WrappedFiducial( const ftkFiducial& other )
    : ftkFiducial( other )
{
}

bool WrappedFiducial::operator==( const WrappedFiducial& other ) const
{
    if ( std::abs( position.x - other.position.x ) > 1.e-7 ||
         std::abs( position.y - other.position.y ) > 1.e-7 ||
         std::abs( position.z - other.position.z ) > 1.e-7 )
    {
        return false;
    }
    if ( std::abs( normalVector.x - other.normalVector.x ) > 1.e-7 ||
         std::abs( normalVector.y - other.normalVector.y ) > 1.e-7 ||
         std::abs( normalVector.z - other.normalVector.z ) > 1.e-7 )
    {
        return false;
    }
    if ( fiducialInfo.type != other.fiducialInfo.type ||
         std::abs( fiducialInfo.angleOfView - other.fiducialInfo.angleOfView ) > 1.e-7f )
    {
        return false;
    }

    return true;
}

WrappedPoint WrappedFiducial::getPosition() const
{
    return WrappedPoint( position );
}

void WrappedFiducial::setPosition( const WrappedPoint& pos )
{
    position = pos;
}

WrappedPoint WrappedFiducial::getNormalVector() const
{
    return WrappedPoint( normalVector );
}

void WrappedFiducial::setNormalVector( const WrappedPoint& norm )
{
    normalVector = norm;
}
