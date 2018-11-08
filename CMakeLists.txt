cmake_minimum_required( VERSION 3.1)

project( mulle-objc-list C)


### mulle-sde environment

# add cmake module paths to search path
list( INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/share")
list( INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake")

include( Environment)

include( Files)

if( HEADERS_PHASE)
   install( FILES ${PUBLIC_HEADERS} DESTINATION "include/mulle-objc-list")

   # short cut out
   if( NOT COMPILE_PHASE AND NOT LINK_PHASE)
      return()
   endif()
endif()

if( LINK_PHASE)
   include( Dependencies OPTIONAL)
endif()


### Executable

#
# we just link mulle-objc-runtime as a c library and not with everything
#
if( LINK_PHASE)
   set( DEPENDENCY_LIBRARIES
      ${ALL_LOAD_DEPENDENCY_LIBRARIES}
      ${DEPENDENCY_LIBRARIES}
      CACHE INTERNAL "need to cache this"
   )
   unset( ALL_LOAD_DEPENDENCY_LIBRARIES CACHE)
endif()

include( Executable)
include( InstallExecutable)

#
# see: https://john.nachtimwald.com/2012/07/15/calling-functions-in-exe-from-plugins-in-windows/
#
if( LINK_PHASE AND MSVC)
   target_link_libraries( "${EXECUTABLE_NAME}"
      "-export:__mulle_objc_loadinfo_callback"
      "-export:__mulle_objc_list_callback"
   )
endif()


install( FILES ${SCRIPTS}
PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
            GROUP_EXECUTE GROUP_READ
            WORLD_EXECUTE WORLD_READ
DESTINATION "bin")


include( FinalOutput OPTIONAL)

# only for Executable
include( Motd OPTIONAL)
