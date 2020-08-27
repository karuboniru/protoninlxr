
#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(pinlxr)
add_subdirectory(analysis)

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

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})


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
  run.sh
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


