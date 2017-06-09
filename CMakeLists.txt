cmake_minimum_required( VERSION 3.0)

project( mulle-objc-list)

set( CMAKE_MODULE_PATH
${CMAKE_MODULE_PATH}
"${CMAKE_SOURCE_DIR}/mulle-configuration/cmake"
)

set( MULLE_LANGUAGE "ObjC")
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  include( Debug)
else()
  include( Release)
endif()


###

include( CMakeSourcesAndHeaders.txt)
include( CMakeDependencies.txt)

###

add_executable( "mulle-objc-list"
${SOURCES}
)


target_link_libraries( "mulle-objc-list"
${BEGIN_ALL_LOAD}
${DEPENDENCY_LIBRARIES}
${END_ALL_LOAD}
${OS_SPECIFIC_LIBRARIES}
)

install( TARGETS "mulle-objc-list" DESTINATION "bin")
install( FILES "mulle-objc-uncovered-methods" DESTINATION "bin")
