#
# You can put more source and resource file definitions here.
#
set( SOURCES
   src/main.c
)

set( SCRIPTS
   mulle-objc-encode
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
