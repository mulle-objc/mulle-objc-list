cmake_minimum_required( VERSION 3.0)

project( mulle-objc-method-list)

set( CMAKE_MODULE_PATH
${CMAKE_MODULE_PATH}
"${CMAKE_SOURCE_DIR}/mulle-configuration"
)

set( MULLE_LANGUAGE "ObjC")
if( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  include( Debug)
else()
  include( Release)
endif()

find_library( MULLE_ABA_LIBRARY            NAMES mulle_aba)
find_library( MULLE_ALLOCATOR_LIBRARY      NAMES mulle_allocator)
find_library( MULLE_CONCURRENT_LIBRARY     NAMES mulle_concurrent)
find_library( MULLE_CONTAINER_LIBRARY      NAMES mulle_container)
find_library( MULLE_OBJC_RUNTIME_LIBRARY   NAMES mulle_objc)
find_library( MULLE_TEST_ALLOCATOR_LIBRARY NAMES mulle_test_allocator)
find_library( MULLE_THREAD_LIBRARY         NAMES mulle_thread)

message( STATUS "MULLE_ABA_LIBRARY is ${MULLE_ABA_LIBRARY}")
message( STATUS "MULLE_ALLOCATOR_LIBRARY is ${MULLE_ALLOCATOR_LIBRARY}")
message( STATUS "MULLE_CONCURRENT_LIBRARY is ${MULLE_CONCURRENT_LIBRARY}")
message( STATUS "MULLE_CONTAINER_LIBRARY is ${MULLE_CONTAINER_LIBRARY}")
message( STATUS "MULLE_OBJC_RUNTIME_LIBRARY is ${MULLE_OBJC_RUNTIME_LIBRARY}")
message( STATUS "MULLE_TEST_ALLOCATOR_LIBRARY is ${MULLE_TEST_ALLOCATOR_LIBRARY}")
message( STATUS "MULLE_THREAD_LIBRARY is ${MULLE_THREAD_LIBRARY}")

set( LIBRARIES
${MULLE_ABA_LIBRARY}
${MULLE_ALLOCATOR_LIBRARY}
${MULLE_CONCURRENT_LIBRARY}
${MULLE_CONTAINER_LIBRARY}
${MULLE_OBJC_RUNTIME_LIBRARY}
${MULLE_TEST_ALLOCATOR_LIBRARY}
${MULLE_THREAD_LIBRARY}
)

###

include( CMakeSourcesAndHeaders.txt)

###

add_executable( "mulle-objc-method-list"
${SOURCES}
)
target_link_libraries( "mulle-objc-method-list"
${BEGIN_ALL_LOAD}
${LIBRARIES}
${END_ALL_LOAD}
)

install( TARGETS "mulle-objc-method-list" DESTINATION "bin")
