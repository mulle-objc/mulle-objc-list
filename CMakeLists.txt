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

# since it's an executable we don't need to ALL-LOAD
target_link_libraries( "mulle-objc-list"
${C_DEPENDENCY_LIBRARIES}
${OS_SPECIFIC_LIBRARIES}
)

install( TARGETS "mulle-objc-list" DESTINATION "bin")
install( FILES "mulle-objc-uncovered-methods" DESTINATION "bin")
install( FILES ${PUBLIC_HEADERS} DESTINATION "include/mulle_objc_list")