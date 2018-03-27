cmake_minimum_required( VERSION 3.0)

project( mulle-objc-list C)


### mulle-sde environment

# add cmake module paths to search path
list( INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/share")
list( INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake")

include( Environment)


### Files

include( PreFiles OPTIONAL)

#
# Include all .cmake files in ./cmake
#
file( GLOB LOCAL_CMAKE_INCLUDES
   "cmake/[A-Za-z0-9]*.cmake"
)

foreach( FILE ${LOCAL_CMAKE_INCLUDES} )
   include( "${FILE}")
endforeach()


include( PostFiles OPTIONAL)


### Executable

include( PreExecutable OPTIONAL)

add_executable( mulle-objc-list
${SOURCES}
${PUBLIC_HEADERS}
${PRIVATE_HEADERS}
${CMAKE_EDITABLE_FILES}
)

message( STATUS "DEPENDENCY_LIBRARIES=${DEPENDENCY_LIBRARIES}")
message( STATUS "OPTIONAL_DEPENDENCY_LIBRARIES=${OPTIONAL_DEPENDENCY_LIBRARIES}")
message( STATUS "OS_SPECIFIC_LIBRARIES=${OS_SPECIFIC_LIBRARIES}")

target_link_libraries( mulle-objc-list
${DEPENDENCY_LIBRARIES}
${OPTIONAL_DEPENDENCY_LIBRARIES}
${OS_SPECIFIC_LIBRARIES}
)

target_compile_definitions( mulle-objc-list
PRIVATE ${MULLE_OBJC_LIST_DEFINITIONS}
)

#
# see: https://john.nachtimwald.com/2012/07/15/calling-functions-in-exe-from-plugins-in-windows/
#
if( MSVC)
   target_link_libraries( mulle-objc-list
      "-export:__mulle_objc_loadinfo_callback"
      "-export:__mulle_objc_list_callback"
   )
endif()

include( PostExecutable OPTIONAL)

### Install

include( PreInstall OPTIONAL)

install( TARGETS mulle-objc-list DESTINATION "bin")

install( FILES ${PUBLIC_HEADERS} DESTINATION "include/mulle-objc-list")
install( FILES ${CMAKE_INCLUDES} DESTINATION "include/mulle-objc-list/cmake")

install( FILES ${SCRIPTS}
PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
            GROUP_EXECUTE GROUP_READ
            WORLD_EXECUTE WORLD_READ
DESTINATION "bin")

include( PostInstall OPTIONAL)

# only for Executable

include( Motd OPTIONAL)
