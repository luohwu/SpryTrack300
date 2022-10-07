#pragma once

#include "WrappedPoint.hpp"

#ifdef BUILD_WITH_PRIVATE_SDK
    #include <TrackingSystemPrivate.hpp>
#else
    #include <TrackingSystem.hpp>
#endif

#include <chrono>
#include <deque>
#include <string>
#include <tuple>
#include <vector>

class WrappedGeometry;

#ifdef BUILD_WITH_PRIVATE_SDK
class WrappedTrackingSystem : public atracsys::TrackingSystemPrivate
#else
class WrappedTrackingSystem : public atracsys::TrackingSystem
#endif
{
public:
    WrappedTrackingSystem( uint32 timeout = 100u, bool allowSimulator = true );

    virtual ~WrappedTrackingSystem() = default;

    atracsys::Status initialise( const std::string& fileName = "" );

    void streamLastError();

    std::tuple< atracsys::Status, std::map< std::string, std::string > >
    getLastError();

    std::tuple< atracsys::Status,
                std::vector< atracsys::DeviceInfo > > getEnumeratedDevices()
    const;

    std::tuple< atracsys::Status,
                std::vector< atracsys::DeviceOption > > getEnumeratedOptions(
        uint64 sn )
    const;

    std::tuple< atracsys::Status,
                std::vector< atracsys::DeviceOption > > getEnumeratedOptions()
    const;

    std::tuple< atracsys::Status, int32 > getIntOption(
        uint64          serialNbr,
        uint32          optId,
        ftkOptionGetter what )
    const;
    std::tuple< atracsys::Status, int32 > getIntOption( uint64 serialNbr,
                                                        uint32 optId ) const;
    std::tuple< atracsys::Status, int32 > getIntOption( uint32          optId,
                                                        ftkOptionGetter what )
    const;
    std::tuple< atracsys::Status, int32 > getIntOption( uint32 optId ) const;
    std::tuple< atracsys::Status, int32 > getIntOption(
        uint64             serialNbr,
        const std::string& optId,
        ftkOptionGetter    what )
    const;
    std::tuple< atracsys::Status, int32 > getIntOption(
        uint64             serialNbr,
        const std::string& optId )
    const;
    std::tuple< atracsys::Status, int32 > getIntOption(
        const std::string& optId,
        ftkOptionGetter    what ) const;
    std::tuple< atracsys::Status, int32 > getIntOption(
        const std::string& optId ) const;

    std::tuple< atracsys::Status, float > getFloatOption(
        uint64          serialNbr,
        uint32          optId,
        ftkOptionGetter what )
    const;
    std::tuple< atracsys::Status, float > getFloatOption( uint64 serialNbr,
                                                          uint32 optId ) const;
    std::tuple< atracsys::Status, float > getFloatOption( uint32          optId,
                                                          ftkOptionGetter what )
    const;
    std::tuple< atracsys::Status, float > getFloatOption( uint32 optId ) const;
    std::tuple< atracsys::Status, float > getFloatOption(
        uint64             serialNbr,
        const std::string& optId,
        ftkOptionGetter    what )
    const;
    std::tuple< atracsys::Status, float > getFloatOption(
        uint64             serialNbr,
        const std::string& optId )
    const;
    std::tuple< atracsys::Status, float > getFloatOption(
        const std::string& optId,
        ftkOptionGetter    what ) const;
    std::tuple< atracsys::Status, float > getFloatOption(
        const std::string& optId ) const;

    std::tuple< atracsys::Status, std::string > getDataOption( uint64 serialNbr,
                                                               uint32 optId )
    const;
    std::tuple< atracsys::Status,
                std::string > getDataOption( uint32 optId ) const;
    std::tuple< atracsys::Status, std::string > getDataOption(
        uint64             serialNbr,
        const std::string& optId )
    const;
    std::tuple< atracsys::Status, std::string > getDataOption(
        const std::string& optId ) const;

#ifdef BUILD_WITH_PRIVATE_SDK
    std::tuple< atracsys::Status, std::vector< atracsys::DeviceRegister > > getEnumeratedRegisters(
      uint64 serialNbr ) const;
    std::tuple< atracsys::Status, std::vector< atracsys::DeviceRegister > > getEnumeratedRegisters() const;

    std::tuple< atracsys::Status, uint32 > getRegisterValue( uint64 serialNbr,
                                                             uint32 regId )
    const;
    std::tuple< atracsys::Status,
                uint32 > getRegisterValue( uint32 regId ) const;
    std::tuple< atracsys::Status, uint32 > getRegisterValue(
        uint64             serialNbr,
        const std::string& regId )
    const;
    std::tuple< atracsys::Status, uint32 > getRegisterValue(
        const std::string& regId ) const;
#endif

    atracsys::Status setGeometry( uint64                 serialNbr,
                                  const WrappedGeometry& geom );
    atracsys::Status setGeometry( const WrappedGeometry& geom );
    atracsys::Status setGeometry( uint64             serialNbr,
                                  const std::string& geomFile );
    atracsys::Status setGeometry( const std::string& geomFile );

    std::tuple< atracsys::Status, WrappedGeometry > loadRigidBody( const std::string& fileName );
    atracsys::Status saveRigidBody( const std::string& fileName, const WrappedGeometry& geom );

    std::tuple< atracsys::Status, std::vector< WrappedPoint > > getAccelerometerDataForDevice(
      uint64_t serialNbr );
    std::tuple< atracsys::Status, std::vector< WrappedPoint > > getAccelerometerDataForDevice();
    std::tuple< atracsys::Status, std::vector< std::vector< WrappedPoint > > > getAccelerometerData();

    std::tuple< atracsys::Status, atracsys::timestamp_t > getRealTimeClockForDevice(
      uint64_t serialNbr );
    std::tuple< atracsys::Status, atracsys::timestamp_t > getRealTimeClockForDevice();
    std::tuple< atracsys::Status, std::vector< atracsys::timestamp_t > > getRealTimeClock();
};
