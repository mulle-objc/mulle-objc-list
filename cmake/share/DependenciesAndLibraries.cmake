### If you want to edit this, copy it from cmake/share to cmake. It will be
### picked up in preference over the one in cmake/share. And it will not get
### clobbered with the next upgrade.

if( MULLE_TRACE_INCLUDE)
   message( STATUS "# Include \"${CMAKE_CURRENT_LIST_FILE}\"" )
endif()

#
# Track include depth so that when this file is included transitively (as a
# dependency of a dependency), we save and restore INHERITED_OBJC_DEPS. This
# prevents sub-dependency ObjC loader markers from propagating up through
# meta-libraries (like MulleFoundation) into the top-level project's dep list.
# The caller's _Dependencies.cmake is responsible for adding this library's own
# MulleObjCDeps+<name>.h marker after the include returns.
#
# Save keys are indexed by depth so that nested includes at different depth
# levels each get a unique slot and never clobber each other's saved values.
#
if( NOT DEFINED MULLE_OBJC_DEPS_INCLUDE_DEPTH)
   set( MULLE_OBJC_DEPS_INCLUDE_DEPTH 0)
endif()

if( MULLE_OBJC_DEPS_INCLUDE_DEPTH GREATER 0)
   set( "_MULLE_DAL_SAVED_OBJC_DEPS_${MULLE_OBJC_DEPS_INCLUDE_DEPTH}" "${INHERITED_OBJC_DEPS}")
endif()

math( EXPR MULLE_OBJC_DEPS_INCLUDE_DEPTH "${MULLE_OBJC_DEPS_INCLUDE_DEPTH} + 1")

#
# Get Libraries first. That way local library definitions override those
# we might inherit from dependencies. The link order should not be affected by
# this.
#
include( _Libraries OPTIONAL)

#
# If we are in an IDE like CLion and the dependencies haven't been made yet
# cmake is unhappy, try to avoid that.
#
if( IS_DIRECTORY "${DEPENDENCY_DIR}")
   include( _Dependencies OPTIONAL)
else()
   message( STATUS "DEPENDENCY_DIR \"${DEPENDENCY_DIR}\" is missing, so no dependencies")
endif()

math( EXPR MULLE_OBJC_DEPS_INCLUDE_DEPTH "${MULLE_OBJC_DEPS_INCLUDE_DEPTH} - 1")

if( MULLE_OBJC_DEPS_INCLUDE_DEPTH GREATER 0)
   # Use depth as key (post-decrement matches the pre-increment save key)
   set( _MULLE_DAL_RESTORE_KEY "_MULLE_DAL_SAVED_OBJC_DEPS_${MULLE_OBJC_DEPS_INCLUDE_DEPTH}")
   set( INHERITED_OBJC_DEPS "${${_MULLE_DAL_RESTORE_KEY}}")
   unset( "${_MULLE_DAL_RESTORE_KEY}")
   unset( _MULLE_DAL_RESTORE_KEY)
endif()
