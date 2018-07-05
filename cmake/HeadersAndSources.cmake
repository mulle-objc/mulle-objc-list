#
# The following includes include definitions generated
# during `mulle-sde update`. Don't edit those files. They are
# overwritten frequently.
#
# === MULLE-SDE START ===

include( _Headers)
include( _Sources)

# === MULLE-SDE END ===
#

# add ignored headers back in
set( PUBLIC_HEADERS
   src/mulle-objc-list.h
)

#
# You can put more source and resource file definitions here.
#
set( SOURCES
	src/main.c
)

set( SCRIPTS
	mulle-objc-loader-tool
	mulle-objc-lista
	mulle-objc-optimize
	mulle-objc-printline
	mulle-objc-searchid
	mulle-objc-unarchive
	mulle-objc-uncovered-methods
)

if( MSVC)
   set( SCRIPTS
      ${SCRIPTS}
      mulle-objc-loader-tool.bat
      mulle-objc-list.bat
   )
endif()
