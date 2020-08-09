find_package(ROOT REQUIRED Spectrum)

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
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIRS})



file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(stack stack.cxx ${sources} ${headers})
target_link_libraries(stack ${ROOT_LIBRARIES})
install(TARGETS stack DESTINATION bin)

add_executable(read read.cxx ${sources} ${headers})
target_link_libraries(read ${ROOT_LIBRARIES})
install(TARGETS read DESTINATION bin)

add_executable(stepstackmode stepstackmode.cxx ${sources} ${headers})
target_link_libraries(stepstackmode ${ROOT_LIBRARIES})
install(TARGETS stepstackmode DESTINATION bin)

add_executable(dedxvrange dedxvrange.cxx ${sources} ${headers})
target_link_libraries(dedxvrange ${ROOT_LIBRARIES})
install(TARGETS dedxvrange DESTINATION bin)

add_executable(2ddedxvrange 2ddedxvrange.cxx ${sources} ${headers})
target_link_libraries(2ddedxvrange ${ROOT_LIBRARIES})
install(TARGETS 2ddedxvrange DESTINATION bin)


add_executable(stack_ending stack_ending.cxx ${sources} ${headers})
target_link_libraries(stack_ending ${ROOT_LIBRARIES})
install(TARGETS stack_ending DESTINATION bin)


add_executable(stack_ending_dedx stack_ending_dedx.cxx ${sources} ${headers})
target_link_libraries(stack_ending_dedx ${ROOT_LIBRARIES})
install(TARGETS stack_ending_dedx DESTINATION bin)


add_executable(getpeak getpeak.cxx ${sources} ${headers})
target_link_libraries(getpeak ${ROOT_LIBRARIES})
install(TARGETS getpeak DESTINATION bin)
