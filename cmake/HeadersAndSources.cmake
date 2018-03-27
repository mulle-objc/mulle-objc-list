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

set( SCRIPTS
mulle-objc-create-dependencies-inc
mulle-objc-lista
mulle-objc-optimize
mulle-objc-printline
mulle-objc-searchid
mulle-objc-unarchive
mulle-objc-uncovered-methods
)


# add ignored header back in
set( PUBLIC_HEADERS
"src/_mulle-objc-list-dependencies.h"
${PUBLIC_HEADERS}
)

if( MSVC)
   set( SCRIPTS
      ${SCRIPTS}
      mulle-objc-create-dependencies-inc.bat
      mulle-objc-list.bat
   )
endif()

#
# You can put more source and resource file definitions here.
#
