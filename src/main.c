//
//  main.c
//  mulle-objc-list
//
//  Created by Nat! on 29.04.17.
//  Copyright © 2017 Mulle kybernetiK. All rights reserved.
//  Copyright © 2017 Codeon GmbH. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  Neither the name of Mulle kybernetiK nor the names of its contributors
//  may be used to endorse or promote products derived from this software
//  without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//

// we don't really load any code, so this is
// the minimal setup

#define __MULLE_OBJC_NO_TPS__ 1
#define __MULLE_OBJC_NO_TRT__ 1
#define __MULLE_OBJC_NO_FMC__ 1

#include <mulle-objc-runtime/mulle-objc-runtime.h>

#include "dependencies.h"
// direct load instead of dependencies.h because its a debug header
#include <mulle-objc-runtime/mulle-objc-csvdump.h>

#include <ctype.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>

#include "version.h"


#define MULLE_OBJC_LOADER_CLASSID  0x47a9beb6

// #define STANDALONE_MULLE_OBJC

#define _stringify( x) #x
#define stringify( x) _stringify( x)



static int       verbose;
static int       dump;
static int       emit_sentinel;
static uint32_t  loader_classid = MULLE_OBJC_LOADER_CLASSID;

static enum
{
   dump_callable_coverage,
   dump_callable_methods,
   dump_classes,
   dump_classes_categories,
   dump_coverage,  // coverage style
   dump_dependencies,
   dump_info,
   dump_loader,
   dump_methods,
   dump_search
} mode = dump_methods;


static void   log_printf( char *format, ...)
{
   va_list   args;

   if( ! verbose)
      return;

   va_start( args, format);
   vfprintf( stderr, format, args);
   va_end( args);
}


static void   usage( void)
{
   fprintf( stderr,
            "usage: mulle-objc-list [options] [command] [libraries] <binary>\n"
            "\n"
            "   The binary is listed. The preceeding libraries are\n"
            "   explicitly loaded but their contents aren't listed.\n"
            "   Implicitly loaded libraries by binary are listed.\n"
            "\n"
            "Options:\n"
            "   -e      : emit dependencies sentinel field\n"
            "   -l <id> : specify loader-class id for -d (default is 0x%08x)"
            "   -v      : verbose\n"
            "\n"
            "Commands:\n"
            "   -C      : list classes with their superclass\n"
            "   -c      : list classes and categories\n"
            "   -d      : list classes and categories as +dependencies. Skips MulleObjCLoaders\n"
            "   -L      : list MulleObjCLoader dependencies\n"
            "   -M      : list also root -methods as +methods\n"
            "   -T      : terse list methods with root -methods as +methods\n"
            "   -i      : dump loadinfo version information\n"
            "   -m      : list methods (default)\n"
            "   -s      : list classes, categories, methods (mixed CSV)\n"
            "   -t      : terse list methods (coverage like)\n"
            "\n"
            , loader_classid);
   exit( 1);
}

enum
{
   rval_index        = 0,
   object_index      = 1,
   sel_index         = 2,
   first_param_index = 3   // objectAtIndex:i (i == first_param)
};


static char   *simple_typename( char *type)
{
   switch( *type)
   {
   case _C_VOID      : return( "void");
   case _C_RETAIN_ID :
   case _C_COPY_ID   :
   case _C_ASSIGN_ID : return( "id");
   case _C_CLASS     : return( "Class");
   case _C_SEL       : return( "SEL");
   case _C_CHR       : return( "char");
   case _C_UCHR      : return( "unsigned char");
   case _C_SHT       : return( "short");
   case _C_USHT      : return( "unsigned short");
   case _C_UINT      : return( "unsigned int");
   case _C_LNG       : return( "long");
   case _C_ULNG      : return( "unsigned long");
   case _C_LNG_LNG   : return( "long long");
   case _C_ULNG_LNG  : return( "unsigned long long");
   case _C_FLT       : return( "float");
   case _C_DBL       : return( "double");
   case _C_LNG_DBL   : return( "long double");
   case _C_CHARPTR   : return( "char *");
#ifdef _C_BOOL
   case _C_BOOL      : return( "_Bool");
#endif
#ifdef _C_UNDEF
   case _C_UNDEF     : return( "undefined");
#endif
#ifdef _C_ATOM
   case _C_ATOM      : return( "char *");
#endif
   }
   return( 0);
}


static void   print_typeinfo( struct mulle_objc_typeinfo  *typeinfo, char *methodname);


static void   print_int( char *type, char *methodname)
{
   char          **p_s;
   size_t        len;
   static char   *bool_verbs[] =
   {
      "is",
      "has",
      "can",
      "allows",
      "contains",
      "conforms",
      "generated",
      0
   };

   if( methodname)
   {
      while( *methodname == '_')
         ++methodname;

      for( p_s = bool_verbs; *p_s; p_s++)
      {
         len = strlen( *p_s);
         if( ! strncmp( *p_s, methodname, len) && isupper( methodname[ len]))
         {
            printf( "BOOL");
            return;
         }
      }
   }

   printf( "int");
}


static int   has_suffix( char *s, char *suffix)
{
   size_t   len;
   size_t   suffix_len;

   len = s ? strlen( s): 0;
   if( ! len)
      return( 0);

   suffix_len = suffix ? strlen( suffix) : 0;
   if( suffix_len > len)
      return( 0);

   return( ! strcmp( &s[ len - suffix_len], suffix));
}


static void   print_struct_or_union( char *type, char *methodname, char *prefix)
{
   char                         *element;
   struct mulle_objc_typeinfo   typeinfo;

   element = strchr( type, '=');
   if( ! element)
   {
      printf( "**malformed struct signature %s **", type);
      return;
   }

   if( *type == '?')
   {
      ++type;

      if( has_suffix( methodname, "Zone:"))
      {
         printf( "NSZone");
         return;
      }

      ++element;
      printf( "%s { ", prefix);
      mulle_objc_signature_supply_next_typeinfo( element, &typeinfo);
      print_typeinfo( &typeinfo, methodname);
      printf( "}");
      return;
   }

   printf( "%s %.*s", prefix, (int) (element - type), type);
}


static void   print_struct( char *type, char *methodname)
{
   print_struct_or_union( ++type, methodname, "struct");
}


static void   print_union( char *type, char *methodname)
{
   print_struct_or_union( ++type, methodname, "union");
}


static void   print_pointer( char *type, char *methodname)
{
   struct mulle_objc_typeinfo  typeinfo;

   if( mulle_objc_signature_supply_next_typeinfo( ++type, &typeinfo))
      print_typeinfo( &typeinfo, methodname);
   printf( " *");  // hmm
}


static void   print_array( char *type, char *methodname)
{
   struct mulle_objc_typeinfo  typeinfo;

   if( mulle_objc_signature_supply_next_typeinfo( ++type, &typeinfo))
      print_typeinfo( &typeinfo, methodname);
   printf( " []");  // hmm
}


static void   print_bitfield( char *type, char *methodname)
{
   printf( "**bitfield as parameter, O RLY ?**");
   return;
}


static void   print_typeinfo( struct mulle_objc_typeinfo  *typeinfo, char *methodname)
{
   char  *s;
   char  *end;

   if( typeinfo->name)
   {
      s   = &typeinfo->name[ 1];
      end = strchr( s, '"');
      if( ! end || ! *s)
      {
         printf( "**malformed named signature %s **", s);
         return;
      }

      if( *s == '<')
         printf( "id %.*s", (int) (end - s), s);
      else
         printf( "%.*s *", (int) (end - s), s);
      return;
   }

   s = simple_typename( typeinfo->type);
   if( s)
   {
      printf( "%s", s);
      return;
   }

   switch( *typeinfo->type)
   {
   case _C_INT      : print_int( typeinfo->type, methodname); return;
   case _C_PTR      : print_pointer( typeinfo->type, methodname); return;
   case _C_ARY_B    : print_array( typeinfo->type, methodname); return;
   case _C_STRUCT_B : print_struct( typeinfo->type, methodname); return;
   case _C_UNION_B  : print_union( typeinfo->type, methodname); return;
   case _C_BFLD     : print_bitfield( typeinfo->type, methodname); return;
   }

   printf( "**unknown type signature**");
   return;
}


# pragma mark - +dependency dumper

static void  hackish_emit_dependencies( struct _mulle_objc_method *method,
                                        struct _mulle_objc_loadcategory *category)
{
   struct _mulle_objc_dependency   *dependencies;

   dependencies = (struct _mulle_objc_dependency *) (*method->value)( NULL, MULLE_OBJC_DEPENDENCIES_METHODID, NULL);
   if( ! dependencies)
      return;

   while( dependencies->classid != MULLE_OBJC_NO_CLASSID)
   {
      printf( "%08x;;%08x;\n",
         dependencies->classid,
         dependencies->categoryid);
      ++dependencies;
   }
}

# pragma mark - load info dumper

//
// Export Format: no escaping, no quoting!
//
// classid;classname;categoryid;categorynane;methodid;+/-;name;rvaltype;objtype;sel;params;variadic
//
// owner    :  either Foo or Foo( A), no quotes
// +/-      :  either + or -
// name     :  methodname like takeValue:forKey:
// rvaltype :  return value C/ObjC type without parantheses
// params   :  parameter C/ObjC types (if any) separated by comma
// variadic :  either ... or empty
//

static void   loadmethod_dump( struct _mulle_objc_method *method,
                               int type,
                               char *classname)
{
   char                         *types;
   struct mulle_objc_typeinfo   typeinfo;
   unsigned int                 i;

   printf( ";%08x", method->descriptor.methodid);
   printf( ";%c%s", type, method->descriptor.name);

   if( mode == dump_search || mode == dump_methods || mode == dump_callable_methods)
   {
      types = method->descriptor.signature;
      i     = 0;
      while( types = mulle_objc_signature_supply_next_typeinfo( types, &typeinfo))
      {
         putchar( (i <= first_param_index) ? ';' : ',');

         switch( i++)
         {
            case object_index :
               printf( "%s *", classname);
               continue;

            case sel_index    :
               printf( "SEL");
               continue;
         }
         print_typeinfo( &typeinfo, method->descriptor.name);
      }
      printf( method->descriptor.bits & _mulle_objc_method_variadic ? ";..." : ";");
      printf( ";0x%x", method->descriptor.bits);
   }

   printf( "\n");
}


static void   loadmethod_class_dump( struct _mulle_objc_method *method,
                                     int type,
                                     struct _mulle_objc_loadclass *p,
                                     struct _mulle_objc_loadinfo *info)
{
   printf( "%08x", p->classid);
   printf( ";%s",  p->classname);
   printf( ";");
   printf( ";");

   loadmethod_dump( method, type, p->classname);
   if( mode == dump_callable_methods || mode == dump_callable_coverage)
      if( type == '-' && p->superclassid == MULLE_OBJC_NO_CLASSID)
         loadmethod_class_dump( method, '+', p, info);
}


static void   loadmethod_category_dump( struct _mulle_objc_method *method,
                                        int type,
                                        struct _mulle_objc_loadcategory *p,
                                        struct _mulle_objc_loadinfo *info)
{
   if( mode == dump_loader)
   {
      if( method->descriptor.methodid != MULLE_OBJC_DEPENDENCIES_METHODID)
         return;

      hackish_emit_dependencies( method, p);
      return;
   }

   printf( "%08x",  p->classid);
   printf( ";%s",   p->classname);
   printf( ";%08x", p->categoryid);
   printf( ";%s",   p->categoryname);

   loadmethod_dump( method, type, p->classname);
}


static void   methodlist_loadclass_dump( struct _mulle_objc_methodlist *list,
                                         int type,
                                         struct _mulle_objc_loadclass *p,
                                         struct _mulle_objc_loadinfo *info)
{
   struct _mulle_objc_method   *method;
   struct _mulle_objc_method   *sentinel;

   if( ! list)
      return;

   method = list->methods;
   sentinel = &method[ list->n_methods];
   while( method < sentinel)
   {
      loadmethod_class_dump( method, type, p, info);
      ++method;
   }
}


static void   methodlist_loadcategory_dump( struct _mulle_objc_methodlist *list,
                                            int type,
                                            struct _mulle_objc_loadcategory *p,
                                            struct _mulle_objc_loadinfo *info)
{
   struct _mulle_objc_method   *method;
   struct _mulle_objc_method   *sentinel;

   if( ! list)
      return;

   method   = list->methods;
   sentinel = &method[ list->n_methods];
   while( method < sentinel)
   {
      loadmethod_category_dump( method, type, p, info);
      ++method;
   }
}


static void   loadclass_walk( struct _mulle_objc_loadclass *p,
                              struct _mulle_objc_loadinfo *info)
{
   log_printf( "Dumping class %s ...\n", p->classname);

   switch( mode)
   {
   case dump_classes_categories :
   case dump_search :
      printf( "%08x;%s\n", p->classid, p->classname);
      break;

   case dump_classes :
      printf( "%08x;%s;", p->classid, p->classname);
      if( p->superclassid == MULLE_OBJC_NO_CLASSID)
         printf( "MULLE_OBJC_NO_CLASSID;\n");
      else
         printf( "%08x;%s\n", p->superclassid, p->superclassname);
      break;

   case dump_dependencies :
      if( p->classid != loader_classid)
         printf( "      { @selector( %s), MULLE_OBJC_NO_CATEGORYID },"
                "      // %08x;%s;;\n",
                p->classname,
                p->classid,
                p->classname);
      break;
   }


   switch( mode)
   {
   case dump_callable_coverage :
   case dump_search :
   case dump_methods :
   case dump_coverage :
      methodlist_loadclass_dump( p->classmethods, '+', p, info);
      methodlist_loadclass_dump( p->instancemethods, '-', p, info);
      break;
   }
}


static void   loadcategory_walk( struct _mulle_objc_loadcategory *p,
                                 struct _mulle_objc_loadinfo *info)
{
   log_printf( "Dumping category %s( %s) ...\n", p->classname, p->categoryname);

   switch( mode)
   {
   case dump_search :
   case dump_classes_categories :
      printf( "%08x;%s;%08x;%s\n",
             p->classid, p->classname, p->categoryid, p->categoryname);
      break;

   case dump_dependencies :
      if( p->classid != loader_classid)
      {
         printf( "      { @selector( %s), @selector( %s) },"
                 "      // %08x;%s;%08x;%s\n",
                    p->classname,
                    p->categoryname,
                    p->classid,
                    p->classname,
                    p->categoryid,
                    p->categoryname);
      }
      break;

   case dump_loader :
      if( p->classid == loader_classid)
         methodlist_loadcategory_dump( p->classmethods, '+', p, info);
      break;
   }

   switch( mode)
   {
   case dump_methods  :
   case dump_search  :
   case dump_callable_coverage :
   case dump_coverage :
      methodlist_loadcategory_dump( p->classmethods, '+', p, info);
      methodlist_loadcategory_dump( p->instancemethods, '-', p, info);
      return;
   }
}


#pragma mark - list walkers

static void   loadclasslist_walk( struct _mulle_objc_loadclasslist *list,
                                  struct _mulle_objc_loadinfo *info)
{
   struct _mulle_objc_loadclass   **p;
   struct _mulle_objc_loadclass   **sentinel;

   if( ! list)
      return;

   p        = list->loadclasses;
   sentinel = &p[ list->n_loadclasses];
   while( p < sentinel)
      loadclass_walk( *p++, info);
}


static void   loadcategorylist_walk( struct _mulle_objc_loadcategorylist *list,
                                     struct _mulle_objc_loadinfo *info)
{
   struct _mulle_objc_loadcategory   **p;
   struct _mulle_objc_loadcategory   **sentinel;

   if( ! list)
      return;

   p        = list->loadcategories;
   sentinel = &p[ list->n_loadcategories];
   while( p < sentinel)
      loadcategory_walk( *p++, info);
}


int  __mulle_objc_list_callback( struct _mulle_objc_loadinfo *info)
{
   char   *s;

   if( dump)
   {
     // if empty, ignore
      if((! info->loadclasslist    || ! info->loadclasslist->n_loadclasses) &&
         (! info->loadcategorylist || ! info->loadcategorylist->n_loadcategories))
      {
         return(0);
      }

      s = mulle_objc_loadinfo_get_originator( info);
      if( s)
         log_printf( "Dumping loadinfo %s ...\n", s);

      switch( mode)
      {
      case dump_info :
         mulle_objc_loadinfo_csvdump_terse( info, stdout);
         return( 0);
      }

      loadclasslist_walk( info->loadclasslist, info);
      loadcategorylist_walk( info->loadcategorylist, info);
   }
}


int  __mulle_objc_loadinfo_callback( struct _mulle_objc_universe *universe,
                                     struct _mulle_objc_loadinfo *info)
{
   static int   done;

   if( verbose && ! done)
   {
      log_printf( "The loaded universe is %u.%u.%u at %p",
                     mulle_objc_version_get_major( _mulle_objc_universe_get_version( universe)),
                     mulle_objc_version_get_minor( _mulle_objc_universe_get_version( universe)),
                     mulle_objc_version_get_patch( _mulle_objc_universe_get_version( universe)),
                     universe);
      if( _mulle_objc_universe_get_path( universe))
         log_printf( " (%s)", _mulle_objc_universe_get_path( universe));
      log_printf( "\n");

      done = 1;
   }

   __mulle_objc_list_callback( info);

   return( 0); // don't add to universe
}


static char   *concat( char *a, char *b)
{
   size_t   len_a;
   size_t   len_b;
   char      *s;

   len_a = strlen( a);
   len_b = strlen( b);
   s = malloc(  len_a + len_b + 1);
   if( ! s)
      abort();
   memcpy( s, a, len_a);
   strcpy( &s[ len_a], b);
   return( s);
}


int  main( int argc, char *argv[])
{
   void   *handle;
   void   *adr;
   int    i;
   int    dlmode;
   char   *path;
   struct _mulle_objc_universe   *(*p_mulle_objc_get_universe)( void);
   struct _mulle_objc_universe   *universe;

#if defined( DEBUG) && defined( __MULLE_OBJC__)
   if( mulle_objc_check_runtime())
   {
      extern void   mulle_objc_dotdump( void);

      mulle_objc_dotdump();
   }
#endif

   if( argc < 2)
      usage();

   dlmode = RTLD_LOCAL;

   // this doesn't work
   // the dylib comes usually with its own global universe
   //
   // struct _mulle_objc_universe   *universe;
   // universe = __get_or_create_mulle_objc_runtime();
   // universe->callbacks.should_load_loadinfo = __mulle_objc_loadinfo_callback;
   // but
   /*
    * This method can fail in an interesting way.
    *
    * Though we load with RTLD_LOCAL, the dylib we load
    * probably contains it's own mulle-objc. So "our"
    * mulle_objc_loadinfo_unfailing_enqueue is not used.
    * If it is too outdated and does not support
    * should_load_loadinfo( at the same offset) stuff
    * goes really wrong!
    *
    * If the universe versioning is done properly this
    * should be caught though.
    *
    */

   for( i = 1; i < argc ; i++)
   {
      if( argv[ i][ 0] != '-')
         break;

      switch( argv[ i][ 1])
      {
      case '-' :
         switch( argv[ i][ 2])
         {
         case 'v' :
            fprintf( stderr, "%u.%u.%u\n",
                    (MULLE_OBJC_LIST_VERSION >> 20),
                    ((MULLE_OBJC_LIST_VERSION >> 8) & 0xFFF),
                    MULLE_OBJC_LIST_VERSION & 0xFF);
            return( 0);
         }

      // options
      case 'v':
         verbose = 1;
         break;

      case 'e':
         emit_sentinel = 1;
         break;

      // commands
      case 'c':
         mode = dump_classes_categories;
         break;

      case 'C':
         mode = dump_classes;
         break;

      case 'd':
         mode = dump_dependencies;
         break;

      case 'g' :
         dlmode = RTLD_GLOBAL;
         break;

      case 'i':
         mode = dump_info;
         break;

      case 'L':
         mode = dump_loader;
         break;

      case 'm':
         mode = dump_methods;
         break;

      case 'M':
         mode = dump_callable_methods;
         break;

      case 's':
         mode = dump_search;
         break;

      case 't':
         mode = dump_coverage;
         break;

      case 'T':
         mode = dump_callable_coverage;
         break;

      case 'l':
         if( i + 1 >= argc)
            usage();
         ++i;

         loader_classid = atoi( argv[ i]);
         break;

      default :
         usage();
      }
   }

   if( i >= argc)
      usage();


   // ensure linker doesn't strip it out
   mulle_objc_loadinfo_unfailingenqueue( NULL);

   dlmode |= RTLD_NOW;
   for(; i < argc ; i++)
   {
      if( i == argc - 1)  // dump the last one
         dump = 1;

      path = argv[ i];
      if( verbose)
         fprintf( stderr, "Loading \"%s\"...\n", argv[ i]);

      switch( argv[ i][ 0])
      {
      case '/' :
      case '.' :
         break;

      default :
         path = concat( "./", argv[ i]);
      }

      handle = dlopen( path, dlmode);
      if( ! handle)
      {
         char   *pwd;

         pwd = getenv( "PWD");
         fprintf( stderr, "mulle-objc-list error: failed to open \"%s\" (%s) %s\n",
                     argv[ i],
                     pwd ? pwd : "???",
                     dlerror());
         return( 1);
      }

      if( path != argv[ i])
         free( path);

      // get a global function
      universe = NULL;
      adr      = dlsym( handle, "mulle_objc_get_or_create_universe");
      if( adr)
      {
         p_mulle_objc_get_universe = adr;
         universe = (*p_mulle_objc_get_universe)();
      }

      if( verbose)
         fprintf( stderr, "Loaded \"%s\". (universe: %p, dlmode: %u)\n", argv[ i], universe, dlmode);
   }

   if( mode == dump_dependencies)
   {
      if( emit_sentinel)
         printf( "      { MULLE_OBJC_NO_CLASSID, MULLE_OBJC_NO_CATEGORYID }\n");
   }

   // get a global function
   if( verbose)
   {
      //
      // only use __mulle_objc_get_universe because we want an address
      // and don't access it otherwise
      //
      universe = __mulle_objc_get_universe();
      fprintf( stderr, "The mulle_objc_list universe is at %p\n", universe);
   }

   return( 0);
}


#ifndef STANDALONE_MULLE_OBJC

MULLE_C_CONST_RETURN  // always returns same value (in same thread)
struct _mulle_objc_universe  *__get_or_create_mulle_objc_universe( void)
{
   struct _mulle_objc_universe  *universe;

   universe = __mulle_objc_get_universe();
   if( _mulle_objc_universe_is_uninitialized( universe))
   {
      _mulle_objc_universe_bang( universe, 0, 0, NULL);
       universe->callbacks.should_load_loadinfo = __mulle_objc_loadinfo_callback;  // lazy but we are not multithreaded
   }
   return( universe);
}


# ifdef __APPLE__
//
// see: https://stackoverflow.com/questions/35998488/where-is-the-eprintf-symbol-defined-in-os-x-10-11/36010972#36010972
//
__attribute__((visibility("hidden")))
void __eprintf( const char* format, const char* file,
               unsigned line, const char *expr)
{
   fprintf( stderr, format, file, line, expr);
   abort();
}
# endif

#endif
