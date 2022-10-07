#
# Atracsys SDK Python              # # This script compile the python wrapper  #
#

cmake_minimum_required(VERSION 3.11)

project(atracsys C CXX)

set(CMAKE_CXX_STANDARD 11)

option(BUILD_WITH_PRIVATE_SDK "Build with private bindings" OFF)

configure_file(${CMAKE_SOURCE_DIR}/CMakeLists.txt.pybind11
               pybind11-download/CMakeLists.txt)
if (WIN32)
  set(GENERATOR_PLATFORM_ARG "-DCMAKE_GENERATOR_PLATFORM=x64")
endif ()
execute_process(
  COMMAND "${CMAKE_COMMAND}" ${GENERATOR_PLATFORM_ARG} -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/pybind11-download")
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/pybind11-download")

# Disable the test during pybind build
set(PYBIND11_TEST OFF)
add_subdirectory(${CMAKE_BINARY_DIR}/pybind11/src/pybind11 build)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/../../../docs/index.rst.in"
"${CMAKE_CURRENT_SOURCE_DIR}/../../../docs/index.rst")

if (WIN32)
  # On Windows, we look for the installation folder in the registry
  if (TRACKER_TYPE STREQUAL "ftk")
    get_filename_component(
      ATRACSYS_SDK_INSTALL_PATH
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Atracsys\\fusionTrack;Root]" ABSOLUTE)
  elseif (TRACKER_TYPE STREQUAL "stk")
    get_filename_component(
      ATRACSYS_SDK_INSTALL_PATH
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Atracsys\\spryTrack;Root]" ABSOLUTE)
  endif ()
else ()
  # On Linux, we assume that the installation folder is in an environment
  if (TRACKER_TYPE STREQUAL "ftk")
    set(ATRACSYS_SDK_INSTALL_PATH $ENV{ATRACSYS_FTK_HOME})
  elseif (TRACKER_TYPE STREQUAL "stk")
    set(ATRACSYS_SDK_INSTALL_PATH $ENV{ATRACSYS_STK_HOME})
  endif ()
endif ()

if (MSVC)
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_WARNINGS /DNOMINMAX /DWIN32_LEAN_AND_MEAN"
  )
endif ()

# When installing though setup.py (for dev only), used the passed parameters if
# they exists.
if (NOT DEFINED ATRACSYS_SDK_LIB_PATH)
  set(ATRACSYS_SDK_LIB_PATH "${ATRACSYS_SDK_INSTALL_PATH}/lib")
endif ()

if (NOT DEFINED ATRACSYS_SDK_INCLUDE_PATH)
  set(ATRACSYS_SDK_INCLUDE_PATH "${ATRACSYS_SDK_INSTALL_PATH}/include")
endif ()

if (NOT DEFINED ATRACSYS_CPP_SDK_PATH)
  set(ATRACSYS_CPP_SDK_HEADERS "${ATRACSYS_SDK_INSTALL_PATH}/advancedAPI")
  set(ATRACSYS_CPP_SDK_SOURCES "${ATRACSYS_SDK_INSTALL_PATH}/advancedAPI")
else ()
  set(ATRACSYS_CPP_SDK_HEADERS "${ATRACSYS_CPP_SDK_PATH}/include")
  set(ATRACSYS_CPP_SDK_SOURCES "${ATRACSYS_CPP_SDK_PATH}/src")
endif ()

link_directories("${ATRACSYS_SDK_LIB_PATH}")

set(advanced_api_SRCS
    "${ATRACSYS_CPP_SDK_HEADERS}/AdditionalDataStructures.hpp"
    "${ATRACSYS_CPP_SDK_SOURCES}/AdditionalDataStructures.cpp"
    "${ATRACSYS_CPP_SDK_HEADERS}/TrackingSystem.hpp"
    "${ATRACSYS_CPP_SDK_SOURCES}/TrackingSystem.cpp"
    "${ATRACSYS_CPP_SDK_HEADERS}/TrackingSystemAbstract.hpp"
    "${ATRACSYS_CPP_SDK_SOURCES}/TrackingSystemAbstract.cpp")

if (BUILD_WITH_PRIVATE_SDK)
  message(INFO "internal build - PRIVATE")
  set(advanced_api_SRCS
      ${advanced_api_SRCS}
      "${ATRACSYS_CPP_SDK_HEADERS}/TrackingSystemPrivate.hpp"
      "${ATRACSYS_CPP_SDK_SOURCES}/TrackingSystemPrivate.cpp")
endif (BUILD_WITH_PRIVATE_SDK)

set(module_SRCS
    main.cpp
    WrappedFiducial.cpp
    WrappedFiducial.hpp
    WrappedGeometry.cpp
    WrappedGeometry.hpp
    WrappedPoint.cpp
    WrappedPoint.hpp
    WrappedTrackingSystem.cpp
    WrappedTrackingSystem.hpp
    ${advanced_api_SRCS})

if ("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
  set(ARCH 64)
else ("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
  set(ARCH 32)
endif ("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
set(module_LIBS ${module_LIBS} fusionTrack${ARCH})

set(module_NAME _atracsys_${TRACKER_TYPE})

pybind11_add_module(${module_NAME} ${module_SRCS})

if (WIN32)
  set(module_LIBS ${module_LIBS} Winmm)
  if (MSVC_IDE)
    set(module_LIBS ${module_LIBS} optimized MSVCRT)
    set(module_LIBS ${module_LIBS} debug MSVCRTD)
  endif (MSVC_IDE)
endif (WIN32)

link_directories("${ATRACSYS_SDK_LIB_PATH}")

if (BUILD_WITH_PRIVATE_SDK)
  target_compile_definitions(${module_NAME} PRIVATE MODULE_NAME=${module_NAME}
                                                    BUILD_WITH_PRIVATE_SDK)
else (BUILD_WITH_PRIVATE_SDK)
  target_compile_definitions(${module_NAME} PRIVATE MODULE_NAME=${module_NAME})
endif ()

target_link_libraries(${module_NAME} PRIVATE ${module_LIBS})

target_include_directories(
  ${module_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src ${PYTHON_INCLUDE_DIRS}
    "${CMAKE_CURRENT_SOURCE_DIR}" "${ATRACSYS_SDK_INCLUDE_PATH}"
    "${ATRACSYS_CPP_SDK_HEADERS}")

if (${TRACKER_TYPE} STREQUAL "ftk")
  target_compile_definitions(${module_NAME} PRIVATE -DATR_FTK)
elseif (${TRACKER_TYPE} STREQUAL "stk")
  target_compile_definitions(${module_NAME} PRIVATE -DATR_STK)
else ()
  message(FATAL_ERROR "No set tracker type")
endif ()
