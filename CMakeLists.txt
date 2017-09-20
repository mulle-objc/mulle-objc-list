cmake_minimum_required( VERSION 3.0)

project( mulle-objc-list C)


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

if( MSVC)
   set( SCRIPTS
	${SCRIPTS}
	mulle-objc-create-dependencies-inc.bat
   )
endif()


add_executable( "mulle-objc-list"
${SOURCES}
)

# since it's an executable we don't need to ALL-LOAD
target_link_libraries( "mulle-objc-list"
${C_DEPENDENCY_LIBRARIES}
${OS_SPECIFIC_LIBRARIES}
)

# PRIVATE is a guess
target_compile_definitions( "mulle-objc-list"
PRIVATE ${MULLE_OBJC_LIST_DEFINITIONS}
)

install( TARGETS "mulle-objc-list" DESTINATION "bin")
install( FILES "mulle-objc-uncovered-methods" DESTINATION "bin")
install( FILES ${PUBLIC_HEADERS} DESTINATION "include/mulle_objc_list")
install( FILES CMakeDependencies.txt DESTINATION "include/mulle_objc_list")


INSTALL( FILES ${SCRIPTS}
PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
            GROUP_EXECUTE GROUP_READ
            WORLD_EXECUTE WORLD_READ
DESTINATION "bin")

