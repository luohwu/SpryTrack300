#include "WrappedGeometry.hpp"
#include "WrappedPoint.hpp"
#include "WrappedTrackingSystem.hpp"

#include <TrackingSystem.hpp>

#include <pybind11/chrono.h>
#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <iostream>
#include <tuple>

using namespace atracsys;
using namespace pybind11;

typedef std::vector< DeviceOption > OptionsContainer;

typedef std::vector< RawData > RawDataContainer;
typedef std::vector< FiducialData > FiducialDataContainer;
typedef std::vector< MarkerData > MarkerDataContainer;
typedef std::vector< EventData > EventDataContainer;

typedef std::vector< FrameData > FrameDataContainer;

typedef std::vector< atracsys::DeviceInfo > DeviceInfoContainer;

PYBIND11_MODULE( MODULE_NAME, atracModule )
{
    atracModule.doc() = "Atracsys C++ API python wrapper";

    enum_< ftkComponent >( atracModule,
                           "Component",
                           module_local(),
                           R"pbdoc(
      Enum for different option category (aka "component" in the c and c++ interfaces).
    )pbdoc" )
    .value( "library",
            ftkComponent::FTK_LIBRARY,
            R"pbdoc(
      Option is specific to the library.
    )pbdoc" )
    .value( "device",
            ftkComponent::FTK_DEVICE,
            R"pbdoc(
      Option is specific to the device.
    )pbdoc" )
    .value( "detector",
            ftkComponent::FTK_DETECTOR,
            R"pbdoc(
      Option is specific to the detection stage done on the host.
    )pbdoc" )
    .value( "match2d3d",
            ftkComponent::FTK_MATCH2D3D,
            R"pbdoc(
      Option is specific to the 3d matching (including marker matching) done on the host.
    )pbdoc" )
    .value( "deviceDetector",
            ftkComponent::FTK_DEVICE_DETECTOR,
            R"pbdoc(
      Option is specific to the detection stage done on the device (only for the spryTrack 180).
    )pbdoc" )
    .value( "deviceMatch2d3d",
            ftkComponent::FTK_DEVICE_MATCH2D3D,
            R"pbdoc(
      Option is specific to the 3d matching (including marker matching) done on the device (only for the spryTrack 180).
    )pbdoc" )
    .value( "deviceWireless",
            ftkComponent::FTK_DEVICE_WIRELESS,
            R"pbdoc(
      Option is specific to the device optical communication.
    )pbdoc" );

    enum_< ftkOptionType >( atracModule,
                            "OptionType",
                            module_local(),
                            R"pbdoc(
      Enum for different option types.
    )pbdoc" )
    .value( "int32",
            ftkOptionType::FTK_INT32,
            R"pbdoc(
      Integer option (on 32 bits).
    )pbdoc" )
    .value( "float32",
            ftkOptionType::FTK_FLOAT32,
            R"pbdoc(
      Float option (on 32 bits).
    )pbdoc" )
    .value( "data",
            ftkOptionType::FTK_DATA,
            R"pbdoc(
      Data option. Usually char array. It can also be raw bytes.
    )pbdoc" );

    enum_< ftkOptionGetter >( atracModule,
                              "OptionGetter",
                              module_local(),
                              R"pbdoc(
      Enum for different types of get requests on the options.
    )pbdoc" )
    .value( "min_value",
            ftkOptionGetter::FTK_MIN_VAL,
            R"pbdoc(
      Retrieve the minimum value of the option.
    )pbdoc" )
    .value( "max_value",
            ftkOptionGetter::FTK_MAX_VAL,
            R"pbdoc(
      Retrieve the maximum value of the option.
    )pbdoc" )
    .value( "default_value",
            ftkOptionGetter::FTK_DEF_VAL,
            R"pbdoc(
      Retrieve the default value of the option.
    )pbdoc" )
    .value( "current_value",
            ftkOptionGetter::FTK_VALUE,
            R"pbdoc(
      Retrieve the actual value of the option.
    )pbdoc" );

    enum_< ftkPixelFormat >( atracModule, "PixelFormat", module_local(),
                             R"pbdoc(
      Enum for different pixel format for images.
    )pbdoc" )
      .value( "gray8", ftkPixelFormat::GRAY8,
              R"pbdoc(
      Pixels are represented as grayscale values ranging from 0
      (black) to 255 (white).
    )pbdoc" )
      .value( "gray8VIS", ftkPixelFormat::GRAY8_VIS,
              R"pbdoc(
      Pixels are represented as grayscale values ranging from 0
      (black) to 255 (white).

      Images have been taken for augmented reality purpose and are not suitable for marker
      recognition.
    )pbdoc" )
      .value( "gray16", ftkPixelFormat::GRAY16,
              R"pbdoc(
      Pixels are represented as grayscale values ranging from 0
      (black) to 4095 (white).
    )pbdoc" )
      .value( "gray16VIS", ftkPixelFormat::GRAY16_VIS,
              R"pbdoc(
      Pixels are represented as grayscale values ranging from 0
      (black) to 4095 (white).

      Images have been taken for augmented reality purpose and are not suitable for marker
      recognition.
    )pbdoc" )
      .value( "gray8SL", ftkPixelFormat::GRAY8_SL,
              R"pbdoc(
      Pixels are represented as grayscale values ranging from 0
      (black) to 255 (white).

      Images have been taken structured light enabled and are not suitable for marker
      recognition.
    )pbdoc" )
      .value( "gray16SL", ftkPixelFormat::GRAY16_SL,
              R"pbdoc(
      Pixels are represented as grayscale values ranging from 0
      (black) to 4095 (white).

      Images have been taken structured light enabled and are not suitable for marker
      recognition.
    )pbdoc" );

    enum_< ftkDeviceType >( atracModule, "DeviceType", module_local(),
                            R"pbdoc(
      Enum for different types of connected devices.
    )pbdoc" )
      .value( "simulator", ftkDeviceType::DEV_SIMULATOR,
              R"pbdoc(
      Generic simulator (simulates a fusionTrack 500).
    )pbdoc" )
      .value( "infiniTrack", ftkDeviceType::DEV_INFINITRACK,
              R"pbdoc(
      infiniTrack. This product is no longer supported.
    )pbdoc" )
      .value( "fusionTrack500", ftkDeviceType::DEV_FUSIONTRACK_500,
              R"pbdoc(
      fusionTrack 500 (https://www.atracsys-measurement.com/products/fusiontrack-500/).
    )pbdoc" )
      .value( "fusionTrack250", ftkDeviceType::DEV_FUSIONTRACK_250,
              R"pbdoc(
      fusionTrack 250 (https://www.atracsys-measurement.com/products/fusiontrack-250/).
    )pbdoc" )
      .value( "spryTrack180", ftkDeviceType::DEV_SPRYTRACK_180,
              R"pbdoc(
      spryTrack 180 (https://www.atracsys-measurement.com/products/sprytrack-180/).
    )pbdoc" )
      .value( "spryTrack300", ftkDeviceType::DEV_SPRYTRACK_300,
              R"pbdoc(
      spryTrack 180 (https://www.atracsys-measurement.com/products/sprytrack-300/).
    )pbdoc" )
      .value( "simulatorfTk500", ftkDeviceType::SIM_FUSIONTRACK_500,
              R"pbdoc(
      Simulator of a fusionTrack 500.
    )pbdoc" )
      .value( "simulatorfTk250", ftkDeviceType::SIM_FUSIONTRACK_250,
              R"pbdoc(
      Simulator of a fusionTrack 250.
    )pbdoc" )
      .value( "simulatorsTk180", ftkDeviceType::SIM_SPRYTRACK_180,
              R"pbdoc(
      Simulator of a spryTrack 180.
    )pbdoc" )
      .value( "simulatorsTk300", ftkDeviceType::SIM_SPRYTRACK_300,
              R"pbdoc(
      Simulator of a spryTrack 300.
    )pbdoc" )
      .value( "unknown", ftkDeviceType::DEV_UNKNOWN_DEVICE,
              R"pbdoc(
      Unknown device, i.e. not supported by this SDK.
    )pbdoc" );

    enum_< FtkEventType >( atracModule, "EventType", module_local(),
                           R"pbdoc(
        Definitions of the event types.

        The event type indicates both the type *and* the version.
    )pbdoc" )
      .value( "LowTemp", FtkEventType::fetLowTemp,
              R"pbdoc(
      Low temperature event. The device temperature is outside the range of temperatures used for calibration.

      This event has no payload.
    )pbdoc" )
      .value( "HighTemp", FtkEventType::fetHighTemp,
              R"pbdoc(
      High temperature event. The device temperature is outside the range of temperatures used for calibration.

      This event has no payload.
    )pbdoc" )
      .value( "TempV4", FtkEventType::fetTempV4,
              R"pbdoc(
      Temperature data event.
    )pbdoc" )
      .value( "FansV1", FtkEventType::fetFansV1,
              R"pbdoc(
      Fans data event.
    )pbdoc" )
      .value( "ActiveMarkersMaskV1", FtkEventType::fetActiveMarkersMaskV1,
              R"pbdoc(
      Active marker mask data event (to know how many active markers are paired).
    )pbdoc" )
      .value( "ActiveMarkersButtonStatusV1", FtkEventType::fetActiveMarkersButtonStatusV1,
              R"pbdoc(
      Active marker button data event (to know which buttons on which active markers are pressed).

      This event is only sent if the option with name "Active Wireless button statuses streaming" is set to 1
      (using :func:`~atracsys.TrackingSystem.set_int_option`)
    )pbdoc" )
      .value( "ActiveMarkersBatteryStateV1", FtkEventType::fetActiveMarkersBatteryStateV1,
              R"pbdoc(
      Active marker battery data event (to know what is the state of the battery for each marker).

      This event is only sent if the option with name "Active Wireless battery state streaming" is set to 1
      (using :func:`~atracsys.TrackingSystem.set_int_option`)
    )pbdoc" )
      .value( "SyntheticTemperaturesV1", FtkEventType::fetSyntheticTemperaturesV1,
              R"pbdoc(
        Synthetic temperatures data.
)pbdoc" )
      .value( "SynchronisationPTPV1", FtkEventType::fetSynchronisationPTPV1,
              R"pbdoc(
        PTP synchronisation data.
)pbdoc" );

    enum_< ftkFiducialType >( atracModule, "FiducialType", module_local() )
      .value( "Unspecified", ftkFiducialType::Unspecified,
              R"pbdoc(
        Unspecified fiducial type. Used for instance when an old geometry file is loaded.
)pbdoc" )
      .value( "GenericDisc", ftkFiducialType::GenericDisc,
              R"pbdoc(
        Generic disc fiducial.
)pbdoc" )
      .value( "NavexV1", ftkFiducialType::NavexV1,
              R"pbdoc(
        Navex v1 type.

        Corresponds to SKU 1031802001.
)pbdoc" )
      .value( "NavexV2", ftkFiducialType::NavexV2,
              R"pbdoc(
        Navex v2 type.

        Corresponds to SKU 1031803000.
)pbdoc" )
      .value( "FlatMarker", ftkFiducialType::FlatMarker,
              R"pbdoc(
        Flat marker

        This is the BlueBelt Tech Flat marker.
)pbdoc" )
      .value( "GenericSphere", ftkFiducialType::GenericSphere,
              R"pbdoc(
        Generic reflecting sphere fiducial.
)pbdoc" )
      .value( "SphereIllumarkScrew", ftkFiducialType::SphereIllumarkScrew,
              R"pbdoc(
        Sphere screw system Illumark

        Corresponds to SKU 1031902001.
)pbdoc" )
      .value( "SphereIllumarkSnap", ftkFiducialType::SphereIllumarkSnap,
              R"pbdoc(
        Sphere snap system Illumark

        Corresponds to SKU 1031901001.
)pbdoc" )
      .value( "SphereAxios3dSort", ftkFiducialType::SphereAxios3dSort,
              R"pbdoc(
        Axios 3D SORT Targets.
)pbdoc" )
      .value( "GlassSphereGeneric", ftkFiducialType::GlassSphereGeneric,
              R"pbdoc(
        Generic glass sphere.
)pbdoc" )
      .value( "GlassSphereNdiRadix", ftkFiducialType::GlassSphereNdiRadix,
              R"pbdoc(
        NDI Radix sphere.
)pbdoc" )
      .value( "LEDGeneric", ftkFiducialType::LEDGeneric,
              R"pbdoc(
        Generic LED fiducial.
)pbdoc" )
      .value( "LEDAtracsys", ftkFiducialType::LEDAtracsys,
              R"pbdoc(
        Atracsys LED fiducial.
)pbdoc" )
      .value( "LEDAtracsysAutoclavable", ftkFiducialType::LEDAtracsysAutoclavable,
              R"pbdoc(
        Autoclavable Atracsys LED fiducial.
)pbdoc" );

    enum_< ftkErrorPTP >( atracModule, "ErrorPTP", module_local(),
                          R"pbdoc(
        This lists the possible returned error by the PTP protocol
)pbdoc" )
      .value( "OK", ftkErrorPTP::Ok,
              R"pbdoc(
        No error.
)pbdoc" )
      .value( "NoMasterDetected", ftkErrorPTP::NoMasterDetected,
              R"pbdoc(
        No master detected.
)pbdoc" )
      .value( "NetworkTimeoutError", ftkErrorPTP::NetworkTimeoutError,
              R"pbdoc(
        Too long time waiting packets in the switch or in the master.
)pbdoc" )
      .value( "AnnounceWrongIntervalError", ftkErrorPTP::AnnounceWrongIntervalError,
              R"pbdoc(
        Wrong interval between Announce message detected, please check if the setup value of `Log
        Announce Interval' is correct.
)pbdoc" )
      .value( "SyncWrongInternalError", ftkErrorPTP::SyncWrongInternalError,
              R"pbdoc(
        Wrong interval between Sync message detected, please check if “Log Sync Interval” is correct.
)pbdoc" )
      .value( "TooMuchMastersError", ftkErrorPTP::TooMuchMastersError,
              R"pbdoc(
        There is too much foreign master (more than five) communicating in the network.
)pbdoc" )
      .value( "NoFollowUp", ftkErrorPTP::NoFollowUp,
              R"pbdoc(
        No follow-up message detected but the two-step flag was set.
)pbdoc" )
      .value( "FollowUpSyntaxError", ftkErrorPTP::FollowUpSyntaxError,
              R"pbdoc(
        Problem encountered with the follow-up messages (check length and sequence ID).
)pbdoc" )
      .value( "NoDelayAnswer", ftkErrorPTP::NoDelayAnswer,
              R"pbdoc(
        No delay-resp message detected from the parent Master (or in wrong order).
)pbdoc" )
      .value( "DelayAnswerSyntaxError", ftkErrorPTP::DelayAnswerSyntaxError,
              R"pbdoc(
        Problem encounter with the delay-resp messages (check length and sequence ID).
)pbdoc" )
      .value( "UnstableSynchronisationError", ftkErrorPTP::UnstableSynchronisationError,
              R"pbdoc(
        The Precision Time Protocol can be applied but the synchronization appears to be unstable.
)pbdoc" )
      .value( "NoSyncFromMaster", ftkErrorPTP::NoSyncFromMaster,
              R"pbdoc(
        No sync message detected from the parent Master (or in wrong order).
)pbdoc" )
      .value( "SyncMessageError", ftkErrorPTP::SyncMessageError,
              R"pbdoc(
        Problem encounter with the sync messages (check length and sequence ID).
)pbdoc" )
      .value( "WrongDomain", ftkErrorPTP::WrongDomain,
              R"pbdoc(
        Wrong PTP domain (check if the Domain Number has been correctly set).
)pbdoc" )
      .value( "UnsupportedVersion", ftkErrorPTP::UnsupportedVersion,
              R"pbdoc(
        The PTP version is not supported (only v2.0 supported).
)pbdoc" )
      .value( "UnsupportedFlags", ftkErrorPTP::UnsupportedFlags,
              R"pbdoc(
        Messages flags do not correspond to fTk PTP usage.

        Please note the alternate master, unicast and profile specific flags should be zero as not supported.
)pbdoc" )
      .value( "UnknownError", ftkErrorPTP::UnknownError,
              R"pbdoc(
        Unknown error, used if the error ID cannot be transformed in a ftkErrorPTP.
)pbdoc" )
      .value( "LastError", ftkErrorPTP::LastError,
              R"pbdoc(
        Last error (currently max size for Error ID is 5 bits).
)pbdoc" );

    enum_< ftkPortStatePTP >( atracModule, "PortStatePTP", module_local(), R"pbdoc(
        State of the PTP module state machine.
)pbdoc" )
      .value("Initialising", ftkPortStatePTP::Initialising, R"pbdoc(
        State to initialize the data sets (instance not sync, no PTP communication).
)pbdoc")
      .value("Faulty", ftkPortStatePTP::Faulty, R"pbdoc(
        State to indicate failure (instance not sync, no PTP communication).
)pbdoc")
      .value("Listening", ftkPortStatePTP::Listening, R"pbdoc(
        State in which the instance listen to Announce messages and apply the BMCA (instance not sync,
        PTP communication only for reception).
)pbdoc")
      .value("Uncalibrate", ftkPortStatePTP::Uncalibrate, R"pbdoc(
        State used when a new master is detected. Initialize the synchronisation servos and getting
        synchronised (instance not sync, slave PTP communication active).
)pbdoc")
      .value("Slave", ftkPortStatePTP::Slave, R"pbdoc(
        State in which the instance is synchronised to the selected master port (instance sync, slave
        PTP communication active)
)pbdoc")
      .value("Unknown", ftkPortStatePTP::UnknownState, R"pbdoc(
        Unknown state, used if the convertion could not be performed.
)pbdoc");

    class_< ftkFanStatus >( atracModule, "FanStatus", module_local() )
    .def( init( [ ]( uint8_t value ){
        union { ftkFanStatus bitfield; uint8_t number; };
        number = value;
        return bitfield;
    } ) )
    .def_property_readonly( "fans_enabled",
                            [ ](
                                ftkFanStatus item ){
        return static_cast< bool >( item.FanModuleEnabled );
    },
                            R"pbdoc(
      bool: True if the fan module is enable, False otherwise.
    )pbdoc" )
    .def_property_readonly( "fan0_pwm_enabled",
                            [ ](
                                ftkFanStatus item ){
        return static_cast< bool >( item.Fan0PWMEnabled );
    },
                            R"pbdoc(
        bool: True if the fan 0 duty cycle is enable, False otherwise.
    )pbdoc" )
    .def_property_readonly( "fan1_pwm_enabled",
                            [ ](
                                ftkFanStatus item ){
        return static_cast< bool >( item.Fan1PWMEnabled );
    },
                            R"pbdoc(
        bool: True if the fan 1 duty cycle is enable, False otherwise.
    )pbdoc" )
    .def_property_readonly( "fan0_speed_enabled",
                            [ ](
                                ftkFanStatus item ){
        return static_cast< bool >( item.Fan0SpeedValid );
    },
                            R"pbdoc(
        bool: True if the fan 0 speed is valid, False otherwise.
    )pbdoc" )
    .def_property_readonly( "fan1_speed_enabled",
                            [ ](
                                ftkFanStatus item ){
        return static_cast< bool >( item.Fan1SpeedValid );
    },
                            R"pbdoc(
        bool: True if the fan 1 speed is valid, False otherwise.
    )pbdoc" );

    class_< ftkStatus >( atracModule, "DataStatus", module_local() )
    .def( init( [ ]( uint32_t value ){
        union { ftkStatus bitfield; uint32_t number; };
        number = value;
        return bitfield;
    } ) )
    .def_property_readonly( "right_edge",
                            [ ](
                                ftkStatus item ){
        return static_cast< bool >( item.RightEdge );
    },
                            R"pbdoc(
        bool: True if the blob touches the right edge of the picture, False otherwise.
    )pbdoc" )
    .def_property_readonly( "bottom_edge",
                            [ ](
                                ftkStatus item ){
        return static_cast< bool >( item.BottomEdge );
    },
                            R"pbdoc(
        bool: True if the blob touches the bottom edge of the picture, False otherwise.
    )pbdoc" )
    .def_property_readonly( "left_edge",
                            [ ](
                                ftkStatus item ){
        return static_cast< bool >( item.LeftEdge );
    },
                            R"pbdoc(
        bool: True if the blob touches the left edge of the picture, False otherwise.
    )pbdoc" )
    .def_property_readonly( "top_edge",
                            [ ](
                                ftkStatus item ){
        return static_cast< bool >( item.TopEdge );
    },
                            R"pbdoc(
        bool: True if the blob touches the top edge of the picture, False otherwise.
    )pbdoc" )
    .def_property_readonly( "sensor_crop",
                            [ ](
                                ftkStatus item ){
        return static_cast< bool >( item.SensorCrop );
    },
                            R"pbdoc(
        bool: True if the blob lies in the part of the sensor not included in the accuracy measurement volume, False otherwise.
    )pbdoc" )
    .def_property_readonly( "working_volume",
                            [ ](
                                ftkStatus item ){
        return static_cast< bool >( item.WorkingVolume );
    },
                            R"pbdoc(
        bool: True if the 3D fiducial has a z value outside the allowed range. False otherwise.
    )pbdoc" );

    class_< ftkOptionStatus >( atracModule, "OptionStatus", module_local() )
    .def( init( [ ]( uint32_t value ){
        union { ftkOptionStatus bitfield; uint32 number; };
        number = value;
        return bitfield;
    } ) )
    .def_property_readonly( "read",
                            [ ](
                                ftkOptionStatus item ){
        return item.read;
    },
                            R"pbdoc(
        bool: True if the option can be read, False otherwise.
    )pbdoc" )
    .def_property_readonly( "write",
                            [ ](
                                ftkOptionStatus item ){
        return item.write;
    },
                            R"pbdoc(
        bool: True if the option can be written, False otherwise.
    )pbdoc" )
    .def_property_readonly( "access_protected",
                            [ ](
                                ftkOptionStatus item ){
        return item.accessProtected;
    },
                            R"pbdoc(
        bool: True if the option as a "protected" access. Used internally only.
    )pbdoc" )
    .def_property_readonly( "access_private",
                            [ ](
                                ftkOptionStatus item ){
        return item.accessPrivate;
    },
                            R"pbdoc(
        bool: True if the option as a "private" access. Used internally only.
    )pbdoc" )
    .def_property_readonly( "global_option",
                            [ ](
                                ftkOptionStatus item ){
        return item.globalOption;
    },
                            R"pbdoc(
        bool: True if the option is global. Used internally only.
    )pbdoc" );

    class_< WrappedPoint >( atracModule,
                            "Ftk3dPoint",
                            "Class defining a 3D point",
                            module_local() )
    .def( init< float, float, float >() )
    .def_property( "x", [ ]( const WrappedPoint& pt ){
        return pt.x;
    }, [ ]( WrappedPoint& pt, float val ){
        pt.x = val;
    }, "float: x coordinate (in mm)." )
    .def_property( "y", [ ]( const WrappedPoint& pt ){
        return pt.y;
    }, [ ]( WrappedPoint& pt, float val ){
        pt.y = val;
    }, "float: y coordinate (in mm)." )
    .def_property( "z", [ ]( const WrappedPoint& pt ){
        return pt.z;
    }, [ ]( WrappedPoint& pt, float val ){
        pt.z = val;
    }, "float: z coordinate (in mm)." );

    class_< WrappedFiducial >( atracModule, "FtkFiducial", "Class defining a fiducial", module_local() )
      .def( init< float, float, float, float, float, float, ftkFiducialType, float >() )
      .def_property(
        "position", &WrappedFiducial::getPosition, &WrappedFiducial::setPosition,
        "Ftk3dPoint: coordinates (in mm) of the fiducial." )
      .def_property(
        "normalVector", &WrappedFiducial::getNormalVector, &WrappedFiducial::setNormalVector,
        "Ftk3dPoint: normal vector for the fiducial." )
      .def_property(
        "fiducial_type", []( const WrappedFiducial& fid ) { return fid.fiducialInfo.type; },
        []( WrappedFiducial& fid, ftkFiducialType type ) { fid.fiducialInfo.type = type; },
        "FiducialType: type of fiducial" )
      .def_property(
        "angle_of_view", []( const WrappedFiducial& fid ) { return fid.fiducialInfo.angleOfView; },
        []( WrappedFiducial& fid, float angle ) { fid.fiducialInfo.angleOfView = angle; },
        "float: angle of view" );

    class_< WrappedGeometry >( atracModule, "Geometry", module_local(),
                               "Class handling a marker geometry, wrapping the C++ ftkGeometry struct." )
      .def( init<>() )
      .def( init< ftkRigidBody >() )
      .def_property( "geometry_id", &WrappedGeometry::getGeometryId, &WrappedGeometry::setGeometryId,
                     "int: The identifier of the geometry." )
      .def_property( "version", &WrappedGeometry::getVersion, &WrappedGeometry::setVersion,
                     "int: Version of the geometry structure." )
      .def_property( "points_count", &WrappedGeometry::getPointsCount, &WrappedGeometry::setPointsCount,
                     "int: Number of points." )
      .def_property( "fiducials", &WrappedGeometry::getPositions, &WrappedGeometry::setPositions,
                     "list[FtkFiducials]: positions of the fiducials." )
      .def_property( "divots_count", &WrappedGeometry::getDivotsCount, &WrappedGeometry::setDivotsCount,
                     "int: Number of divots." )
      .def_property( "divots", &WrappedGeometry::getDivots, &WrappedGeometry::setDivots,
                     "list[Ftk3dPoint]: positions of the divots." );

    enum_< atracsys::Status >( atracModule,
                               "Status",
                               module_local(),
                               R"pbdoc(
      Enum used as a return type for most of the library functions.
    )pbdoc" )
    .value( "Ok", atracsys::Status::Ok, "Everything fine." )
    .value( "LibNotInitialised",
            atracsys::Status::LibNotInitialised,
            "The library is not initialised. A call to :func:`~atracsys.TrackingSystem.initialise` is probably missing." )
    .value( "NoDevices",
            atracsys::Status::NoDevices,
            "The library has no connected devices. A call to :func:`~atracsys.TrackingSystem.enumerate_devices` is probably missing or did not return ok." )
    .value( "SeveralDevices",
            atracsys::Status::SeveralDevices,
            "The library has several connected devices and does not know on which one to act." )
    .value( "UnknownDevice",
            atracsys::Status::UnknownDevice,
            "The library is connected to an unsupported device." )
    .value( "InvalidOption",
            atracsys::Status::InvalidOption,
            "The option that is getting accessed does not exist." )
    .value( "UnmatchingSizes", atracsys::Status::UnmatchingSizes )
    .value( "SdkWarning",
            atracsys::Status::SdkWarning,
            "The SDK did return some warnings. They can be accessed using :func:`~atracsys.TrackingSystem.get_last_error`." )
    .value( "SdkError",
            atracsys::Status::SdkError,
            "The SDK did return some errors. They can be accessed using :func:`~atracsys.TrackingSystem.get_last_error`." )
    .value( "InvalidFile",
            atracsys::Status::InvalidFile,
            "An invalid file has been given as input." )
    .value( "ParsingError",
            atracsys::Status::ParsingError,
            "An file with invalid content has been given as input." );

    enum_< LogLevel >( atracModule, "LogLevel", module_local() )
    .value( "All",
            LogLevel::All,
            R"pbdoc(
      The most verbose level.
    )pbdoc" )
    .value( "Verbose", LogLevel::Verbose, R"pbdoc(
      Verbose.
    )pbdoc" )
    .value( "Debug", LogLevel::Debug, R"pbdoc(
      Debug.
    )pbdoc" )
    .value( "Info", LogLevel::Info, R"pbdoc(
      Info.
    )pbdoc" )
    .value( "Warning", LogLevel::Warning, R"pbdoc(
      Warning.
    )pbdoc" )
    .value( "Error", LogLevel::Error, R"pbdoc(
      Error.
    )pbdoc" )
    .value( "None",
            LogLevel::None,
            R"pbdoc(
      No output is shown.
    )pbdoc" );


    class_< DeviceOption >( atracModule,
                            "DeviceOption",
                            module_local(),
                            R"pbdoc(
      This class contains the info about a specific option.
    )pbdoc" )
    .def( init<>() )
    .def( init< uint32, ftkComponent, ftkOptionStatus, ftkOptionType,
                const std::string&, const std::string&, const std::string& >(),
          arg( "id" ), arg( "component" ), arg( "status" ), arg( "type" ),
          arg( "name" ), arg( "desc" ), arg( "unit" ) = std::string( "" ) )
    .def_property_readonly( "id",
                            &DeviceOption::id,
                            "ID of the option",
                            R"pbdoc(
     int: The option unique ID.

      .. warning::

            This unique ID can be changed by Atracsys without notice. The correct
            identifier to track option in your software is the option :func:`~atracsys.DeviceOption.name`.

    )pbdoc" )
    .def_property_readonly( "component",
                            &DeviceOption::component,
                            R"pbdoc(
        Component: The option category.
    )pbdoc" )
    .def_property_readonly( "status",
                            &DeviceOption::status,
                            R"pbdoc(
      OptionStatus: The status of the option (wether the option is private/public/global, readable or writable )
    )pbdoc" )
    .def_property_readonly( "type",
                            &DeviceOption::type,
                            R"pbdoc(
      OptionType: The option type.

      Using this value, one can determine which call is needed to get (:func:`~atracsys.TrackingSystem.get_data_option`, :func:`~atracsys.TrackingSystem.get_float_option`
      or :func:`~atracsys.TrackingSystem.get_int_option`) or set (:func:`~atracsys.TrackingSystem.set_data_option`, :func:`~atracsys.TrackingSystem.set_float_option`
      or :func:`~atracsys.TrackingSystem.set_int_option`) the option.
    )pbdoc" )
    .def_property_readonly( "name",
                            &DeviceOption::name,
                            return_value_policy::reference,
                            R"pbdoc(
      str: The option name.

      .. note::

            If you need to modify an option in your program, the :func:`~atracsys.DeviceOption.name` is the correct identifier to use.
            Atracsys does guarantee that it will not change between SDK releases.
    )pbdoc" )
    .def_property_readonly( "description",
                            &DeviceOption::description,
                            return_value_policy::reference,
                            R"pbdoc(
      str: The description of the option.
    )pbdoc" )
    .def_property_readonly( "unit",
                            &DeviceOption::unit,
                            return_value_policy::reference,
                            R"pbdoc(
      str: the unit of the option (if available).
    )pbdoc" );

#ifdef BUILD_WITH_PRIVATE_SDK
    class_< DeviceRegister >( atracModule, "DeviceRegister", module_local(),
                              R"pbdoc(
    This class contains the info about a specific register.
)pbdoc" )
      .def_property_readonly( "address", &DeviceRegister::address,
                              R"pbdoc(
    int: address of the register.
)pbdoc" )
      .def_property_readonly( "name", &DeviceRegister::name, return_value_policy::reference,
                              R"pbdoc(
    str: name of the register.
)pbdoc" )
      .def_property_readonly( "description", &DeviceRegister::description, return_value_policy::reference,
                              R"pbdoc(
    str: description of the register.
)pbdoc" )
      .def_property_readonly( "readable", &DeviceRegister::isReadable, return_value_policy::reference,
                              R"pbdoc(
    bool: is the register readable.
)pbdoc" )
      .def_property_readonly( "writeable", &DeviceRegister::isWriteable, return_value_policy::reference,
                              R"pbdoc(
    bool: is the register writeable.
)pbdoc" );
#endif

    class_< DeviceInfo >( atracModule, "DeviceInfo", module_local(),
                          R"pbdoc(
      This class contains the info about a connected device (tracker).
    )pbdoc" )
      .def_property_readonly( "serial_number", &DeviceInfo::serialNumber,
                              R"pbdoc(
      int: serial number of the device (same as on the back sticker).
    )pbdoc" )
      .def_property_readonly( "type", &DeviceInfo::type,
                              R"pbdoc(
      DeviceType: the type of the device (simulator, fusionTrack 250, fusionTrack 500 or simulator)
    )pbdoc" )
      .def_property_readonly( "options", &DeviceInfo::options, return_value_policy::reference,
                              "Device options",
                              R"pbdoc(
      list[DeviceOption]: all the available options of a device.

      These options can then be accessed using
      :func:`~atracsys.TrackingSystem.get_data_option`, :func:`~atracsys.TrackingSystem.get_float_option`
      and :func:`~atracsys.TrackingSystem.get_int_option`.

      These options can be changed using
      :func:`~atracsys.TrackingSystem.set_data_option`, :func:`~atracsys.TrackingSystem.set_float_option`
      and :func:`~atracsys.TrackingSystem.set_int_option`.
    )pbdoc" );

#ifdef BUILD_WITH_PRIVATE_SDK
    class_< DeviceInfoPrivate >( atracModule, "DeviceInfoPrivate", module_local(),
                                 R"pbdoc(
      This class contains the private info about a connected device (tracker).
    )pbdoc" )
      .def_property_readonly( "serial_number", &DeviceInfo::serialNumber,
                              R"pbdoc(
      int: serial number of the device (same as on the back sticker).
    )pbdoc" )
      .def_property_readonly( "type", &DeviceInfo::type,
                              R"pbdoc(
      DeviceType: the type of the device (simulator, fusionTrack 250, fusionTrack 500 or simulator)
    )pbdoc" )
      .def_property_readonly( "registers", &DeviceInfoPrivate::registers, return_value_policy::reference,
                              "Device registers",
                              R"pbdoc(
      list[DeviceOption]: all the available registers of a device.

      These registers can then be accessed using
      :func:`~atracsys.TrackingSystem.get_data_option`.

      These options can be changed using
      :func:`~atracsys.TrackingSystem.set_data_option`.
    )pbdoc" )
      .def_property_readonly( "options", &DeviceInfo::options, return_value_policy::reference,
                              "Device options",
                              R"pbdoc(
      list[DeviceOption]: all the available options of a device.

      These options can then be accessed using
      :func:`~atracsys.TrackingSystem.get_data_option`, :func:`~atracsys.TrackingSystem.get_float_option`
      and :func:`~atracsys.TrackingSystem.get_int_option`.

      These options can be changed using
      :func:`~atracsys.TrackingSystem.set_data_option`, :func:`~atracsys.TrackingSystem.set_float_option`
      and :func:`~atracsys.TrackingSystem.set_int_option`.
    )pbdoc" );
#endif

    class_< HeaderData >( atracModule,
                          "HeaderData",
                          module_local(),
                          R"pbdoc(
      This class contains all the frame header data. The
      instance does now if it has been properly set (i.e.
      whether its member values make sense).
      )pbdoc" )
    .def_property_readonly( "timestamp",
                            &HeaderData::timestamp,
                            R"pbdoc(
        int: The frame timestamp in micro-seconds (start at 0 when booting up the device).
    )pbdoc" )
    .def_property_readonly( "counter",
                            &HeaderData::counter,
                            R"pbdoc(
        int: The frame counter (number of frame taken since the boot of the device).
    )pbdoc" )
    .def_property_readonly( "format",
                            &HeaderData::format,
                            R"pbdoc(
        PixelFormat: The format of the pixels in the image.
    )pbdoc" )
    .def_property_readonly( "width",
                            &HeaderData::width,
                            R"pbdoc(
        int: The width of the image.
    )pbdoc" )
    .def_property_readonly( "height",
                            &HeaderData::height,
                            R"pbdoc(
        int: The height of the image.
    )pbdoc" )
    .def_property_readonly( "picture_stride",
                            &HeaderData::pictureStride,
                            R"pbdoc(
        int: The stride of the image (in bytes) as: Image width * size of pixel + padding in bytes.
    )pbdoc" )
    .def_property_readonly( "synchronised_strobe_left",
                            &HeaderData::synchronisedStrobeLeft,
                            R"pbdoc(
        bool: Whether the strobe was synchronised with the current frame for the left camera)pbdoc" )
    .def_property_readonly( "synchronised_strobe_right",
                            &HeaderData::synchronisedStrobeRight,
                            R"pbdoc(
        bool: Whether the strobe was synchronised with the current frame for the right camera)pbdoc" )
    .def_property_readonly( "valid",
                            &HeaderData::valid,
                            R"pbdoc(
        bool: True if the header data is valid, False otherwise.
    )pbdoc" );

    class_< RawData >( atracModule,
                       "RawData",
                       module_local(),
                       R"pbdoc(
      This class contains all the information about a fiducial raw data. The instance does know if it has been
      properly set (i.e. whether its member values make sense).

      Fiducial raw data are low-level detection results from left and right
      images.
      )pbdoc" )
    .def_property_readonly( "index",
                            &RawData::index,
                            R"pbdoc(
        int: The index of the raw fiducial.
    )pbdoc" )
    .def_property_readonly( "position",
                            &RawData::position,
                            return_value_policy::reference,
                            R"pbdoc(
        list[int[2]]: Center of the fiducial in image reference (unit pixels).
    )pbdoc" )
    .def_property_readonly( "dimensions",
                            &RawData::dimensions,
                            return_value_policy::reference,
                            R"pbdoc(
        list[int[2]]: Size of the bounding rectangle (width, height) (unit pixels).
    )pbdoc" )
    .def_property_readonly( "aspect_ratio",
                            &RawData::aspectRatio,
                            R"pbdoc(
        float: The aspect ration of the blob.
    )pbdoc" )
    .def_property_readonly( "status",
                            &RawData::status,
                            R"pbdoc(
        DataStatus: The status of the data (if it is in the working volume, cropped or near an edge).
    )pbdoc" )
    .def_property_readonly( "surface",
                            &RawData::surface,
                            R"pbdoc(
        int: The surface of pixels composing the fiducial (unit pixels).
    )pbdoc" )
    .def_property_readonly( "valid",
                            &RawData::valid,
                            R"pbdoc(
        bool: True if the Fiducial raw data is valid, False otherwise.
    )pbdoc" )
    .def_static( "invalid_instance",
                 RawData::invalidInstance,
                 return_value_policy::reference,
                 R"pbdoc(
      Return an invalid instance of the fiducial raw data (internal use only).

      Returns:
        RawData: an invalid instance of the event.
    )pbdoc" );

    class_< FiducialData >( atracModule,
                            "FiducialData",
                            module_local(),
                            R"pbdoc(
      This class contains all the information about a 3D fiducial. The instance does know if it has been
      properly set (i.e. whether its member values make sense).

      3D fiducials are retrieved after triangulation and matching of raw data.
      The instance contains \\e copies of the two RawData instances used to reconstruct the
      FiducialData one.
      )pbdoc" )
    .def_property_readonly( "index",
                            &FiducialData::index,
                            R"pbdoc(
        int: The 3D fiducial index.
    )pbdoc" )
    .def_property_readonly( "left_instance",
                            &FiducialData::leftInstance,
                            return_value_policy::reference,
                            R"pbdoc(
        RawData: The corresponding raw data in the left image.
    )pbdoc" )
    .def_property_readonly( "right_instance",
                            &FiducialData::rightInstance,
                            return_value_policy::reference,
                            R"pbdoc(
        RawData: The corresponding raw data in the right image.
    )pbdoc" )
    .def_property_readonly( "position",
                            &FiducialData::position,
                            return_value_policy::reference,
                            R"pbdoc(
        list[float[3]]: Return the 3D position of the center of the fiducial (unit mm).
    )pbdoc" )
    .def_property_readonly( "epipolar_error",
                            &FiducialData::epipolarError,
                            R"pbdoc(
      float: Epipolar matching error (unit pixel).

      The epipolar error represents the signed distance between the right
      epipolar line (of the left fiducial) and its matching right fiducial. Units
      are in pixels.
    )pbdoc" )
    .def_property_readonly( "triangulation_error",
                            &FiducialData::triangulationError,
                            R"pbdoc(
      float: Triangulation error (unit mm).

      The triangulation error represents the minimum distance of the 3D lines
      defined by the left optical center and the left projection and the right
      optical center and the right position. Units are in millimeters.
    )pbdoc" )
    .def_property_readonly( "probability",
                            &FiducialData::probability,
                            R"pbdoc(
      float: Probability (range 0..1).

      The probability is defined by the number of potential matches. Basically
      defined by number of potential matched points that are at a specified
      distance from the epipolar lines. This ambiguity is usually disambiguated
      once the 3D point is matched with a marker geometry.
    )pbdoc" )
    .def_property_readonly( "status",
                            &FiducialData::status,
                            R"pbdoc(
      DataStatus: The status of the data (if it is in the working volume, cropped or near an edge).
    )pbdoc" )
    .def_property_readonly( "valid",
                            &FiducialData::valid,
                            R"pbdoc(
      bool: True if the Fiducial data is valid, False otherwise.
    )pbdoc" )
    .def_static( "invalid_instance",
                 FiducialData::invalidInstance,
                 return_value_policy::reference,
                 R"pbdoc(
      Return an invalid instance of the fiducial data (internal use only).

      FiducialData: an invalid instance of the event.
    )pbdoc" );

    class_< MarkerData >( atracModule,
                          "MarkerData",
                          module_local(),
                          R"pbdoc(
      This class contains all the Marker data after left-right triangulation and marker retrieval.

      Marker are retrieved within the 3D fiducials data based on their unique
      geometry. When several markers are used, it is recommended to use specific
      geometries in order to provide a unique tracking.
      )pbdoc" )
    .def_property_readonly( "index",
                            &MarkerData::index,
                            R"pbdoc(
      int: The marker index.
    )pbdoc" )
    .def_property_readonly( "tracking_id",
                            &MarkerData::trackingId,
                            R"pbdoc(
      int: The marker tracking ID.

      Tracking id is provided to differentiate between several markers of the same
      geometry. The id is reset when less than 3 spheres composing the marker are
      visible or if the marker cannot be found again from its last known position.
    )pbdoc" )
    .def_property_readonly( "geometry_id",
                            &MarkerData::geometryId,
                            R"pbdoc(
      int: The marker geometric id, i.e. the unique id of the used geometry..
    )pbdoc" )
    .def_property_readonly( "presence_mask",
                            &MarkerData::presenceMask,
                            R"pbdoc(
      int: The correspondence between geometry index and 3D fiducials indices.

      The geometryPresenceMask allows to get the correspondence between the
      indices of fiducials specified in the geometry and 3D fiducial
      (FiducialData) indexes of the current measure.
    )pbdoc" )
    .def_property_readonly( "registration_error",
                            &MarkerData::registrationError,
                            R"pbdoc(
      float: The registration error.

      Registration error is the mean distance of geometry and measured fiducials,
      expressed in the same referential.
    )pbdoc" )
    .def_property_readonly( "status",
                            &MarkerData::status,
                            R"pbdoc(
      DataStatus: The status of the data (if it is in the working volume, cropped or near an edge).
    )pbdoc" )
    .def_property_readonly( "valid",
                            &MarkerData::valid,
                            R"pbdoc(
        bool: True if the Fiducial data is valid, False otherwise.
    )pbdoc" )
    .def_property_readonly( "position",
                            &MarkerData::position,
                            return_value_policy::reference,
                            R"pbdoc(
        list[float[3]]: Translation (position) vector (unit mm).
    )pbdoc" )
    .def_property_readonly( "rotation",
                            &MarkerData::rotation,
                            return_value_policy::reference,
                            R"pbdoc(
        list[list[float[3]]]: Rotation matrix: format [row][column].
    )pbdoc" )
    .def( "corresponding_fiducial",
          &MarkerData::correspondingFiducial,
          return_value_policy::reference,
          R"pbdoc(
      Parameters:
        arg0: The id of the fiducial.
      Returns:
            FiducialData: The fiducial.
    )pbdoc" );

    class_< FanStateData >( atracModule, "FanStateData", module_local() )
    .def_property_readonly( "pwm_duty",
                            &FanStateData::pwmDuty,
                            R"pbdoc(
        int: The pwm duty cycle of one fan.
    )pbdoc" )
    .def_property_readonly( "speed",
                            &FanStateData::speed,
                            R"pbdoc(
        int: The speed of one fan.
    )pbdoc" );

    typedef std::vector< FanStateData > FanStateContainer;

    class_< EvtFansV1Data >( atracModule,
                             "EvtFansV1Data",
                             module_local(),
                             R"pbdoc(
      This class encapsulate a fan event.
    )pbdoc" )
    .def_property_readonly( "fan_status",
                            &EvtFansV1Data::fansStatus,
                            R"pbdoc(
        FanStatus: The status of the fans (enable / pwm enable / speed valid).
    )pbdoc" )
    .def_property_readonly( "fans",
                            &EvtFansV1Data::fans,
                            return_value_policy::reference,
                            R"pbdoc(
        list[FanStateData]: One data item for each fans (usually 2).
    )pbdoc" )
    .def_property_readonly( "valid",
                            &EvtFansV1Data::valid,
                            R"pbdoc(
        bool: True if the event is valid, False otherwise.
    )pbdoc" )
    .def_static( "invalid_instance",
                 &EvtFansV1Data::invalidInstance,
                 return_value_policy::reference,
                 R"pbdoc(
      Return an invalid instance of the event (for internal use).

      Returns:
        EvtFansV1Data: an invalid instance of the event.
    )pbdoc" );

    class_< TempV4Item >( atracModule, "TempV4Item", module_local() )
    .def_property_readonly( "sensor_id",
                            &TempV4Item::sensorId,
                            R"pbdoc(
        int: The index of the sensor.
    )pbdoc" )
    .def_property_readonly( "sensor_value",
                            &TempV4Item::sensorValue,
                            R"pbdoc(
        float: The value of the sensor.
    )pbdoc" );

    typedef std::vector< TempV4Item > TempV4ItemContainer;

    class_< EvtTemperatureV4Data >( atracModule,
                                    "EvtTemperatureV4Data",
                                    module_local(),
                                    R"pbdoc(
      This class encapsulate a temperature event.
    )pbdoc" )
    .def_property_readonly( "sensors",
                            &EvtTemperatureV4Data::sensors,
                            return_value_policy::reference,
                            R"pbdoc(
      list[TempV4Item]: One data item for each temperature sensors (usually 2).
    )pbdoc" )
    .def_property_readonly( "valid",
                            &EvtTemperatureV4Data::valid,
                            R"pbdoc(
      bool: True if the event is valid, False otherwise.
    )pbdoc" )
    .def_static( "invalid_instance",
                 &EvtTemperatureV4Data::invalidInstance,
                 return_value_policy::reference,
                 R"pbdoc(
      Return an invalid instance of the event (for internal use).

      Returns:
        EvtTemperatureV4Data: an invalid instance of the event.
    )pbdoc" );

    class_< EvtActiveMarkersMaskV1Data >( atracModule,
                                          "EvtActiveMarkersMaskV1Data",
                                          module_local(),
                                          R"pbdoc(
      This class encapsulate an active marker mask event.
    )pbdoc" )
    .def_property_readonly( "active_markers_mask",
                            &EvtActiveMarkersMaskV1Data::activeMarkersMask,
                            R"pbdoc(
        int: The active marker mask. Each bit correspond to one marker. The bit position is the marker ID (bit 0 -> marker with ID 0, ...).
    )pbdoc" )
    .def( "isMarkerPaired",
          &EvtActiveMarkersMaskV1Data::isMarkerPaired,
          R"pbdoc(
      Helper function that inspect the mask to find out if a given marker (identified by its ID) is paired.

      Parameters:
        arg0: The index of an active marker (aka short ID).

      Returns:
        bool: True if marker with given ID is paired, false otherwise.
    )pbdoc" )
    .def_property_readonly( "valid",
                            &EvtActiveMarkersMaskV1Data::valid,
                            R"pbdoc(
        bool: True if the event is valid, False otherwise.
    )pbdoc" )
    .def_static( "invalid_instance",
                 &EvtActiveMarkersMaskV1Data::invalidInstance,
                 return_value_policy::reference,
                 R"pbdoc(
      Return an invalid instance of the event (for internal use).

      Returns:
        EvtActiveMarkersMaskV1Data: an invalid instance of the event.
    )pbdoc" );

    class_< ActiveMarkersButtonStatusesV1Item >( atracModule,
                                                 "ActiveMarkersButtonStatusesV1Item",
                                                 module_local(),
                                                 R"pbdoc(
      This class contains the data of one button status reading.
    )pbdoc" )
    .def_property_readonly( "image_count",
                            &ActiveMarkersButtonStatusesV1Item::imageCount,
                            R"pbdoc(
        int: Frame counter at the time of the reading. Can be linked to the header data
        :func:`~atracsys.HeaderData.counter` field retrieved
        through :func:`~atracsys.TrackingSystem.getLastFrame`.
    )pbdoc" )
    .def_property_readonly( "device_id",
                            &ActiveMarkersButtonStatusesV1Item::deviceID,
                            R"pbdoc(
        int: The short ID of the marker. Can be mapped to the option :func:`~atracsys.TrackingSystem.get_data_option` with
        option name "Active Wireless Markers info"
    )pbdoc" )
    .def_property_readonly( "button_status",
                            &ActiveMarkersButtonStatusesV1Item::buttonStatus,
                            R"pbdoc(
        int: The button state. Bit 0 is for the first button, bit 1 for the second.
    )pbdoc" );

    typedef std::vector< ActiveMarkersButtonStatusesV1Item >
        ActMrkButtonStatusesContainer;

    class_< EvtActiveMarkersButtonStatusesV1Data >( atracModule,
                                                    "EvtActiveMarkersButtonStatusesV1Data",
                                                    module_local(),
                                                    R"pbdoc(
      This class encapsulate an active marker button event.
    )pbdoc" )
    .def_property_readonly( "statuses",
                            &EvtActiveMarkersButtonStatusesV1Data::statuses,
                            return_value_policy::reference,
                            R"pbdoc(
        list[ActiveMarkersButtonStatusesV1Item]: The list of active marker button statuses (one item per marker)
    )pbdoc" )
    .def_property_readonly( "valid",
                            &EvtActiveMarkersButtonStatusesV1Data::valid,
                            R"pbdoc(
        bool: True if the event is valid, False otherwise.
    )pbdoc" )
    .def_static( "invalid_instance",
                 &EvtActiveMarkersButtonStatusesV1Data::invalidInstance,
                 return_value_policy::reference,
                 R"pbdoc(
      Return an invalid instance of the event (for internal use).

      Returns:
        EvtActiveMarkersButtonStatusesV1Data: an invalid instance of the event.
    )pbdoc" );

    class_< ActiveMarkersBatteryStateV1Item >( atracModule,
                                               "ActiveMarkersBatteryStateV1Item",
                                               module_local(),
                                               R"pbdoc(
      This class contains the data of one battery status reading.
    )pbdoc" )
    .def_property_readonly( "image_count",
                            &ActiveMarkersBatteryStateV1Item::imageCount,
                            R"pbdoc(
        int: Frame counter at the time of the reading. Can be linked to the header data
        :func:`~atracsys.HeaderData.counter` field retrieved
        through :func:`~atracsys.TrackingSystem.getLastFrame`.
    )pbdoc" )
    .def_property_readonly( "device_id",
                            &ActiveMarkersBatteryStateV1Item::deviceID,
                            R"pbdoc(
        int: The short ID of the marker. Can be mapped to the option :func:`~atracsys.TrackingSystem.get_data_option` with
        option name "Active Wireless Markers info"
    )pbdoc" )
    .def_property_readonly( "battery_state",
                            &ActiveMarkersBatteryStateV1Item::batteryState,
                            R"pbdoc(
        int: The battery state. Can be converted to voltage by multiplying it with 18.5e-3f.
    )pbdoc" );

    typedef std::vector< ActiveMarkersBatteryStateV1Item >
        ActMrkBatteryStateContainer;

    class_< EvtActiveMarkersBatteryStateV1Data >( atracModule, "EvtActiveMarkersBatteryStateV1Data",
                                                  module_local(),
                                                  R"pbdoc(
      This class encapsulate an active marker battery event.
    )pbdoc" )
      .def_property_readonly( "states", &EvtActiveMarkersBatteryStateV1Data::states,
                              return_value_policy::reference,
                              R"pbdoc(
        list[ActiveMarkersBatteryStateV1Item]: The list of active marker  battery states (one item per marker)
    )pbdoc" )
      .def_property_readonly( "valid", &EvtActiveMarkersBatteryStateV1Data::valid,
                              R"pbdoc(
        bool: True if the event is valid, False otherwise.
    )pbdoc" )
      .def_static( "invalid_instance", &EvtActiveMarkersBatteryStateV1Data::invalidInstance,
                   return_value_policy::reference,
                   R"pbdoc(
      Return an invalid instance of the event (for internal use).

      Returns:
        EvtActiveMarkersBatteryStateV1Data: an invalid instance of the event.
    )pbdoc" );

    class_<ftkTimestampPTP>(atracModule, "TimestampPTP", module_local(), R"pbdoc(
        This class holds a PTP timestamp.
)pbdoc")
        .def_property_readonly(
            "seconds",
            [](const ftkTimestampPTP& item) { return item.Seconds; },
            R"pbdoc(Seconds part of the PTP timestamp.)pbdoc")
        .def_property_readonly(
            "nano_seconds",
            [](const ftkTimestampPTP& item) { return item.NanoSeconds; },
                              R"pbdoc(Nanoseconds part of the PTP timestamp.)pbdoc" );

    class_<ftkTimestampCorrectionPTP>(atracModule, "TimestampCorrectionPTP", module_local(), R"pbdoc(
        This class holds a PTP correction.
)pbdoc")
        .def_property_readonly(
            "seconds",
            [](const ftkTimestampCorrectionPTP& item) { return item.Seconds; },
            R"pbdoc(Seconds part of the PTP correction.)pbdoc")
        .def_property_readonly(
            "nano_seconds",
            [](const ftkTimestampCorrectionPTP& item) { return item.NanoSeconds; },
                              R"pbdoc(Nanoseconds part of the PTP correction.)pbdoc" );

    class_<ftkParentId>(atracModule, "ParentId", module_local(), R"pbdoc(
        This class holds a PTP clock master ID.
)pbdoc")
        .def_property_readonly(
            "clock_id",
            [](const ftkParentId& item) { return item.ClockId; },
            R"pbdoc(ID of the master clock.)pbdoc")
        .def_property_readonly(
            "source_port_id",
            [](const ftkParentId& item) { return item.SourcePortId; },
                              R"pbdoc(ID of the master source port.)pbdoc" );

    class_< EvtSynchronisationPTPV1Data >( atracModule, "EvtSynchronisationPTPV1Data", module_local(),
                                           R"pbdoc(
        This class provides access to the PTP synchronisation event.
)pbdoc" )
      .def_property_readonly("timestamp", &EvtSynchronisationPTPV1Data::timestamp, return_value_policy::reference,
                              R"pbdoc(
        TimestampPTP: PTP timestamp
)pbdoc")
      .def_property_readonly("last_correction", &EvtSynchronisationPTPV1Data::lastCorrection, return_value_policy::reference,
                              R"pbdoc(
        TimestampCorrectionPTP: last applied PTP correction.
)pbdoc")
      .def_property_readonly("parent_id", &EvtSynchronisationPTPV1Data::parentId, return_value_policy::reference,
                              R"pbdoc(
        ParentId: ID of the parent
)pbdoc")
      .def_property_readonly("status", &EvtSynchronisationPTPV1Data::status, return_value_policy::reference,
                              R"pbdoc(
        PortStatePTP: internal state of the PTP module state machine.
)pbdoc")
      .def_property_readonly("ptp_error", &EvtSynchronisationPTPV1Data::errorId, return_value_policy::reference,
                              R"pbdoc(
        ErrorPTP: status code of the PTP protocol.
)pbdoc")
      .def_static( "invalid_instance",
                   EvtSynchronisationPTPV1Data::invalidInstance,
                   return_value_policy::reference,
                   R"pbdoc(
      Return an invalid instance of the the event (internal use only).

      Returns:
        EvtSynchronisationPTPV1Data: an invalid instance of the event.
    )pbdoc" );

    class_< EventData >( atracModule, "EventData", module_local(),
                         R"pbdoc(
      This class encapsulate one event returned by the SDK in :func:`~atracsys.FrameData.events`.

      It can be used in a for loop to parse all the events like this:

      .. code-block:: python

            for event in frame.events:
              if event.valid:
                  if event.type == atracsys.EventType.ActiveMarkersMaskV1:
                      print("Active marker mask: {0}".format(event.activeMarkerMaskV1().active_markers_mask))
                  elif event.type == atracsys.EventType.FansV1:
                      print("Fans V1: Enabled: {0}, Fan 0 PWN Enabled: {1}, Fan 1 PWN Enabled: {2}, Fan 0 speed valid: {3}, Fan 1 speed valid: {4},".format(
                          event.fans_v1().fan_status.fans_enabled,
                          event.fans_v1().fan_status.fan0_pwm_enabled,
                          event.fans_v1().fan_status.fan1_pwm_enabled,
                          event.fans_v1().fan_status.fan0_speed_enabled,
                          event.fans_v1().fan_status.fan1_speed_enabled))
                      for item in event.fans_v1().fans:
                          print("\t {0} % {1} rmp".format(item.pwm_duty, item.speed))
                  elif event.type == atracsys.EventType.TempV4:
                      print("Temp V4: {0} sensors".format(len(event.temperature_v4().sensors)))
                      for item in event.temperature_v4().sensors:
                          print("\t {0}:{1}".format(hex(item.sensor_id), item.sensor_value))

    )pbdoc" )
      .def_property_readonly( "type", &EventData::type,
                              R"pbdoc(
        EventType: the type of the event. This is usually used to dermine which of the casting method to use.
    )pbdoc" )
      .def_property_readonly( "valid", &EventData::valid,
                              R"pbdoc(
        bool: True if event is valid, false otherwise.
    )pbdoc" )
      .def( "fans_v1", &EventData::fansV1, return_value_policy::reference,
            R"pbdoc(
      Return a copy of the event casted as an :class:`~atracsys.EvtFansV1Data` object.

      Returns:
        EvtFansV1Data: The event casted as a fan event.
    )pbdoc" )
      .def( "temperature_v4", &EventData::temperatureV4, return_value_policy::reference,
            R"pbdoc(
      Return a copy of the event casted as an :class:`~atracsys.EvtTemperatureV4Data` object.

      Returns:
        EvtTemperatureV4Data: The event casted as a temperature event.
    )pbdoc" )
      .def( "active_marker_mask_v1", &EventData::activeMarkerMaskV1, return_value_policy::reference,
            R"pbdoc(
      Return a copy of the event casted as an :class:`~atracsys.EvtActiveMarkersMaskV1Data` object.

      Returns:
        EvtActiveMarkersMaskV1Data: The event casted as an active marker mask event.
    )pbdoc" )
      .def( "active_markers_button_statuses_v1", &EventData::activeMarkersButtonStatusesV1,
            return_value_policy::reference,
            R"pbdoc(
      Return a copy of the event casted as an :class:`~atracsys.EvtActiveMarkersButtonStatusesV1Data` object.

      Returns:
        EvtActiveMarkersButtonStatusesV1Data: The event casted as an active marker button event.
    )pbdoc" )
      .def( "active_marker_battery_state_v1", &EventData::activeMarkerBatteryStateV1,
            return_value_policy::reference,
            R"pbdoc(
      Return a copy of the event casted as an :class:`~atracsys.EvtActiveMarkersBatteryStateV1Data` object.

      Returns:
        EvtActiveMarkersBatteryStateV1Data: The event casted as an active marker battery event.
    )pbdoc" )
      .def( "synchronisation_PTP_v1", &EventData::synchronisationPTPV1, return_value_policy::reference,
            R"pbdoc(
    Return a copy of the event casted as an :class:`atracsys.EvtSynchronisationPTPV1Data` object.

    Returns:
      EvtSynchronisationPTPV1Data: The event casted as a PTP synchronisation event.
)pbdoc" );

    class_< PictureData >( atracModule,
                           "PictureData",
                           module_local(),
                           pybind11::buffer_protocol(),
                           R"pbdoc(
      This class encapsulate the raw image pixels. It can be used like this:

      .. code-block:: python

            import atracsys.stk as tracker
            import numpy as np
            from PIL import Image

            # Initialisation is left to the reader...

            tracking_system.get_last_frame(100, frame)
            im = Image.fromarray(np.array(frame.left_picture()))
            im.show()

    )pbdoc" )
    .def_buffer( [ ]( PictureData& picture ) -> pybind11::buffer_info {
        if ( ftkPixelFormat::GRAY16 == picture.format() ||
             ftkPixelFormat::GRAY16_VIS == picture.format() )
        {
            return pybind11::buffer_info(
                // Pointer to buffer
                picture.getPixels8bits(),
                // Size of one scalar
                sizeof( uint16_t ),
                // Python struct-style format descriptor
                pybind11::format_descriptor< uint16_t >::format(),
                // Number of dimensions
                2,
                // Buffer dimensions
                { picture.height(), picture.width() },
                // Strides (in bytes) for each index
            {
                sizeof( uint16_t ) * picture.width(),
                sizeof( uint16_t )
            }
                );
        }
        else
        {
            return pybind11::buffer_info(
                // Pointer to buffer
                picture.getPixels8bits(),
                // Size of one scalar
                sizeof( uint8_t ),
                // Python struct-style format descriptor
                pybind11::format_descriptor< uint8_t >::format(),
                // Number of dimensions
                2,
                // Buffer dimensions
                { picture.height(), picture.width() },
                // Strides (in bytes) for each index
            {
                sizeof( uint8_t ) * picture.width(),
                sizeof( uint8_t )
            }
                );
        }
    } )
    .def_property_readonly( "width",
                            &PictureData::width,
                            R"pbdoc(
        int: The width of the image.
      )pbdoc" )
    .def_property_readonly( "index",
                            &PictureData::height,
                            R"pbdoc(
        int: The height of the image.
      )pbdoc" );

    class_< FrameData >( atracModule,
                         "FrameData",
                         module_local(),
                         R"pbdoc(
      This class encapsulate all the data received when calling :meth:`~atracsys.TrackingSystem.get_last_frame`

      .. warning::

            The order of elements in the :attr:`~atracsys.FrameData.left_raws`,
            :attr:`~atracsys.FrameData.right_raws`,
            :attr:`~atracsys.FrameData.fiducials`,
            :attr:`~atracsys.FrameData.markers` and
            :attr:`~atracsys.FrameData.events` containers is *not* stable, i.e.
            it is *not* guaranteed. No application should rely on the order of
            the element in those containers.

    )pbdoc" )
    .def( init<>() )
    .def_property_readonly( "header",
                            &FrameData::header,
                            return_value_policy::reference,
                            R"pbdoc(
        HeaderData: The current image information.
    )pbdoc" )
    .def_property_readonly( "left_raws",
                            &FrameData::leftRaws,
                            return_value_policy::reference,
                            R"pbdoc(
        list[RawData]: The raw data from the left image.
    )pbdoc" )
    .def_property_readonly( "right_raws",
                            &FrameData::rightRaws,
                            return_value_policy::reference,
                            R"pbdoc(
        list[RawData]: The raw data from the right image.
    )pbdoc" )
    .def_property_readonly( "fiducials",
                            &FrameData::fiducials,
                            return_value_policy::reference,
                            R"pbdoc(
        list[FiducialData]: The list of 3D fiducials.
    )pbdoc" )
    .def_property_readonly( "markers",
                            &FrameData::markers,
                            return_value_policy::reference,
                            R"pbdoc(
        list[MarkerData]: The list of markers.
    )pbdoc" )
    .def_property_readonly( "events",
                            &FrameData::events,
                            return_value_policy::reference,
                            R"pbdoc(
        list[EventData]: The list of events.

        See :class:`~atracsys.EventData` for more information on how to parse this list.
    )pbdoc" )
    .def( "left_picture",
          &FrameData::leftPicture,
          return_value_policy::reference,
          R"pbdoc(
      PictureData: The left image (pixels).

      See :class:`~atracsys.PictureData` for more information on how to parse the image.
    )pbdoc" )
    .def( "right_picture",
          &FrameData::rightPicture,
          return_value_policy::reference,
          R"pbdoc(
      PictureData: The right image (pixels).

      See :class:`~atracsys.PictureData` for more information on how to parse the image.
    )pbdoc" );

    class_< WrappedTrackingSystem >( atracModule,
                                     "TrackingSystem",
                                     module_local(),
                                     R"pbdoc(
      This class is the main entry point of the Atracsys SDK.
)pbdoc" )
    .def( init< uint32, bool >() )
    .def( init< uint32 >() )
    .def( init<>() )
    .def( "initialise",
          &WrappedTrackingSystem::initialise,
          arg( "fileName" ) = std::string(
              "" ),
          R"pbdoc(
      Initialise the SDK. This method should be called before any other.

      Parameters:
        fileName: Optional, the path to a json file containing an Atracsys environment.

      Returns:
        Status: atracsys::Status of the initialisation.
    )pbdoc" )
    .def( "create_frame",
          &WrappedTrackingSystem::createFrame,
          arg( "pixels" ) = bool( false ),
          arg( "eventCount" ) = uint32_t( 0u ),
          arg( "rawDataCount" ) = uint32_t( 0u ),
          arg( "fiducialsCount" ) = uint32_t( 0u ),
          arg( "markerCount" ) = uint32_t(
              0u ),
          R"pbdoc(
      Method creating a frame for 1 detected devices. This has to be called
      before :func:`~atracsys.TrackingSystem.get_last_frame`.

      A double call to this function will \e not create two instances per
      connected device, but it allows to change the settings (e.g.
      increasing the number of retrieved 3D fiducials or disabling the
      picture retrieving).


      Parameters:
        pixels: True if the pixels must be retrieved.
        eventCount: Maximum number of events retrieved in a frame.
        rawDataCount: maximum number of raw data retrieved in a frame.
        fiducialCount: maximum number of fiducials retrieved in a frame.
        markerCount: maximum number of markers retrieved in a frame.

      Returns:
        Status: atracsys::Status of the creation of the frame.
    )pbdoc" )
    .def( "create_frames",
          &WrappedTrackingSystem::createFrames,
          arg( "pixels" ) = bool( false ),
          arg( "eventCount" ) = uint32_t( 0u ),
          arg( "rawDataCount" ) = uint32_t( 0u ),
          arg( "fiducialsCount" ) = uint32_t( 0u ),
          arg( "markerCount" ) = uint32_t(
              0u ),
          R"pbdoc(
      Method creating a for multiple detected devices. This has to be called
      before :func:`~atracsys.TrackingSystem.get_last_frames`.

      A double call to this function will \e not create two instances per
      connected device, but it allows to change the settings (e.g.
      increasing the number of retrieved 3D fiducials or disabling the
      picture retrieving).


      Parameters:
        pixels: True if the pixels must be retrieved.
        eventCount: Maximum number of events retrieved in a frame.
        rawDataCount: maximum number of raw data retrieved in a frame.
        fiducialCount: maximum number of fiducials retrieved in a frame.
        markerCount: maximum number of markers retrieved in a frame.

      Returns:
        Status: atracsys::Status of the creation of the frame.
    )pbdoc" )
    .def( "get_last_frame_for_device",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint64, uint32, FrameData& ) const >
          ( &WrappedTrackingSystem::getLastFrameForDevice ),
          R"pbdoc(
      Get the last retrieved frame for a given device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Timeout to wait before returning.

      Returns:
        Status: atracsys::Status of the call.
    )pbdoc" )
    .def( "get_last_frame_for_device",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint64, FrameData& ) const >(
              &WrappedTrackingSystem::getLastFrameForDevice ),
          R"pbdoc(
      Get the last retrieved frame for a given device using the default timeout.

      Parameters:
        arg0: Serial number (int) of the device.

      Returns:
        Status: atracsys::Status of the call.
    )pbdoc" )
    .def( "get_last_frame",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint32, FrameData& ) const >(
              &WrappedTrackingSystem::getLastFrame ),
          R"pbdoc(
      Get the last retrieved frame for the only connected device.

      Parameters:
        arg0: Timeout to wait before returning.

      Returns:
        Status: atracsys::Status of the call.
    )pbdoc" )
    .def( "get_last_frame",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( FrameData& ) const >(
              &WrappedTrackingSystem::getLastFrame ),
          R"pbdoc(
      Get the last retrieved frame for the only connected device.

      Returns:
        Status: atracsys::Status of the call.
    )pbdoc" )
    .def( "get_last_frames",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint32, std::vector< FrameData >& ) const >(
              &WrappedTrackingSystem::getLastFrames ),
          R"pbdoc(
      Get the last retrieved frames for multiple connected device.

      Parameters:
        arg0: Timeout to wait before returning.

      Returns:
        Status: atracsys::Status of the call.
    )pbdoc" )
    .def( "get_last_frames",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( std::vector< FrameData >& ) const >(
              &WrappedTrackingSystem::getLastFrames ),
          R"pbdoc(
      Get the last retrieved frames for multiple connected device.

      Returns:
        Status: atracsys::Status of the call.
    )pbdoc" )
    .def( "get_last_error",
          &WrappedTrackingSystem::getLastError,
          R"pbdoc(
      Get a dictionnary of the last errors returned by the SDK.

      .. code-block:: python

        result = tracking_system.get_last_error()
        if result[0] == atracsys.Status.Ok:
          for level in ['errors', 'warnings', 'messages']:
              if level in result[1]:
                  print(result[1][level])


      Returns:
        tuple(Status,dict): Tuple containing the status of the call and a dictionary holding error messages.
    )pbdoc" )
    .def( "stream_last_error",
          &WrappedTrackingSystem::streamLastError,
          R"pbdoc(
      Print the last errors on the standard output
    )pbdoc" )
    .def( "enumerate_devices",
          &WrappedTrackingSystem::enumerateDevices,
          R"pbdoc(
      This function scan all the connected devices.

      Returns:
        Status: atracsys::Status of the call.
    )pbdoc" )
    .def( "get_enumerated_devices",
          &WrappedTrackingSystem::getEnumeratedDevices,
          R"pbdoc(
      This function returns all devices detected during a call to :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Returns:
        tuple(Status,list[DeviceInfo]): A tuple containing the status code and the list of enumerated devices.
    )pbdoc" )
    .def( "get_enumerated_options",
          static_cast< std::tuple< atracsys::Status,
                                   std::vector< DeviceOption > >(
                           WrappedTrackingSystem::* )(
                           uint64 ) const >
          ( &WrappedTrackingSystem::getEnumeratedOptions ),
          R"pbdoc(
      This function returns all the options of a specific device detected during a call to :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Parameters:
        arg0: Serial number (int) of the device.

      Returns:
        tuple(Status,list[DeviceOption]): A tuple containing the status code and the list of enumerated options.
    )pbdoc" )
    .def( "get_enumerated_options",
          static_cast< std::tuple< atracsys::Status,
                                   std::vector< DeviceOption > >(
                           WrappedTrackingSystem::* )() const >
          ( &WrappedTrackingSystem::getEnumeratedOptions ),
          R"pbdoc(
      This function returns all the options of the only device detected during a call to :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Returns:
        tuple(Status,list[DeviceOption]): A tuple containing the status code and the list of enumerated options.
    )pbdoc" )
    .def( "get_int_option",
          static_cast< std::tuple< atracsys::Status,
                                   int32 >( WrappedTrackingSystem::* )
                           ( uint64, uint32,
                           ftkOptionGetter ) const >
          ( &WrappedTrackingSystem::getIntOption ),
          R"pbdoc(
      This functions allows to read an integer option from its integer unique
      ID, for the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Option unique ID.
        arg2: What selector for min / max / default / current value.

      Returns:
        tuple(Status,int): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_int_option",
          static_cast< std::tuple< atracsys::Status,
                                   int32 >( WrappedTrackingSystem::* )
                           ( uint64,
                           uint32 ) const >
          ( &WrappedTrackingSystem::getIntOption ),
          R"pbdoc(
      This functions allows to read an integer option from its integer unique
      ID, for the wanted device. It returns the current value.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Option unique ID.

      Returns:
        tuple(Status,int): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_int_option",
          static_cast< std::tuple< atracsys::Status,
                                   int32 >( WrappedTrackingSystem::* )
                           ( uint32,
                           ftkOptionGetter ) const >
          ( &WrappedTrackingSystem::getIntOption ),
          R"pbdoc(
      This functions allows to read an integer option from its integer unique
      ID, for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`..

      Parameters:
        arg0: Option unique ID.
        arg1: What selector for min / max / default / current value.

      Returns:
        tuple(Status,int): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_int_option",
          static_cast< std::tuple< atracsys::Status,
                                   int32 >( WrappedTrackingSystem::* )
                       (
                           uint32 ) const >
          ( &WrappedTrackingSystem::getIntOption ),
          R"pbdoc(
      This functions allows to read an integer option from its integer unique
      ID, for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`.
      It returns the current value.

      Parameters:
        arg0: Option unique ID.

      Returns:
        tuple(Status,int): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_int_option",
          static_cast< std::tuple< atracsys::Status,
                                   int32 >( WrappedTrackingSystem::* )
                           ( uint64, const std::string&,
                           ftkOptionGetter ) const >
          ( &WrappedTrackingSystem::getIntOption ),
          R"pbdoc(
      This functions allows to read an integer option from its name
      , for the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Name of the option.
        arg2: What selector for min / max / default / current value.

      Returns:
        tuple(Status,int): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_int_option",
          static_cast< std::tuple< atracsys::Status,
                                   int32 >( WrappedTrackingSystem::* )
                           ( uint64,
                           const std::string& ) const >
          ( &WrappedTrackingSystem::getIntOption ),
          R"pbdoc(
      This functions allows to read an integer option from its name
      , for the wanted device. It returns the current value.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Name of the option.

      Returns:
        tuple(Status,int): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_int_option",
          static_cast< std::tuple< atracsys::Status,
                                   int32 >( WrappedTrackingSystem::* )
                           ( const std::string&,
                           ftkOptionGetter ) const >
          ( &WrappedTrackingSystem::getIntOption ),
          R"pbdoc(
      This functions allows to read an integer option from its name
      , for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Parameters:
        arg0: Name of the option.
        arg1: What selector for min / max / default / current value.

      Returns:
        tuple(Status,int): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_int_option",
          static_cast< std::tuple< atracsys::Status,
                                   int32 >( WrappedTrackingSystem::* )
                       (
                           const std::string& ) const >
          ( &WrappedTrackingSystem::getIntOption ),
          R"pbdoc(
      This functions allows to read an integer option from its name
      , for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`.
      It returns the current value.

      Parameters:
        arg0: Name of the option.

      Returns:
        tuple(Status,int): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_float_option",
          static_cast< std::tuple< atracsys::Status,
                                   float >( WrappedTrackingSystem::* )
                           ( uint64, uint32,
                           ftkOptionGetter ) const >
          ( &WrappedTrackingSystem::getFloatOption ),
          R"pbdoc(
      This functions allows to read a float option from its integer unique
      ID, for the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Option unique ID.
        arg2: What selector for min / max / default / current value.

      Returns:
        tuple(Status,float): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_float_option",
          static_cast< std::tuple< atracsys::Status,
                                   float >( WrappedTrackingSystem::* )
                           ( uint64,
                           uint32 ) const >
          ( &WrappedTrackingSystem::getFloatOption ),
          R"pbdoc(
      This functions allows to read a float option from its integer unique
      ID, for the wanted device. It returns the current value.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Option unique ID.

      Returns:
        tuple(Status,float): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_float_option",
          static_cast< std::tuple< atracsys::Status,
                                   float >( WrappedTrackingSystem::* )
                           ( uint32,
                           ftkOptionGetter ) const >
          ( &WrappedTrackingSystem::getFloatOption ),
          R"pbdoc(
      This functions allows to read a float option from its integer unique
      ID, for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`..

      Parameters:
        arg0: Option unique ID.
        arg1: What selector for min / max / default / current value.

      Returns:
        tuple(Status,float): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_float_option",
          static_cast< std::tuple< atracsys::Status,
                                   float >( WrappedTrackingSystem::* )
                       (
                           uint32 ) const >
          ( &WrappedTrackingSystem::getFloatOption ),
          R"pbdoc(
      This functions allows to read a float option from its integer unique
      ID, for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`.
      It returns the current value.

      Parameters:
        arg0: Option unique ID.

      Returns:
        tuple(Status,float): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_float_option",
          static_cast< std::tuple< atracsys::Status,
                                   float >( WrappedTrackingSystem::* )
                           ( uint64, const std::string&,
                           ftkOptionGetter ) const >
          ( &WrappedTrackingSystem::getFloatOption ),
          R"pbdoc(
      This functions allows to read a float option from its name
      , for the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Name of the option.
        arg2: What selector for min / max / default / current value.

      Returns:
        tuple(Status,float): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_float_option",
          static_cast< std::tuple< atracsys::Status,
                                   float >( WrappedTrackingSystem::* )
                           ( uint64,
                           const std::string& ) const >
          ( &WrappedTrackingSystem::getFloatOption ),
          R"pbdoc(
      This functions allows to read a float option from its name
      , for the wanted device. It returns the current value.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Name of the option.

      Returns:
        tuple(Status,float): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_float_option",
          static_cast< std::tuple< atracsys::Status,
                                   float >( WrappedTrackingSystem::* )
                           ( const std::string&,
                           ftkOptionGetter ) const >
          ( &WrappedTrackingSystem::getFloatOption ),
          R"pbdoc(
      This functions allows to read a float option from its name
      , for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Parameters:
        arg0: Name of the option.
        arg1: What selector for min / max / default / current value.

      Returns:
        tuple(Status,float): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_float_option",
          static_cast< std::tuple< atracsys::Status,
                                   float >( WrappedTrackingSystem::* )
                       (
                           const std::string& ) const >
          ( &WrappedTrackingSystem::getFloatOption ),
          R"pbdoc(
      This functions allows to read a float option from its name
      , for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`.
      It returns the current value.

      Parameters:
        arg0: Name of the option.

      Returns:
        tuple(Status,float): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_data_option",
          static_cast< std::tuple< atracsys::Status,
                                   std::string >( WrappedTrackingSystem::* )
                           ( uint64,
                           uint32 ) const >
          ( &WrappedTrackingSystem::getDataOption ),
          R"pbdoc(
      This functions allows to read a data option from its integer unique
      ID, for the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Option unique ID.

      Returns:
        tuple(Status,str): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_data_option",
          static_cast< std::tuple< atracsys::Status,
                                   std::string >( WrappedTrackingSystem::* )
                       (
                           uint32 ) const >
          ( &WrappedTrackingSystem::getDataOption ),
          R"pbdoc(
      This functions allows to read a data option from its integer unique
      ID, for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Parameters:
        arg0: Option unique ID.

      Returns:
        tuple(Status,str): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_data_option",
          static_cast< std::tuple< atracsys::Status,
                                   std::string >( WrappedTrackingSystem::* )
                           ( uint64,
                           const std::string& ) const >
          ( &WrappedTrackingSystem::getDataOption ),
          R"pbdoc(
      This functions allows to read a data option from its name
      , for the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Name of the option.

      Returns:
        tuple(Status,str): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "get_data_option",
          static_cast< std::tuple< atracsys::Status,
                                   std::string >( WrappedTrackingSystem::* )
                       (
                           const std::string& ) const >
          ( &WrappedTrackingSystem::getDataOption ),
          R"pbdoc(
      This functions allows to read an data option from its name
      , for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Parameters:
        arg0: Name of the option.

      Returns:
        tuple(Status,str): A tuple containing the status code and the value of the option
    )pbdoc" )
    .def( "set_int_option",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint64, uint32, int32 ) const >(
              &WrappedTrackingSystem::setIntOption ),
          R"pbdoc(
      This functions allows to set an integer option from its integer unique
      ID, for the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Option unique ID.
        arg2: Option new value.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "set_int_option",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint32, int32 ) const >(
              &WrappedTrackingSystem::setIntOption ),
          R"pbdoc(
      This functions allows to set an integer option from its integer unique
      ID, for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Parameters:
        arg0: Option unique ID.
        arg1: Option new value.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "set_int_option",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint64, const std::string&, int32 ) const >(
              &WrappedTrackingSystem::setIntOption ),
          R"pbdoc(
      This functions allows to set an integer option from its name
      ID, for the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Name of the option.
        arg2: Option new value.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "set_int_option",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( const std::string&, int32 ) const >(
              &WrappedTrackingSystem::setIntOption ),
          R"pbdoc(
      This functions allows to set an integer option from its name
      ID, for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Parameters:
        arg0: Name of the option.
        arg1: Option new value.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "set_float_option",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint64, uint32, float ) const >(
              &WrappedTrackingSystem::setFloatOption ),
          R"pbdoc(
      This functions allows to set a float option from its integer unique
      ID, for the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Option unique ID.
        arg2: Option new value.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "set_float_option",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint32, float ) const >(
              &WrappedTrackingSystem::setFloatOption ),
          R"pbdoc(
      This functions allows to set a float option from its integer unique
      ID, for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Parameters:
        arg0: Option unique ID.
        arg1: Option new value.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "set_float_option",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint64, const std::string&, float ) const >(
              &WrappedTrackingSystem::setFloatOption ),
          R"pbdoc(
      This functions allows to set a float option from its name
      ID, for the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Name of the option.
        arg2: Option new value.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "set_float_option",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( const std::string&, float ) const >(
              &WrappedTrackingSystem::setFloatOption ),
          R"pbdoc(
      This functions allows to set an float option from its name
      ID, for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Parameters:
        arg0: Name of the option.
        arg1: Option new value.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "set_data_option",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint64, uint32, const std::string& ) const >(
              &WrappedTrackingSystem::setDataOption ),
          R"pbdoc(
      This functions allows to set a data option from its integer unique
      ID, for the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Option unique ID.
        arg2: Option new value.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "set_data_option",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint32, const std::string& ) const >(
              &WrappedTrackingSystem::setDataOption ),
          R"pbdoc(
      This functions allows to set a data option from its integer unique
      ID, for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Parameters:
        arg0: Option unique ID.
        arg1: Option new value.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "set_data_option",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint64, const std::string&, const std::string& ) const >(
              &WrappedTrackingSystem::setDataOption ),
          R"pbdoc(
      This functions allows to set a data option from its name
      ID, for the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Name of the option.
        arg2: Option new value.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "set_data_option",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( const std::string&, const std::string& ) const >(
              &WrappedTrackingSystem::setDataOption ),
          R"pbdoc(
      This functions allows to set a data option from its name
      ID, for the only device detected during a call to
      :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Parameters:
        arg0: Name of the option.
        arg1: Option new value.

      Returns:
        Status: The result of the call.
    )pbdoc" )
#ifdef BUILD_WITH_PRIVATE_SDK
      .def( "get_enumerated_registers",
            static_cast< std::tuple< atracsys::Status,
                                     std::vector< DeviceRegister > >(
              WrappedTrackingSystem::* )(
              uint64 ) const >
            ( &WrappedTrackingSystem::getEnumeratedRegisters ),
            R"pbdoc(
      This function returns all the options of a specific device detected during a call to :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Parameters:
        arg0: Serial number (int) of the device.

      Returns:
        tuple(Status,list[DeviceOption]): A tuple containing the status code and the list of enumerated options.
    )pbdoc" )
      .def( "get_enumerated_registers",
            static_cast< std::tuple< atracsys::Status,
                                     std::vector< DeviceRegister > >(
              WrappedTrackingSystem::* )() const >
            ( &WrappedTrackingSystem::getEnumeratedRegisters ),
            R"pbdoc(
      This function returns all the options of the only device detected during a call to :func:`~atracsys.TrackingSystem.enumerate_devices`.

      Returns:
        tuple(Status,list[DeviceOption]): A tuple containing the status code and the list of enumerated options.
    )pbdoc" )
    .def( "get_register_value",
          static_cast< std::tuple< atracsys::Status,
                                   uint32 >( WrappedTrackingSystem::* )
                           ( uint64,
                           uint32 ) const >
          ( &WrappedTrackingSystem::getRegisterValue ),
          R"pbdoc(
      This functions allows to read a register from its address, for
          the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Address (int) of the register

      Returns:
        tuple(Status,nbr): A tuple containing the status code and the value of the register
    )pbdoc" )
    .def( "get_register_value",
          static_cast< std::tuple< atracsys::Status,
                                   uint32 >( WrappedTrackingSystem::* )
                       (
                           uint32 ) const >
          ( &WrappedTrackingSystem::getRegisterValue ),
          R"pbdoc(
      This functions allows to read a register from its address, for
          the connected device.

      Parameters:
        arg0: Address (int) of the register

      Returns:
        tuple(Status,nbr): A tuple containing the status code and the value of the register
    )pbdoc" )
    .def( "get_register_value",
          static_cast< std::tuple< atracsys::Status,
                                   uint32 >( WrappedTrackingSystem::* )
                           ( uint64,
                           const std::string& ) const >
          ( &WrappedTrackingSystem::getRegisterValue ),
          R"pbdoc(
      This functions allows to read a register from its address, for
          the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Name (str) of the register

      Returns:
        tuple(Status,nbr): A tuple containing the status code and the value of the register
    )pbdoc" )
    .def( "get_register_value",
          static_cast< std::tuple< atracsys::Status,
                                   uint32 >( WrappedTrackingSystem::* )
                       (
                           const std::string& ) const >
          ( &WrappedTrackingSystem::getRegisterValue ),
          R"pbdoc(
      This functions allows to read a register from its address, for
          the connected device.

      Parameters:
        arg0: Name (str) of the register

      Returns:
        tuple(Status,nbr): A tuple containing the status code and the value of the register
    )pbdoc" )
    .def( "set_register_value",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint64, uint32, uint32 ) const >
          ( &WrappedTrackingSystem::setRegisterValue ),
          R"pbdoc(
      This functions allows to write a register from its address, for
          the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Address (int) of the register
        arg2: Value (int) of the register

      Returns:
        Status: The status code
    )pbdoc" )
    .def( "set_register_value",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint32, uint32 ) const >
          ( &WrappedTrackingSystem::setRegisterValue ),
          R"pbdoc(
      This functions allows to write a register from its address, for
          the connected device.

      Parameters:
        arg0: Address (int) of the register
        arg1: Value (int) of the register

      Returns:
        Status: The status code
    )pbdoc" )
    .def( "set_register_value",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint64, const std::string&, uint32 ) const >
          ( &WrappedTrackingSystem::setRegisterValue ),
          R"pbdoc(
      This functions allows to read a register from its address, for
          the wanted device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: Name (str) of the register
        arg2: Value (int) of the register

      Returns:
        Status: The status code
    )pbdoc" )
    .def( "set_register_value",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( const std::string&, uint32 ) const >
          ( &WrappedTrackingSystem::setRegisterValue ),
          R"pbdoc(
      This functions allows to read a register from its address, for
          the connected device.

      Parameters:
        arg0: Name (str) of the register
        arg1: Value (int) of the register

      Returns:
        Status: The status code
    )pbdoc" )
#endif
    .def( "set_geometry",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint64,
                         const WrappedGeometry& ) >( &WrappedTrackingSystem::
                                                     setGeometry ),
          R"pbdoc(
      This functions allows to register a geometry for a given device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: The geometry (loaded by load_geometry).

      Returns:
        Status:The result of the call.
    )pbdoc" )
    .def( "set_geometry",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint64, const std::string& ) >
          ( &WrappedTrackingSystem::setGeometry ),
          R"pbdoc(
      This functions allows to register a geometry for a given device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: The geometry file path.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "set_geometry",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( const WrappedGeometry& ) >
          ( &WrappedTrackingSystem::setGeometry ),
          R"pbdoc(
      This functions allows to register a geometry for all detected devices.

      Parameters:
        arg0: The geometry (loaded by load_geometry).

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "set_geometry",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( const std::string& ) >
          ( &WrappedTrackingSystem::setGeometry ),
          R"pbdoc(
      This functions allows to register a geometry for all detected devices.

      Parameters:
        arg0: The geometry file path.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "unset_geometry",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint64,
                         uint32 ) >( &WrappedTrackingSystem::unsetGeometry ),
          R"pbdoc(
      This functions allows to unregister a geometry for a given device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: The geometry id.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "unset_geometry",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint64,
                         const std::string& ) >( &WrappedTrackingSystem::
                                                 unsetGeometry ),
          R"pbdoc(
      This functions allows to unregister a geometry for a given device.

      Parameters:
        arg0: Serial number (int) of the device.
        arg1: The geometry file path.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "unset_geometry",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( uint32 ) >( &WrappedTrackingSystem::unsetGeometry ),
          R"pbdoc(
      This functions allows to unregister a geometry for all devices.

      Parameters:
        arg0: The geometry id.

      Returns:
        Status: The result of the call.
    )pbdoc" )
    .def( "unset_geometry",
          static_cast< atracsys::Status( WrappedTrackingSystem::* )
                       ( const std::string& ) >( &WrappedTrackingSystem::
                                                 unsetGeometry ),
          R"pbdoc(
      This functions allows to unregister a geometry for all devices.

      Parameters:
        arg0: The geometry file path.

      Returns:
        Status: The result of the call.
    )pbdoc" )
      .def( "load_geometry", &WrappedTrackingSystem::loadRigidBody,
            R"pbdoc(
        This function allow to load a geometry from the given file name.

        Parameters:
            arg0: The path to the file to load
)pbdoc" )
      .def( "save_geometry", &WrappedTrackingSystem::saveRigidBody,
            R"pbdoc(
        This function allow to save a geometry to the given file name.

        Parameters:
            arg0: The path to the file to write
            arg1: The geometry instance to save
)pbdoc" )
      .def( "convert_geometryfile", &WrappedTrackingSystem::convertGeometryFile,
            R"pbdoc(
        This function allow to convert a geometry file from binary to INI or vice-versa.

        Parameters:
            arg0: The path to the input file
            arg1: The path to the output file
)pbdoc" )
      .def("get_accelerometer_data_for_device",
            static_cast< std::tuple< atracsys::Status, std::vector< WrappedPoint > >( WrappedTrackingSystem::* )
                         ( uint64_t ) >( &WrappedTrackingSystem::getAccelerometerDataForDevice ),
            R"pbdoc(
        This function allows to get the accelerometer reading(s) from the wanted device.

        Parameters:
            int: The serial number of the wanted device
)pbdoc")
      .def("get_accelerometer_data_for_device",
            static_cast< std::tuple< atracsys::Status, std::vector< WrappedPoint > >( WrappedTrackingSystem::* )() >( &WrappedTrackingSystem::getAccelerometerDataForDevice ),
            R"pbdoc(
        This function allows to get the accelerometer reading(s) from the connected device.

        Parameters:
            int: The serial number of the wanted device
)pbdoc")
      .def("get_accelerometer_data",
            static_cast< std::tuple< atracsys::Status, std::vector< std::vector< WrappedPoint > > >( WrappedTrackingSystem::* )() >( &WrappedTrackingSystem::getAccelerometerData ),
            R"pbdoc(
        This function allows to get the accelerometer reading(s) from all devices.

        Parameters:
            int: The serial number of the wanted device
)pbdoc")
      .def("get_real_time_clock_for_device",
            static_cast< std::tuple< atracsys::Status, atracsys::timestamp_t >( WrappedTrackingSystem::* )
                         ( uint64_t ) >( &WrappedTrackingSystem::getRealTimeClockForDevice ),
            R"pbdoc(
        This function allows to get the accelerometer reading(s) from the wanted device.

        Parameters:
            int: The serial number of the wanted device
)pbdoc")
      .def("get_real_time_clock_for_device",
            static_cast< std::tuple< atracsys::Status, atracsys::timestamp_t >( WrappedTrackingSystem::* ) () >( &WrappedTrackingSystem::getRealTimeClockForDevice ),
            R"pbdoc(
        This function allows to get the accelerometer reading(s) from the connected device.

        Parameters:
)pbdoc")
      .def("get_real_time_clock_for_device", &WrappedTrackingSystem::getRealTimeClock,
            R"pbdoc(
        This function allows to get the accelerometer reading(s) from all devices.

        Parameters:
)pbdoc")
    .def_property( "default_timeout",
                   &WrappedTrackingSystem::defaultTimeout,
                   &WrappedTrackingSystem::setDefaultTimeout,
                   "int: Default timeout used in get_last_frame(s) calls." )
    .def_property( "allow_simulator",
                   &WrappedTrackingSystem::allowSimulator,
                   &WrappedTrackingSystem::setAllowSimulator,
                   "bool: Allows the simulator to be enumerated" )
    .def_property( "level",
                   &WrappedTrackingSystem::level,
                   &WrappedTrackingSystem::setLevel,
                   "LogLevel: The verbosity level" );

#ifdef VERSION_INFO
    atracModule.attr( "__version__" ) = VERSION_INFO;
#else
    atracModulem.attr( "__version__" ) = "dev";
#endif
}
