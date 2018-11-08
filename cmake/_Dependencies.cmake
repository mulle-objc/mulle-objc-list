#
# cmake/_Dependencies.cmake is generated by `mulle-sde`. Edits will be lost.
#
if( MULLE_TRACE_INCLUDE)
   message( STATUS "# Include \"${CMAKE_CURRENT_LIST_FILE}\"" )
endif()

if( NOT MULLE_OBJC_RUNTIME_LIBRARY)
   find_library( MULLE_OBJC_RUNTIME_LIBRARY NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}mulle-objc-runtime${CMAKE_STATIC_LIBRARY_SUFFIX} mulle-objc-runtime)
   message( STATUS "MULLE_OBJC_RUNTIME_LIBRARY is ${MULLE_OBJC_RUNTIME_LIBRARY}")
   #
   # the order looks ascending, but due to the way this file is read
   # it ends up being descending, which is what we need
   if( MULLE_OBJC_RUNTIME_LIBRARY)
      #
      # Add to MULLE_OBJC_RUNTIME_LIBRARY list.
      # Disable with: `mark no-cmakeadd`
      #
      set( ALL_LOAD_DEPENDENCY_LIBRARIES
         ${ALL_LOAD_DEPENDENCY_LIBRARIES}
         ${MULLE_OBJC_RUNTIME_LIBRARY}
         CACHE INTERNAL "need to cache this"
      )
      #
      # Inherit ObjC loader and link dependency info.
      # Disable with: `mark no-cmakeinherit`
      #
      # // temporarily expand CMAKE_MODULE_PATH
      get_filename_component( _TMP_MULLE_OBJC_RUNTIME_ROOT "${MULLE_OBJC_RUNTIME_LIBRARY}" DIRECTORY)
      get_filename_component( _TMP_MULLE_OBJC_RUNTIME_ROOT "${_TMP_MULLE_OBJC_RUNTIME_ROOT}" DIRECTORY)
      #
      #
      # Search for "DependenciesAndLibraries.cmake" to include.
      # Disable with: `mark no-cmakedependency`
      #
      foreach( _TMP_MULLE_OBJC_RUNTIME_NAME "mulle-objc-runtime")
         set( _TMP_MULLE_OBJC_RUNTIME_DIR "${_TMP_MULLE_OBJC_RUNTIME_ROOT}/include/${_TMP_MULLE_OBJC_RUNTIME_NAME}/cmake")
         # use explicit path to avoid "surprises"
         if( EXISTS "${_TMP_MULLE_OBJC_RUNTIME_DIR}/DependenciesAndLibraries.cmake")
            unset( MULLE_OBJC_RUNTIME_DEFINITIONS)
            list( INSERT CMAKE_MODULE_PATH 0 "${_TMP_MULLE_OBJC_RUNTIME_DIR}")
            # we only want top level INHERIT_OBJC_LOADERS, so disable them
            if( NOT NO_INHERIT_OBJC_LOADERS)
               set( NO_INHERIT_OBJC_LOADERS OFF)
            endif()
            list( APPEND _TMP_INHERIT_OBJC_LOADERS ${NO_INHERIT_OBJC_LOADERS})
            set( NO_INHERIT_OBJC_LOADERS ON)
            #
            include( "${_TMP_MULLE_OBJC_RUNTIME_DIR}/DependenciesAndLibraries.cmake")
            #
            list( GET _TMP_INHERIT_OBJC_LOADERS -1 NO_INHERIT_OBJC_LOADERS)
            list( REMOVE_AT _TMP_INHERIT_OBJC_LOADERS -1)
            #
            list( REMOVE_ITEM CMAKE_MODULE_PATH "${_TMP_MULLE_OBJC_RUNTIME_DIR}")
            set( INHERITED_DEFINITIONS
               ${INHERITED_DEFINITIONS}
               ${MULLE_OBJC_RUNTIME_DEFINITIONS}
               CACHE INTERNAL "need to cache this"
            )
            break()
         else()
            message( STATUS "${_TMP_MULLE_OBJC_RUNTIME_DIR}/DependenciesAndLibraries.cmake not found")
         endif()
      endforeach()
      #
      # Search for "objc-loader.inc" in include directory.
      # Disable with: `mark no-cmakeloader`
      #
      if( NOT NO_INHERIT_OBJC_LOADERS)
         foreach( _TMP_MULLE_OBJC_RUNTIME_NAME "mulle-objc-runtime")
            set( _TMP_MULLE_OBJC_RUNTIME_FILE "${_TMP_MULLE_OBJC_RUNTIME_ROOT}/include/${_TMP_MULLE_OBJC_RUNTIME_NAME}/objc-loader.inc")
            if( EXISTS "${_TMP_MULLE_OBJC_RUNTIME_FILE}")
               set( INHERITED_OBJC_LOADERS
                  ${INHERITED_OBJC_LOADERS}
                  ${_TMP_MULLE_OBJC_RUNTIME_FILE}
                  CACHE INTERNAL "need to cache this"
               )
               break()
            endif()
         endforeach()
      endif()
   else()
      message( FATAL_ERROR "MULLE_OBJC_RUNTIME_LIBRARY was not found")
   endif()
endif()
