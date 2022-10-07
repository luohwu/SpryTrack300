#pragma once

#include "WrappedFiducial.hpp"
#include "WrappedPoint.hpp"

#include <ftkInterface.h>


class WrappedGeometry : public ftkRigidBody
{
public:
    WrappedGeometry() = default;

    WrappedGeometry( const WrappedGeometry& other ) = default;

    WrappedGeometry( WrappedGeometry&& other ) = default;

    WrappedGeometry( const ftkRigidBody& geom );

    ~WrappedGeometry() = default;

    WrappedGeometry& operator=( const WrappedGeometry& other ) = default;
    WrappedGeometry& operator=( WrappedGeometry&& other ) = default;

    unsigned int getVersion() const;
    void setVersion( unsigned int value );
    unsigned int getGeometryId() const;
    void setGeometryId( unsigned int value );
    unsigned int getPointsCount() const;
    void setPointsCount( unsigned int value );
    FiducialsInGeom getPositions() const;
    void setPositions( const FiducialsInGeom& value );
    unsigned int getDivotsCount() const;
    void setDivotsCount( unsigned int value );
    PointsInGeom getDivots() const;
    void setDivots( const PointsInGeom& value );
};
