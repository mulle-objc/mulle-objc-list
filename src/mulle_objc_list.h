#ifndef mulle_objc_list_h__
#define mulle_objc_list_h__

#include <mulle_objc_runtime/mulle_objc_universe.h>
#include <stdio.h>


// clang speciality
#ifdef __has_include
# if __has_include( <dlfcn.h>)
#  include <dlfcn.h>
#  define HAVE_DLSYM  1
# endif
#endif


static inline void   mulle_objc_list_install_hook( struct _mulle_objc_universe *universe)
{
//
// this is for the test generator wedge
// so we only search in MAIN so that the search is hopefully not that slow
//
#if HAVE_DLSYM
   {
#if __APPLE__
# define MULLE_OBJC_DLSYM_HANDLE   RTLD_MAIN_ONLY
#else
# define MULLE_OBJC_DLSYM_HANDLE   RTLD_DEFAULT
#endif
      void      *function;
      Dl_info   info;

      function = dlsym( MULLE_OBJC_DLSYM_HANDLE, "__mulle_objc_loadinfo_callback");
      if( function)
      {
         universe->loadcallbacks.should_load_loadinfo = (int (*)()) function;
         if( universe->debug.trace.loadinfo)
            fprintf( stderr, "mulle_objc_universe %p: "
                 "__mulle_objc_loadinfo_callback set to %p\n", universe, function);
      }

     // set path of universe for debugging
     if( dladdr( (void *) __mulle_objc_universe_setup, &info))
        mulle_objc_universe_set_path( universe, (char *) info.dli_fname);
   }
#endif
}

#endif

