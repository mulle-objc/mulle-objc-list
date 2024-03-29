/* DOS:   #import this files in public headers
 *
 * DONTS: #import this file in private headers
 *        #import this files directly in sources
 *        #include this file anywhere
 *
 * This is a central include file to keep dependencies out of the library
 *  Objective-C files. It is usally imported by Objective-C .h files only.
 *  .m and .aam use import-private.h.
 */

// this is a global symbol that is exposed, which can be used to detect
// if this library is available

#define HAVE_IMPORT_MYLIB

/*
 * Get C includes first
 */
#ifdef __has_include
# if __has_include( "include.h")
#   include "include.h"
# endif
#endif


/* Include the header file automatically generated by mulle-sourcetree-to-c.
   Here the prefix is harmless and serves disambiguation. If you have no
   sourcetree, then you don't need it.
 */

#import "_mylib-import.h"

/* You can add some more import statements here */
