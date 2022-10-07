#include "WrappedTrackingSystem.hpp"

#include "WrappedGeometry.hpp"

#include <functional>
#include <iostream>
#include <limits>

using namespace std;

#ifdef BUILD_WITH_PRIVATE_SDK
    #define TrackingSystemBase TrackingSystemPrivate
#else
    #define TrackingSystemBase TrackingSystem
#endif

WrappedTrackingSystem::WrappedTrackingSystem( uint32 timeout,
                                              bool   allowSimulator )
    : atracsys::TrackingSystemBase( timeout, allowSimulator )
{}

atracsys::Status WrappedTrackingSystem::initialise( const string& fileName )
{
    return TrackingSystemBase::initialise( fileName );
}

void WrappedTrackingSystem::streamLastError()
{
    atracsys::Status retCode( atracsys::TrackingSystemBase::streamLastError() );


    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             <<
            " atracsys::TrackingSystemBase::streamLastError returned status "
             << uint32( retCode ) << endl;
    }
}

tuple< atracsys::Status, map< string, string > >
WrappedTrackingSystem::getLastError()
{
    map< string, string > messages;
    atracsys::Status retCode( atracsys::TrackingSystemBase::getLastError(
                                  messages ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getLastError returned status "
             << uint32( retCode ) << endl;
    }

    return make_tuple( retCode, messages );
}

tuple< atracsys::Status,
       vector< atracsys::DeviceInfo > > WrappedTrackingSystem::
getEnumeratedDevices()
const
{
    vector< atracsys::DeviceInfo > result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getEnumeratedDevices(
                                  result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             <<
            " atracsys::TrackingSystemBase::getEnumeratedDevices returned status "
             << uint32( retCode ) << endl;
        result.clear();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status,
       vector< atracsys::DeviceOption > > WrappedTrackingSystem::
getEnumeratedOptions(
    uint64 sn ) const
{
    vector< atracsys::DeviceOption > result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getEnumeratedOptions(
                                  sn,
                                  result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             <<
            "atracsys::TrackingSystemBase::getEnumeratedOptions returned status "
             << uint32( retCode ) << endl;
        result.clear();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status,
       vector< atracsys::DeviceOption > > WrappedTrackingSystem::
getEnumeratedOptions()
const
{
    vector< atracsys::DeviceOption > result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getEnumeratedOptions(
                                  result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             <<
            "atracsys::TrackingSystemBase::getEnumeratedOptions returned status "
             << uint32( retCode ) << endl;
        result.clear();
    }

    return make_tuple( retCode, result );
}

// -----------------------------------------------------------------------------
//
//                             Integer option getters
//
// -----------------------------------------------------------------------------

tuple< atracsys::Status, int32 > WrappedTrackingSystem::getIntOption(
    uint64          serialNbr,
    uint32          optId,
    ftkOptionGetter what )
const
{
    int32 result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getIntOption(
                                  serialNbr, optId, what, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getIntOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< int32 >::min();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, int32 > WrappedTrackingSystem::getIntOption(
    uint64 serialNbr,
    uint32 optId ) const
{
    int32 result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getIntOption(
                                  serialNbr, optId, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getIntOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< int32 >::min();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, int32 > WrappedTrackingSystem::getIntOption(
    uint32          optId,
    ftkOptionGetter what ) const
{
    int32 result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getIntOption(
                                  optId, what, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getIntOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< int32 >::min();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, int32 > WrappedTrackingSystem::getIntOption(
    uint32 optId ) const
{
    int32 result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getIntOption(
                                  optId, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getIntOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< int32 >::min();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, int32 > WrappedTrackingSystem::getIntOption(
    uint64          serialNbr,
    const string&   optId,
    ftkOptionGetter what )
const
{
    int32 result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getIntOption(
                                  serialNbr, optId, what, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getIntOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< int32 >::min();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, int32 > WrappedTrackingSystem::getIntOption(
    uint64        serialNbr,
    const string& optId )
const
{
    int32 result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getIntOption(
                                  serialNbr, optId, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getIntOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< int32 >::min();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, int32 > WrappedTrackingSystem::getIntOption(
    const string&   optId,
    ftkOptionGetter what )
const
{
    int32 result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getIntOption(
                                  optId, what, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getIntOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< int32 >::min();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, int32 > WrappedTrackingSystem::getIntOption(
    const string& optId ) const
{
    int32 result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getIntOption(
                                  optId, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getIntOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< int32 >::min();
    }

    return make_tuple( retCode, result );
}

// -----------------------------------------------------------------------------
//
//                             Float option getters
//
// -----------------------------------------------------------------------------

tuple< atracsys::Status, float > WrappedTrackingSystem::getFloatOption(
    uint64          serialNbr,
    uint32          optId,
    ftkOptionGetter what )
const
{
    float result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getFloatOption(
                                  serialNbr, optId, what, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getFloatOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< float >::min();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, float > WrappedTrackingSystem::getFloatOption(
    uint64 serialNbr,
    uint32 optId ) const
{
    float result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getFloatOption(
                                  serialNbr, optId, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getFloatOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< float >::min();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, float > WrappedTrackingSystem::getFloatOption(
    uint32          optId,
    ftkOptionGetter what )
const
{
    float result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getFloatOption(
                                  optId, what, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getFloatOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< float >::min();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, float > WrappedTrackingSystem::getFloatOption(
    uint32 optId ) const
{
    float result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getFloatOption(
                                  optId, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getFloatOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< float >::min();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, float > WrappedTrackingSystem::getFloatOption(
    uint64          serialNbr,
    const string&   optId,
    ftkOptionGetter what )
const
{
    float result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getFloatOption(
                                  serialNbr, optId, what, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getFloatOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< float >::min();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, float > WrappedTrackingSystem::getFloatOption(
    uint64        serialNbr,
    const string& optId )
const
{
    float result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getFloatOption(
                                  serialNbr, optId, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getFloatOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< float >::min();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, float > WrappedTrackingSystem::getFloatOption(
    const string&   optId,
    ftkOptionGetter what )
const
{
    float result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getFloatOption(
                                  optId, what, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getFloatOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< float >::min();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, float > WrappedTrackingSystem::getFloatOption(
    const string& optId ) const
{
    float result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getFloatOption(
                                  optId, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getFloatOption returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< float >::min();
    }

    return make_tuple( retCode, result );
}

// -----------------------------------------------------------------------------
//
//                             Binary option getters
//
// -----------------------------------------------------------------------------

tuple< atracsys::Status, string > WrappedTrackingSystem::getDataOption(
    uint64 serialNbr,
    uint32 optId ) const
{
    string result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getDataOption(
                                  serialNbr, optId, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getDataOption returned status "
             << uint32( retCode ) << endl;
        result = "ERROR";
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, string > WrappedTrackingSystem::getDataOption(
    uint32 optId ) const
{
    string result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getDataOption(
                                  optId, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getDataOption returned status "
             << uint32( retCode ) << endl;
        result = "ERROR";
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, string > WrappedTrackingSystem::getDataOption(
    uint64        serialNbr,
    const string& optId )
const
{
    string result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getDataOption(
                                  serialNbr, optId, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getDataOption returned status "
             << uint32( retCode ) << endl;
        result = "ERROR";
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, string > WrappedTrackingSystem::getDataOption(
    const string& optId ) const
{
    string result;

    atracsys::Status retCode( atracsys::TrackingSystemBase::getDataOption(
                                  optId, result ) );

    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             << " atracsys::TrackingSystemBase::getDataOption returned status "
             << uint32( retCode ) << endl;
        result = "ERROR";
    }

    return make_tuple( retCode, result );
}

#ifdef BUILD_WITH_PRIVATE_SDK
tuple< atracsys::Status, vector< atracsys::DeviceRegister > > WrappedTrackingSystem::getEnumeratedRegisters(
  uint64 serialNbr ) const
{
    vector< atracsys::DeviceRegister > regs{};
    atracsys::Status retCode( atracsys::TrackingSystemBase::getEnumeratedRegisters( serialNbr, regs ) );

    return make_tuple( retCode, regs );
}

tuple< atracsys::Status, vector< atracsys::DeviceRegister > > WrappedTrackingSystem::getEnumeratedRegisters()
  const
{
    vector< atracsys::DeviceRegister > regs{};
    atracsys::Status retCode( atracsys::TrackingSystemBase::getEnumeratedRegisters( regs ) );

    return make_tuple( retCode, regs );
}

tuple< atracsys::Status, uint32 > WrappedTrackingSystem::getRegisterValue(
    uint64 serialNbr,
    uint32 regId )
const
{
    uint32 result( 0u );

    atracsys::Status retCode(
        atracsys::TrackingSystemBase::getRegisterValue( serialNbr, regId,
                                                        result ) );
    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             <<
            " atracsys::TrackingSystemPrivate::getRegisterValue returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< uint32 >::max();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, uint32 > WrappedTrackingSystem::getRegisterValue(
    uint32 regId ) const
{
    uint32 result( 0u );

    atracsys::Status retCode(
        atracsys::TrackingSystemBase::getRegisterValue( regId, result ) );
    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             <<
            " atracsys::TrackingSystemPrivate::getRegisterValue returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< uint32 >::max();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, uint32 > WrappedTrackingSystem::getRegisterValue(
    uint64             serialNbr,
    const string& regId )
const
{
    uint32 result( 0u );

    atracsys::Status retCode(
        atracsys::TrackingSystemBase::getRegisterValue( serialNbr, regId,
                                                        result ) );
    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             <<
            " atracsys::TrackingSystemPrivate::getRegisterValue returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< uint32 >::max();
    }

    return make_tuple( retCode, result );
}

tuple< atracsys::Status, uint32 > WrappedTrackingSystem::getRegisterValue(
    const string& regId ) const
{
    uint32 result( 0u );

    atracsys::Status retCode(
        atracsys::TrackingSystemBase::getRegisterValue( regId, result ) );
    if ( retCode != atracsys::Status::Ok )
    {
        cerr << __FILE__ << ": " << __LINE__
             <<
            " atracsys::TrackingSystemPrivate::getRegisterValue returned status "
             << uint32( retCode ) << endl;
        result = numeric_limits< uint32 >::max();
    }

    return make_tuple( retCode, result );
}

#endif

atracsys::Status WrappedTrackingSystem::setGeometry(
    uint64                 serialNbr,
    const WrappedGeometry& geom )
{
    ftkRigidBody theGeom{};
    theGeom.geometryId = geom.getGeometryId();
    theGeom.pointsCount = geom.getPointsCount();
    theGeom.divotsCount = geom.getDivotsCount();
    for ( uint32_t iPt( 0u ); iPt < theGeom.pointsCount; ++iPt )
    {
        theGeom.fiducials[ iPt ] = geom.getPositions().at( iPt );
    }
    for ( uint32_t iPt( 0u ); iPt < theGeom.divotsCount; ++iPt )
    {
        theGeom.divotPositions[ iPt ] = geom.getDivots().at( iPt );
    }
    return atracsys::TrackingSystemBase::setGeometry( serialNbr, theGeom );
}

atracsys::Status WrappedTrackingSystem::setGeometry(
    const WrappedGeometry& geom )
{
    ftkRigidBody theGeom{};
    theGeom.geometryId = geom.getGeometryId();
    theGeom.pointsCount = geom.getPointsCount();
    theGeom.divotsCount = geom.getDivotsCount();
    for ( uint32_t iPt( 0u ); iPt < theGeom.pointsCount; ++iPt )
    {
        theGeom.fiducials[ iPt ] = geom.getPositions().at( iPt );
    }
    for ( uint32_t iPt( 0u ); iPt < theGeom.divotsCount; ++iPt )
    {
        theGeom.divotPositions[ iPt ] = geom.getDivots().at( iPt );
    }
    return atracsys::TrackingSystemBase::setGeometry( theGeom );
}

atracsys::Status WrappedTrackingSystem::setGeometry(
    uint64             serialNbr,
    const string& geomFile )
{
    return atracsys::TrackingSystemBase::setGeometry( serialNbr, geomFile );
}

atracsys::Status WrappedTrackingSystem::setGeometry(
    const string& geomFile )
{
    return atracsys::TrackingSystemBase::setGeometry( geomFile );
}

tuple< atracsys::Status, WrappedGeometry > WrappedTrackingSystem::loadRigidBody( const string& fileName )
{
    tuple< atracsys::Status, WrappedGeometry > result{};
    get< 0u >( result ) = atracsys::TrackingSystemBase::loadGeometry( fileName, get< 1u >( result ) );
    return result;
}

atracsys::Status WrappedTrackingSystem::saveRigidBody( const string& fileName, const WrappedGeometry& geom )
{
     return atracsys::TrackingSystemBase::saveGeometry( fileName, geom );
}

tuple< atracsys::Status, vector< WrappedPoint > > WrappedTrackingSystem::getAccelerometerDataForDevice(
  uint64_t serialNbr )
{
    vector< array< float, 3u > > data{};

    tuple< atracsys::Status, vector< WrappedPoint > > result{};

    get< 0u >( result ) = atracsys::TrackingSystemBase::getAccelerometerData( serialNbr, data );

    get< 1u >( result ).reserve( data.size() );

    for ( const auto& item : data )
    {
        get< 1u >( result ).emplace_back( WrappedPoint( item.at( 0u ), item.at( 1u ), item.at( 2u ) ) );
    }

    return result;
}

tuple< atracsys::Status, vector< WrappedPoint > > WrappedTrackingSystem::getAccelerometerDataForDevice()
{
    vector< array< float, 3u > > data{};

    tuple< atracsys::Status, vector< WrappedPoint > > result{};

    get< 0u >( result ) = atracsys::TrackingSystemBase::getAccelerometerData( data );

    get< 1u >( result ).reserve( data.size() );

    for ( const auto& item : data )
    {
        get< 1u >( result ).emplace_back( WrappedPoint( item.at( 0u ), item.at( 1u ), item.at( 2u ) ) );
    }

    return result;
}

tuple< atracsys::Status, vector< vector< WrappedPoint > > > WrappedTrackingSystem::getAccelerometerData()
{
    vector< vector< array< float, 3u > > > dataForEachDevice{};

    tuple< atracsys::Status, vector< vector< WrappedPoint > > > result{};

    get< 0u >( result ) = atracsys::TrackingSystemBase::getAccelerometerData( dataForEachDevice );

    get< 1u >( result ).reserve( dataForEachDevice.size() );

    for ( const auto& points : dataForEachDevice )
    {
        get< 1u >( result ).emplace_back( vector< WrappedPoint >() );
        get< 1u >( result ).back().reserve( points.size() );
        for ( const auto& item : points )
        {
            get< 1u >( result ).back().emplace_back( item.at( 0u ), item.at( 1u ), item.at( 2u ) );
        }
    }

    return result;
}

tuple< atracsys::Status, atracsys::timestamp_t > WrappedTrackingSystem::getRealTimeClockForDevice(
  uint64_t serialNbr )
{
    tuple< atracsys::Status, atracsys::timestamp_t > result{};

    get< 0u >( result ) = atracsys::TrackingSystemBase::getRealTimeClock( serialNbr, get< 1u >( result ) );

    return result;
}

tuple< atracsys::Status, atracsys::timestamp_t >
  WrappedTrackingSystem::getRealTimeClockForDevice()
{
    tuple< atracsys::Status, atracsys::timestamp_t > result{};

    get< 0u >( result ) = atracsys::TrackingSystemBase::getRealTimeClock( get< 1u >( result ) );

    return result;
}

tuple< atracsys::Status, vector< atracsys::timestamp_t > >
  WrappedTrackingSystem::getRealTimeClock()
{
    tuple< atracsys::Status, vector< atracsys::timestamp_t > > result{};

    get< 0u >( result ) = atracsys::TrackingSystemBase::getRealTimeClock( get< 1u >( result ) );

    return result;
}
