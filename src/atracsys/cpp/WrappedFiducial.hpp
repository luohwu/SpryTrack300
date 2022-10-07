#pragma once

#include "WrappedPoint.hpp"

#include <ftkInterface.h>

#include <vector>

class WrappedFiducial : public ftkFiducial
{
public:
    WrappedFiducial( float xVal, float yVal, float zVal, float normX, float normY, float normZ,
                     ftkFiducialType fidType = ftkFiducialType::Unspecified, float angleOfView = -1.f );

    WrappedFiducial( const ftkFiducial& other );

    WrappedFiducial() = default;

    WrappedFiducial( const WrappedFiducial& other ) = default;

    WrappedFiducial( WrappedFiducial&& other ) = default;

    ~WrappedFiducial() = default;

    WrappedFiducial& operator=( const WrappedFiducial& other ) = default;
    WrappedFiducial& operator=( WrappedFiducial&& other ) = default;

    bool operator==( const WrappedFiducial& other ) const;

    WrappedPoint getPosition() const;
    void setPosition( const WrappedPoint& pos );

    WrappedPoint getNormalVector() const;
    void setNormalVector( const WrappedPoint& norm );
};

typedef std::vector< WrappedFiducial > FiducialsInGeom;
