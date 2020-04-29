
#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(pinlxr)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

find_package(ROOT REQUIRED)

#include("${ROOT_DIR}/modules/RootNewMacros.cmake")
include("${ROOT_USE_FILE}")
# ROOT version 6 required
if(ROOT_FOUND)
  STRING(REGEX MATCH "6.*" VERSION6MATCH ${ROOT_VERSION})
  if(NOT VERSION6MATCH)
    message(FATAL_ERROR "CXPD requires ROOT 6")
  endif()
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIRS})



file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(stack stack.cxx ${sources} ${headers})
target_link_libraries(stack ${ROOT_LIBRARIES})

add_executable(read read.cxx ${sources} ${headers})
target_link_libraries(read ${ROOT_LIBRARIES})

add_executable(stepstackmode stepstackmode.cxx ${sources} ${headers})
target_link_libraries(stepstackmode ${ROOT_LIBRARIES})

add_executable(dedxvrange dedxvrange.cxx ${sources} ${headers})
target_link_libraries(dedxvrange ${ROOT_LIBRARIES})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
add_executable(protoninlxr protoninlxr.cc ${sources} ${headers})
target_link_libraries(protoninlxr ${Geant4_LIBRARIES})



#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build pinlxr. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(protoninlxr_SCRIPTS
  protoninlxr.in
  protoninlxr.out
  init_vis.mac
  run.mac
  vis.mac
  )

foreach(_script ${protoninlxr_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(pinlxr DEPENDS protoninlxr)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS protoninlxr DESTINATION bin)


