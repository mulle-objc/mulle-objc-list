#
# This file will be included by cmake/share/Files.cmake
#
# cmake/reflect/_Dependencies.cmake is generated by `mulle-sde reflect`.
# Edits will be lost.
#
# Disable generation of this file with:
#
# mulle-sde environment set MULLE_SOURCETREE_TO_CMAKE_DEPENDENCIES_FILE DISABLE
#
if( MULLE_TRACE_INCLUDE)
   message( STATUS "# Include \"${CMAKE_CURRENT_LIST_FILE}\"" )
endif()

#
# Generated from sourcetree: 315B9D64-1484-402B-AD9E-D8AD1E86AF4B;mulle-dlfcn;no-actual-link,no-all-load,no-bequeath,no-cmake-searchpath,no-import,no-singlephase;
# Disable with : `mulle-sourcetree mark mulle-dlfcn no-header`
# Disable for this platform: `mulle-sourcetree mark mulle-dlfcn no-cmake-platform-darwin`
#
if( NOT MULLE_DLFCN_HEADER)
   find_file( MULLE_DLFCN_HEADER NAMES mulle-dlfcn.h mulle-dlfcn/mulle-dlfcn.h)
   message( STATUS "MULLE_DLFCN_HEADER is ${MULLE_DLFCN_HEADER}")

   #
   # Add MULLE_DLFCN_HEADER to HEADER_ONLY_LIBRARIES list.
   # Disable with: `mulle-sourcetree mark mulle-dlfcn no-cmake-add`
   #
   set( HEADER_ONLY_LIBRARIES
      ${MULLE_DLFCN_HEADER}
      ${HEADER_ONLY_LIBRARIES}
      CACHE INTERNAL "need to cache this"
   )
   if( MULLE_DLFCN_HEADER)
      #
      # Inherit ObjC loader and link dependency info.
      # Disable with: `mulle-sourcetree mark mulle-dlfcn no-cmake-inherit`
      #
      get_filename_component( _TMP_MULLE_DLFCN_ROOT "${MULLE_DLFCN_HEADER}" DIRECTORY)
      get_filename_component( _TMP_MULLE_DLFCN_NAME "${_TMP_MULLE_DLFCN_ROOT}" NAME)
      get_filename_component( _TMP_MULLE_DLFCN_ROOT "${_TMP_MULLE_DLFCN_ROOT}" DIRECTORY)
      get_filename_component( _TMP_MULLE_DLFCN_ROOT "${_TMP_MULLE_DLFCN_ROOT}" DIRECTORY)
      #
      # Search for "DependenciesAndLibraries.cmake" to include.
      # Disable with: `mulle-sourcetree mark mulle-dlfcn no-cmake-dependency`
      #
      foreach( _TMP_MULLE_DLFCN_NAME IN LISTS _TMP_MULLE_DLFCN_NAME)
         set( _TMP_MULLE_DLFCN_DIR "${_TMP_MULLE_DLFCN_ROOT}/include/${_TMP_MULLE_DLFCN_NAME}/cmake")
         # use explicit path to avoid "surprises"
         if( EXISTS "${_TMP_MULLE_DLFCN_DIR}/DependenciesAndLibraries.cmake")
            unset( MULLE_DLFCN_DEFINITIONS)
            list( INSERT CMAKE_MODULE_PATH 0 "${_TMP_MULLE_DLFCN_DIR}")
            # we only want top level INHERIT_OBJC_LOADERS, so disable them
            if( NOT NO_INHERIT_OBJC_LOADERS)
               set( NO_INHERIT_OBJC_LOADERS OFF)
            endif()
            list( APPEND _TMP_INHERIT_OBJC_LOADERS ${NO_INHERIT_OBJC_LOADERS})
            set( NO_INHERIT_OBJC_LOADERS ON)
            #
            include( "${_TMP_MULLE_DLFCN_DIR}/DependenciesAndLibraries.cmake")
            #
            list( GET _TMP_INHERIT_OBJC_LOADERS -1 NO_INHERIT_OBJC_LOADERS)
            list( REMOVE_AT _TMP_INHERIT_OBJC_LOADERS -1)
            #
            list( REMOVE_ITEM CMAKE_MODULE_PATH "${_TMP_MULLE_DLFCN_DIR}")
            set( INHERITED_DEFINITIONS
               ${INHERITED_DEFINITIONS}
               ${MULLE_DLFCN_DEFINITIONS}
               CACHE INTERNAL "need to cache this"
            )
            break()
         else()
            message( STATUS "${_TMP_MULLE_DLFCN_DIR}/DependenciesAndLibraries.cmake not found")
         endif()
      endforeach()
   else()
      # Disable with: `mulle-sourcetree mark mulle-dlfcn no-require`
      message( FATAL_ERROR "MULLE_DLFCN_HEADER was not found")
   endif()
endif()



#
# Generated from sourcetree: 1a4c5f04-6d19-477a-bfb7-55a189dd1e54;mulle-objc-runtime;no-bequeath,no-cmake-searchpath,no-header,no-import,no-singlephase;
# Disable with : `mulle-sourcetree mark mulle-objc-runtime no-link`
# Disable for this platform: `mulle-sourcetree mark mulle-objc-runtime no-cmake-platform-darwin`
#
if( NOT MULLE_OBJC_RUNTIME_LIBRARY)
   find_library( MULLE_OBJC_RUNTIME_LIBRARY NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}mulle-objc-runtime${CMAKE_STATIC_LIBRARY_SUFFIX} mulle-objc-runtime NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH)
   message( STATUS "MULLE_OBJC_RUNTIME_LIBRARY is ${MULLE_OBJC_RUNTIME_LIBRARY}")
   #
   # The order looks ascending, but due to the way this file is read
   # it ends up being descending, which is what we need.
   #
   if( MULLE_OBJC_RUNTIME_LIBRARY)
      #
      # Add MULLE_OBJC_RUNTIME_LIBRARY to ALL_LOAD_DEPENDENCY_LIBRARIES list.
      # Disable with: `mulle-sourcetree mark mulle-objc-runtime no-cmake-add`
      #
      set( ALL_LOAD_DEPENDENCY_LIBRARIES
         ${ALL_LOAD_DEPENDENCY_LIBRARIES}
         ${MULLE_OBJC_RUNTIME_LIBRARY}
         CACHE INTERNAL "need to cache this"
      )
      #
      # Inherit information from dependency.
      # Encompasses: no-cmake-searchpath,no-cmake-dependency,no-cmake-loader
      # Disable with: `mulle-sourcetree mark mulle-objc-runtime no-cmake-inherit`
      #
      # temporarily expand CMAKE_MODULE_PATH
      get_filename_component( _TMP_MULLE_OBJC_RUNTIME_ROOT "${MULLE_OBJC_RUNTIME_LIBRARY}" DIRECTORY)
      get_filename_component( _TMP_MULLE_OBJC_RUNTIME_ROOT "${_TMP_MULLE_OBJC_RUNTIME_ROOT}" DIRECTORY)
      #
      #
      # Search for "DependenciesAndLibraries.cmake" to include.
      # Disable with: `mulle-sourcetree mark mulle-objc-runtime no-cmake-dependency`
      #
      foreach( _TMP_MULLE_OBJC_RUNTIME_NAME "mulle-objc-runtime")
         set( _TMP_MULLE_OBJC_RUNTIME_DIR "${_TMP_MULLE_OBJC_RUNTIME_ROOT}/include/${_TMP_MULLE_OBJC_RUNTIME_NAME}/cmake")
         # use explicit path to avoid "surprises"
         if( EXISTS "${_TMP_MULLE_OBJC_RUNTIME_DIR}/DependenciesAndLibraries.cmake")
            unset( MULLE_OBJC_RUNTIME_DEFINITIONS)
            list( INSERT CMAKE_MODULE_PATH 0 "${_TMP_MULLE_OBJC_RUNTIME_DIR}")
            #
            include( "${_TMP_MULLE_OBJC_RUNTIME_DIR}/DependenciesAndLibraries.cmake")
            #
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
      # Search for "MulleObjCLoader+<name>.h" in include directory.
      # Disable with: `mulle-sourcetree mark mulle-objc-runtime no-cmake-loader`
      #
      if( NOT NO_INHERIT_OBJC_LOADERS)
         foreach( _TMP_MULLE_OBJC_RUNTIME_NAME "mulle-objc-runtime")
            set( _TMP_MULLE_OBJC_RUNTIME_FILE "${_TMP_MULLE_OBJC_RUNTIME_ROOT}/include/${_TMP_MULLE_OBJC_RUNTIME_NAME}/MulleObjCLoader+${_TMP_MULLE_OBJC_RUNTIME_NAME}.h")
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
      # Disable with: `mulle-sourcetree mark mulle-objc-runtime no-require-link`
      message( FATAL_ERROR "MULLE_OBJC_RUNTIME_LIBRARY was not found")
   endif()
endif()


#
# Generated from sourcetree: 4e27ab5c-4bc1-4fea-adb7-405231f43c8c;mulle-atinit;no-bequeath,no-cmake-searchpath,no-dynamic-link,no-header,no-import,no-intermediate-link,no-singlephase;
# Disable with : `mulle-sourcetree mark mulle-atinit no-link`
# Disable for this platform: `mulle-sourcetree mark mulle-atinit no-cmake-platform-darwin`
#
if( NOT MULLE_ATINIT_LIBRARY)
   find_library( MULLE_ATINIT_LIBRARY NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}mulle-atinit${CMAKE_STATIC_LIBRARY_SUFFIX} mulle-atinit NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH)
   message( STATUS "MULLE_ATINIT_LIBRARY is ${MULLE_ATINIT_LIBRARY}")
   #
   # The order looks ascending, but due to the way this file is read
   # it ends up being descending, which is what we need.
   #
   if( MULLE_ATINIT_LIBRARY)
      #
      # Add MULLE_ATINIT_LIBRARY to STARTUP_ALL_LOAD_DEPENDENCY_LIBRARIES list.
      # Disable with: `mulle-sourcetree mark mulle-atinit no-cmake-add`
      #
      set( STARTUP_ALL_LOAD_DEPENDENCY_LIBRARIES
         ${STARTUP_ALL_LOAD_DEPENDENCY_LIBRARIES}
         ${MULLE_ATINIT_LIBRARY}
         CACHE INTERNAL "need to cache this"
      )
      #
      # Inherit information from dependency.
      # Encompasses: no-cmake-searchpath,no-cmake-dependency,no-cmake-loader
      # Disable with: `mulle-sourcetree mark mulle-atinit no-cmake-inherit`
      #
      # temporarily expand CMAKE_MODULE_PATH
      get_filename_component( _TMP_MULLE_ATINIT_ROOT "${MULLE_ATINIT_LIBRARY}" DIRECTORY)
      get_filename_component( _TMP_MULLE_ATINIT_ROOT "${_TMP_MULLE_ATINIT_ROOT}" DIRECTORY)
      #
      #
      # Search for "DependenciesAndLibraries.cmake" to include.
      # Disable with: `mulle-sourcetree mark mulle-atinit no-cmake-dependency`
      #
      foreach( _TMP_MULLE_ATINIT_NAME "mulle-atinit")
         set( _TMP_MULLE_ATINIT_DIR "${_TMP_MULLE_ATINIT_ROOT}/include/${_TMP_MULLE_ATINIT_NAME}/cmake")
         # use explicit path to avoid "surprises"
         if( EXISTS "${_TMP_MULLE_ATINIT_DIR}/DependenciesAndLibraries.cmake")
            unset( MULLE_ATINIT_DEFINITIONS)
            list( INSERT CMAKE_MODULE_PATH 0 "${_TMP_MULLE_ATINIT_DIR}")
            #
            include( "${_TMP_MULLE_ATINIT_DIR}/DependenciesAndLibraries.cmake")
            #
            #
            list( REMOVE_ITEM CMAKE_MODULE_PATH "${_TMP_MULLE_ATINIT_DIR}")
            set( INHERITED_DEFINITIONS
               ${INHERITED_DEFINITIONS}
               ${MULLE_ATINIT_DEFINITIONS}
               CACHE INTERNAL "need to cache this"
            )
            break()
         else()
            message( STATUS "${_TMP_MULLE_ATINIT_DIR}/DependenciesAndLibraries.cmake not found")
         endif()
      endforeach()
      #
      # Search for "MulleObjCLoader+<name>.h" in include directory.
      # Disable with: `mulle-sourcetree mark mulle-atinit no-cmake-loader`
      #
      if( NOT NO_INHERIT_OBJC_LOADERS)
         foreach( _TMP_MULLE_ATINIT_NAME "mulle-atinit")
            set( _TMP_MULLE_ATINIT_FILE "${_TMP_MULLE_ATINIT_ROOT}/include/${_TMP_MULLE_ATINIT_NAME}/MulleObjCLoader+${_TMP_MULLE_ATINIT_NAME}.h")
            if( EXISTS "${_TMP_MULLE_ATINIT_FILE}")
               set( INHERITED_OBJC_LOADERS
                  ${INHERITED_OBJC_LOADERS}
                  ${_TMP_MULLE_ATINIT_FILE}
                  CACHE INTERNAL "need to cache this"
               )
               break()
            endif()
         endforeach()
      endif()
   else()
      # Disable with: `mulle-sourcetree mark mulle-atinit no-require-link`
      message( FATAL_ERROR "MULLE_ATINIT_LIBRARY was not found")
   endif()
endif()


#
# Generated from sourcetree: e8de79c9-c40a-41f9-bc5e-c7830831afe7;mulle-atexit;no-bequeath,no-cmake-searchpath,no-dynamic-link,no-header,no-import,no-intermediate-link,no-singlephase;
# Disable with : `mulle-sourcetree mark mulle-atexit no-link`
# Disable for this platform: `mulle-sourcetree mark mulle-atexit no-cmake-platform-darwin`
#
if( NOT MULLE_ATEXIT_LIBRARY)
   find_library( MULLE_ATEXIT_LIBRARY NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}mulle-atexit${CMAKE_STATIC_LIBRARY_SUFFIX} mulle-atexit NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH)
   message( STATUS "MULLE_ATEXIT_LIBRARY is ${MULLE_ATEXIT_LIBRARY}")
   #
   # The order looks ascending, but due to the way this file is read
   # it ends up being descending, which is what we need.
   #
   if( MULLE_ATEXIT_LIBRARY)
      #
      # Add MULLE_ATEXIT_LIBRARY to STARTUP_ALL_LOAD_DEPENDENCY_LIBRARIES list.
      # Disable with: `mulle-sourcetree mark mulle-atexit no-cmake-add`
      #
      set( STARTUP_ALL_LOAD_DEPENDENCY_LIBRARIES
         ${STARTUP_ALL_LOAD_DEPENDENCY_LIBRARIES}
         ${MULLE_ATEXIT_LIBRARY}
         CACHE INTERNAL "need to cache this"
      )
      #
      # Inherit information from dependency.
      # Encompasses: no-cmake-searchpath,no-cmake-dependency,no-cmake-loader
      # Disable with: `mulle-sourcetree mark mulle-atexit no-cmake-inherit`
      #
      # temporarily expand CMAKE_MODULE_PATH
      get_filename_component( _TMP_MULLE_ATEXIT_ROOT "${MULLE_ATEXIT_LIBRARY}" DIRECTORY)
      get_filename_component( _TMP_MULLE_ATEXIT_ROOT "${_TMP_MULLE_ATEXIT_ROOT}" DIRECTORY)
      #
      #
      # Search for "DependenciesAndLibraries.cmake" to include.
      # Disable with: `mulle-sourcetree mark mulle-atexit no-cmake-dependency`
      #
      foreach( _TMP_MULLE_ATEXIT_NAME "mulle-atexit")
         set( _TMP_MULLE_ATEXIT_DIR "${_TMP_MULLE_ATEXIT_ROOT}/include/${_TMP_MULLE_ATEXIT_NAME}/cmake")
         # use explicit path to avoid "surprises"
         if( EXISTS "${_TMP_MULLE_ATEXIT_DIR}/DependenciesAndLibraries.cmake")
            unset( MULLE_ATEXIT_DEFINITIONS)
            list( INSERT CMAKE_MODULE_PATH 0 "${_TMP_MULLE_ATEXIT_DIR}")
            #
            include( "${_TMP_MULLE_ATEXIT_DIR}/DependenciesAndLibraries.cmake")
            #
            #
            list( REMOVE_ITEM CMAKE_MODULE_PATH "${_TMP_MULLE_ATEXIT_DIR}")
            set( INHERITED_DEFINITIONS
               ${INHERITED_DEFINITIONS}
               ${MULLE_ATEXIT_DEFINITIONS}
               CACHE INTERNAL "need to cache this"
            )
            break()
         else()
            message( STATUS "${_TMP_MULLE_ATEXIT_DIR}/DependenciesAndLibraries.cmake not found")
         endif()
      endforeach()
      #
      # Search for "MulleObjCLoader+<name>.h" in include directory.
      # Disable with: `mulle-sourcetree mark mulle-atexit no-cmake-loader`
      #
      if( NOT NO_INHERIT_OBJC_LOADERS)
         foreach( _TMP_MULLE_ATEXIT_NAME "mulle-atexit")
            set( _TMP_MULLE_ATEXIT_FILE "${_TMP_MULLE_ATEXIT_ROOT}/include/${_TMP_MULLE_ATEXIT_NAME}/MulleObjCLoader+${_TMP_MULLE_ATEXIT_NAME}.h")
            if( EXISTS "${_TMP_MULLE_ATEXIT_FILE}")
               set( INHERITED_OBJC_LOADERS
                  ${INHERITED_OBJC_LOADERS}
                  ${_TMP_MULLE_ATEXIT_FILE}
                  CACHE INTERNAL "need to cache this"
               )
               break()
            endif()
         endforeach()
      endif()
   else()
      # Disable with: `mulle-sourcetree mark mulle-atexit no-require-link`
      message( FATAL_ERROR "MULLE_ATEXIT_LIBRARY was not found")
   endif()
endif()
