cmake_minimum_required(VERSION 3.16...3.27 FATAL_ERROR)

project(Simulation)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

#Setup CRY-----------------------------------
find_library( CRY NAMES CRY PATHS $ENV{CRYHOME}/lib NO_DEFAULT_PATH )

set(CRY_INCLUDE_DIRS $ENV{CRYHOME}/src)
MESSAGE ("Cry setup: " ${CRY_INCLUDE_DIRS} )
#set(CRY_LIBRARY_DIR ${CRYHOME}/lib)
set(CRY_LIBRARIES $ENV{CRYHOME}/lib/libCRY.a)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
                    ${CRY_INCLUDE_DIRS}
                    )

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

file(GLOB MACRO_FILES
    "*.mac"
)
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_subdirectory(OPSim)
add_subdirectory(sslg4)

add_executable(sim sim.cc ${sources} ${headers})
target_link_libraries(sim OPSim sslg4 ${Geant4_LIBRARIES} ${CRY_LIBRARIES})

set(_SCRIPTS
  setup.file
  )

foreach(_script ${_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

add_custom_target(Simulation DEPENDS sim)
