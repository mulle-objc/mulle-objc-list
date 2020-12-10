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
#pragma clang diagnostic ignored "-Wparentheses"
#pragma clang diagnostic ignored "-Wswitch"

// we don't really load any code, so this is
// the minimal setup

#define __MULLE_OBJC_NO_TPS__ 1
#define __MULLE_OBJC_NO_FCS__ 1

#include <mulle-objc-runtime/mulle-objc-runtime.h>

#include "include-private.h"

// direct load instead of dependencies.h because its a debug header
#include <mulle-objc-runtime/mulle-objc-csvdump.h>

#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>

#include "mulle-objc-list-version.h"


#define MULLE_OBJC_LOADER_CLASSID  0x47a9beb6

// #define STANDALONE_MULLE_OBJC

#define _stringify( x) #x
#define stringify( x) _stringify( x)


static int       verbose;
static int       dump;
static int       emit_sentinel;
static int       ignore_empty_args;
static int       raw_types;
static uint32_t  loader_classid = MULLE_OBJC_LOADER_CLASSID;


static mulle_objc_classid_t      filter_classid;
static mulle_objc_categoryid_t   filter_categoryid;
static mulle_objc_methodid_t     filter_methodid;


static enum
{
   dump_callable_coverage,
   dump_callable_methods,
   dump_classes,
   dump_classes_categories,
   dump_coverage,  // coverage style
   dump_dependencies,
   dump_info,
   dump_ivars,
   dump_loader,
   dump_methods,
   dump_properties,
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


static void   _usage( void)
{
   fprintf( stderr,
            "Options:\n"
            "   -a      : ignore empty filter arguments"
            "   -e      : emit dependencies sentinel field\n"
            "   -l <id> : specify loader-class id for -d (default is 0x%08x)\n"
            "   -f <id> : filter for class with id\n"
            "   -F <id> : filter for category with id (specify -f as well)\n"
            "   -M <id> : filter for methods with id\n"
            "   -r      : output raw types and signature strings\n"
            "   -v      : verbose\n"
            "\n"
            "Commands:\n"
            "   -C      : list classes with their superclass\n"
            "   -c      : list classes and categories\n"
            "   -d      : list classes and categories as +dependencies. Skips MulleObjCLoaders\n"
            "   -L      : list MulleObjCLoader dependencies\n"
            "   -R      : list also root -methods as +methods\n"
            "   -T      : terse list methods with root -methods as +methods\n"
            "   -i      : dump loadinfo version information\n"
            "   -m      : list methods (default)\n"
            "   -p      : list properties (default)\n"
            "   -I      : list instance variables (default)\n"
            "   -s      : list classes, categories, methods (mixed CSV)\n"
            "   -t      : terse list methods (coverage like)\n"
            "\n"
            , loader_classid);
   exit( 1);
}


static void   usage( void)
{
   fprintf( stderr,
            "usage: mulle-objc-list [options] [command] [libraries] <binary>\n"
            "\n"
            "   Lists Objective-C symbols of a binary. Preceeding libraries are\n"
            "   explicitly loaded but their contents aren't listed.\n"
            "   Libraries implicitly loaded by the binary are listed.\n"
            "\n");
   _usage();
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
   case _C_FLT       : return( "float");
   case _C_DBL       : return( "double");
   case _C_LNG_DBL   : return( "long double");
   case _C_CHARPTR   : return( "char *");
   case _C_BOOL      : return( "BOOL");
#ifdef _C_UNDEF
   case _C_UNDEF     : return( "undefined");
#endif
#ifdef _C_ATOM
   case _C_ATOM      : return( "char *");
#endif
   }
   return( 0);
}


static void   print_typeinfo( struct mulle_objc_typeinfo *typeinfo,
                              char *methodname,
                              char *fragment);


enum translated
{
   isUnknown = 0,
   isBOOL,
   isUnichar,
   isNSStringEncoding,
   isNSUInteger,
   isNSInteger,
   isNSRange,
   isNSZone,
   isBOOLPointer,
   isUnicharPointer,
   isCString
};

enum verbtype
{
   isName = 0,
   isPrefix,
   isSuffix
};

struct verbtable
{
   char              *verb;
   enum verbtype      type;
   enum translated   translation;
};


static enum translated  verbtable_match( struct verbtable *p_s, char *fragment)
{
   size_t   len;
   size_t   fragment_len;

   if( ! fragment)
      return( isUnknown);

   while( *fragment == '_')
      ++fragment;
   fragment_len = strlen( fragment);

   for( ;p_s->verb; p_s++)
   {
      len = strlen( p_s->verb);
      if( fragment_len < len)
         continue;
      if( p_s->type == isName && len != fragment_len)
         continue;
      if( p_s->type == isSuffix)
      {
         if( strncmp( p_s->verb, &fragment[ fragment_len - len], len))
            continue;
      }
      else
         if( strncmp( p_s->verb, fragment, len))
            continue;

      if( p_s->type != isPrefix || isupper( fragment[ len]))
         return( p_s->translation);
   }
   return( isUnknown);
}



static enum translated   translate_integral( char *fragment)
{
   // TODO: promote verb matching to regexp sometime
   static struct verbtable   verbs[] =
   {
      { "is",              isPrefix, isBOOL },
      { "has",             isPrefix, isBOOL },
      { "can",             isPrefix, isBOOL },
      { "allows",          isPrefix, isBOOL },
      { "contains",        isPrefix, isBOOL },
      { "conforms",        isPrefix, isBOOL },
      { "generates",       isPrefix, isBOOL },
      { "copyItems",       isName,   isBOOL },
      { "freeWhenDone",    isName,   isBOOL },
      { "encoding",        isName,   isNSStringEncoding },
      { "length",          isName,   isNSUInteger },
      { "hash",            isName,   isNSUInteger },
      { "count",           isName,   isNSUInteger },
      { "options",         isName,   isNSUInteger },
      { "Index",           isSuffix, isNSUInteger },
      { "StringLength",    isSuffix, isNSUInteger },
      { "getOwnedObjects", isName,   isNSUInteger },
      { "getObjects",      isName,   isNSUInteger },
      { 0, 0, 0 }
   };

   return( verbtable_match( verbs, fragment));
}


static void   print_int( char *type, char *methodname, char *fragment)
{
   switch( translate_integral( fragment ? fragment : methodname))
   {
      case isNSUInteger       : printf( "NSUInteger"); return;
      case isNSInteger        : printf( "NSInteger"); return;
      case isNSStringEncoding : printf( "NSStringEncoding"); return;
      case isBOOL             : printf( "BOOL"); return;
   }
   printf( "int");
}


static void   print_unsigned_int( char *type, char *methodname, char *fragment)
{
   switch( translate_integral( fragment ? fragment : methodname))
   {
      case isNSUInteger       : printf( "NSUInteger"); return;
      case isNSStringEncoding : printf( "NSStringEncoding"); return;
   }
   printf( "unsigned int");
}


static void   print_long( char *type, char *methodname, char *fragment)
{
   switch( translate_integral( fragment ? fragment : methodname))
   {
      case isNSInteger        : printf( "NSInteger"); return;
      case isNSStringEncoding : printf( "NSStringEncoding"); return;
   }
   printf( "long");
}


static void   print_unsigned_long( char *type, char *methodname, char *fragment)
{
   switch( translate_integral( fragment ? fragment : methodname))
   {
      case isNSUInteger       : printf( "NSUInteger"); return;
      case isNSStringEncoding : printf( "NSStringEncoding"); return;
   }
   printf( "unsigned long");
}


static void   print_long_long( char *type, char *methodname, char *fragment)
{
   switch( translate_integral( fragment ? fragment : methodname))
   {
      case isNSInteger        : printf( "NSInteger"); return;
      case isNSStringEncoding : printf( "NSStringEncoding"); return;
   }
   printf( "long long");
}


static void   print_unsigned_long_long( char *type, char *methodname, char *fragment)
{
   switch( translate_integral( fragment ? fragment : methodname))
   {
      case isNSUInteger       : printf( "NSUInteger"); return;
      case isNSStringEncoding : printf( "NSStringEncoding"); return;
   }
   printf( "unsigned long long");
}


//
// typedefs are resolved to struct
//
static void   _print_struct_or_union( char *type, char *prefix)
{
   struct mulle_objc_typeinfo              typeinfo;
   char                                    *element;
   unsigned int                            i;
   struct mulle_objc_signatureenumerator   rover;

   element = strchr( type, '=');
   if( ! element)
   {
      printf( "**malformed struct signature \"%s\" **", type);
      return;
   }

   i = 0;
   if( *type != '?')
   {
      printf( "%s %.*s", prefix, (int) (element - type), type);
      return;
   }

   printf( "%s { ", prefix);

   errno = 0;
   i     = 0;
   type  = &element[ 1];

   rover = mulle_objc_signature_enumerate( type);
   while( _mulle_objc_signatureenumerator_next( &rover, &typeinfo))
   {
      print_typeinfo( &typeinfo, NULL, NULL);
      printf( " f%d; ", i++);
      type = _mulle_objc_signatureenumerator_get_type( &rover);
      if( *type == '}')
         break;
   }
   mulle_objc_signatureenumerator_done( &rover);

   if( errno == EINVAL)
      printf( "** broken struct/union signature \"%s\" **", type);

   printf( "}");
}


static enum translated   translate_struct( char *fragment)
{
   // TODO: promote verb matching to regexp sometime
   static struct verbtable   verbs[] =
   {
      { "rangeOf",  isPrefix,  isNSRange },
      { "range",    isName,    isNSRange },
      { "Range",    isSuffix,  isNSRange },
      { "withZone", isSuffix,  isNSZone },
      { 0, 0, 0 }
   };

   return( verbtable_match( verbs, fragment));
}


static void   print_struct( char *type, char *methodname, char *fragment)
{
   switch( translate_struct( fragment ? fragment : methodname))
   {
      case isNSRange : printf( "NSRange"); return;
      case isNSZone  : printf( "NSZone"); return;
   }

   _print_struct_or_union( ++type, "struct");
}


static void   print_union( char *type, char *methodname, char *fragment)
{
   _print_struct_or_union( ++type, "union");
}


static enum translated   translate_int_pointer( char *fragment)
{
   // TODO: promote verb matching to regexp sometime
   static struct verbtable   verbs[] =
   {
      { "is",               isPrefix, isBOOLPointer    },
      { "CharactersNoCopy", isSuffix, isUnicharPointer },
      { "Characters",       isSuffix, isUnicharPointer },
      { 0, 0, 0 }
   };

   return( verbtable_match( verbs, fragment));
}


static void   print_pointer( char *type, char *methodname, char *fragment)
{
   struct mulle_objc_typeinfo   typeinfo;

   if( mulle_objc_signature_supply_typeinfo( ++type, NULL, &typeinfo))
   {
      // fprintf( stderr, ">>> %s: %.1s\n",  fragment ? fragment : methodname, typeinfo.type);
      switch( *typeinfo.type)
      {
      case 'i' :
         switch( translate_int_pointer( fragment ? fragment : methodname))
         {
            case isBOOLPointer    : printf( "BOOL *"); return;
            case isUnicharPointer : printf( "unichar *"); return;
         }
      }

      print_typeinfo( &typeinfo, NULL, NULL);
   }
   else
      printf( "** broken pointer signature \"%s\" **", type);
   printf( " *");  // hmm
}


static void   print_array( char *type, char *methodname, char *fragment)
{
   struct mulle_objc_typeinfo   typeinfo;
   int    size = 0;

   while( isdigit( type[ 1]))
   {
      ++type;
      size *= 10;
      size += *type - '0';
   }

   if( mulle_objc_signature_supply_typeinfo( ++type, NULL, &typeinfo))
      print_typeinfo( &typeinfo, NULL, NULL);
   else
      printf( "** broken array signature \"%s\" **", type);
   if( size == 0)
      printf( " []");  // hmm
   else
      printf( " [%d]", size);  // hmm
}


static void   print_bitfield( char *type, char *methodname, char *fragment)
{
   printf( "**bitfield as parameter, O RLY ?**");
   return;
}


static void   print_typeinfo( struct mulle_objc_typeinfo *typeinfo,
                              char *methodname,
                              char *fragment)
{
   char  *s;
   char  *end;

   if( typeinfo->name)
   {
      s   = &typeinfo->name[ 1];
      end = strchr( s, '"');
      if( ! end || ! *s)
      {
         printf( "**%s named signature \"%s\" **", s ? "malformed" : "NULL", s ? s : "");
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
   case _C_INT      : print_int( typeinfo->type, methodname, fragment); return;
   case _C_UINT     : print_unsigned_int( typeinfo->type, methodname, fragment); return;
   case _C_LNG      : print_long( typeinfo->type, methodname, fragment); return;
   case _C_ULNG     : print_unsigned_long( typeinfo->type, methodname, fragment); return;
   case _C_LNG_LNG  : print_long_long( typeinfo->type, methodname, fragment); return;
   case _C_ULNG_LNG : print_unsigned_long_long( typeinfo->type, methodname, fragment); return;
   case _C_PTR      : print_pointer( typeinfo->type, methodname, fragment); return;
   case _C_ARY_B    : print_array( typeinfo->type, methodname, fragment); return;
   case _C_STRUCT_B : print_struct( typeinfo->type, methodname, fragment); return;
   case _C_UNION_B  : print_union( typeinfo->type, methodname, fragment); return;
   case _C_BFLD     : print_bitfield( typeinfo->type, methodname, fragment); return;
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

static void   method_dump( struct _mulle_objc_method *method,
                           int type,
                           char *classname)
{
   char                         *types;
   struct mulle_objc_typeinfo   typeinfo;
   unsigned int                 i;
   char                         *methodname;
   char                         *fragment;
   char                         *tokenizer;

   printf( ";%08x", _mulle_objc_method_get_methodid( method));
   printf( ";%c%s", type, _mulle_objc_method_get_name( method));

   if( mode != dump_search && mode != dump_methods && mode != dump_callable_methods)
      return;

   if( raw_types)
   {
      printf( ";%s", _mulle_objc_method_get_signature( method));
      printf( ";0x%x", _mulle_objc_method_get_bits( method));
      return;
   }

   /*
    * emit bits first, so that types can contain ';' for CSV parsing
    */
   printf( method->descriptor.bits & _mulle_objc_method_variadic ? ";..." : ";");
   printf( ";0x%x", method->descriptor.bits);

   methodname =_mulle_objc_method_get_name( method);
   tokenizer  = mulle_strdup( methodname);

   types = _mulle_objc_method_get_signature( method);
   i     = 0;

   while( types = mulle_objc_signature_supply_typeinfo( types, NULL, &typeinfo))
   {
      putchar( ! i ? ';' : ',');

      switch( i)
      {
         case rval_index :
            print_typeinfo( &typeinfo, methodname, NULL);
            break;

         case object_index :
            printf( "%s *", classname);
            break;

         case sel_index    :
            printf( "SEL");
            break;

         case first_param_index    :
            fragment = strtok( tokenizer, ":");
            print_typeinfo( &typeinfo, methodname, fragment);
            break;

         default :
            fragment = strtok( NULL, ":");
            print_typeinfo( &typeinfo, methodname, fragment);
            break;
      }
      ++i;
   }
   mulle_free( tokenizer);
}



static void   method_loadclass_dump( struct _mulle_objc_method *method,
                                     int type,
                                     struct _mulle_objc_loadclass *p,
                                     struct _mulle_objc_loadinfo *info)
{
   if( filter_methodid && method->descriptor.methodid != filter_methodid)
      return;

   printf( "%08x", p->classid);
   printf( ";%s",  p->classname);
   printf( ";");
   printf( ";");

   method_dump( method, type, p->classname);
   if( mode == dump_callable_methods || mode == dump_callable_coverage)
      if( type == '-' && p->superclassid == MULLE_OBJC_NO_CLASSID)
         method_loadclass_dump( method, '+', p, info);
   printf( "\n");
}


static void   method_loadcategory_dump( struct _mulle_objc_method *method,
                                        int type,
                                        struct _mulle_objc_loadcategory *p,
                                        struct _mulle_objc_loadinfo *info)
{
   if( filter_methodid && method->descriptor.methodid != filter_methodid)
      return;

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

   method_dump( method, type, p->classname);
   printf( "\n");
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
      method_loadclass_dump( method, type, p, info);
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
      method_loadcategory_dump( method, type, p, info);
      ++method;
   }
}


/*
 *
 */
static void   property_dump( struct _mulle_objc_property *property)
{
   printf( ";%08x", _mulle_objc_property_get_propertyid( property));
   printf( ";%s", _mulle_objc_property_get_name( property));
   printf( ";%s", _mulle_objc_property_get_signature( property));
}


static void   property_loadclass_dump( struct _mulle_objc_property *property,
                                       struct _mulle_objc_loadclass *p)
{
   printf( "%08x", p->classid);
   printf( ";%s",  p->classname);
   property_dump( property);
   printf( "\n");
}


static void   property_loadcategory_dump( struct _mulle_objc_property *property,
                                          struct _mulle_objc_loadcategory *p)
{
   printf( "%08x",  p->classid);
   printf( ";%s",   p->classname);
   printf( ";%08x", p->categoryid);
   printf( ";%s",   p->categoryname);
   property_dump( property);
   printf( "\n");
}


static void   propertylist_loadclass_dump( struct _mulle_objc_propertylist *list,
                                           struct _mulle_objc_loadclass *p,
                                           struct _mulle_objc_loadinfo *info)
{
   struct _mulle_objc_property   *property;
   struct _mulle_objc_property   *sentinel;

   if( ! list)
      return;

   property = list->properties;
   sentinel = &property[ list->n_properties];
   while( property < sentinel)
   {
      property_loadclass_dump( property, p);
      ++property;
   }
}


static void   propertylist_loadcategory_dump( struct _mulle_objc_propertylist *list,
                                              struct _mulle_objc_loadcategory *p,
                                              struct _mulle_objc_loadinfo *info)
{
   struct _mulle_objc_property   *property;
   struct _mulle_objc_property   *sentinel;

   if( ! list)
      return;

   property = list->properties;
   sentinel = &property[ list->n_properties];
   while( property < sentinel)
   {
      property_loadcategory_dump( property, p);
      ++property;
   }
}


/*
 *
 */
static void   ivar_dump( struct _mulle_objc_ivar *ivar)
{
   char                         *type;
   struct mulle_objc_typeinfo   typeinfo;

   printf( ";%08x", _mulle_objc_ivar_get_ivarid( ivar));
   printf( ";%s", _mulle_objc_ivar_get_name( ivar));
   printf( ";%ld", (long) _mulle_objc_ivar_get_offset( ivar));

   if( raw_types)
   {
      printf( ";%s", _mulle_objc_ivar_get_signature( ivar));
      return;
   }

   putchar(';');
   type = _mulle_objc_ivar_get_signature( ivar);
   if( mulle_objc_signature_supply_typeinfo( type, NULL, &typeinfo))
      print_typeinfo( &typeinfo, _mulle_objc_ivar_get_name( ivar), _mulle_objc_ivar_get_name( ivar));
   else
      printf( "** broken ivar signature \"%s\" **", type);
}


static void   ivar_loadclass_dump( struct _mulle_objc_ivar *ivar,
                                   struct _mulle_objc_loadclass *p)
{
   printf( "%08x", p->classid);
   printf( ";%s",  p->classname);

   ivar_dump( ivar);

   printf( "\n");
}


static void   ivarlist_loadclass_dump( struct _mulle_objc_ivarlist *list,
                                       struct _mulle_objc_loadclass *p,
                                       struct _mulle_objc_loadinfo *info)
{
   struct _mulle_objc_ivar   *ivar;
   struct _mulle_objc_ivar   *sentinel;

   if( ! list)
      return;

   ivar     = list->ivars;
   sentinel = &ivar[ list->n_ivars];
   while( ivar < sentinel)
   {
      ivar_loadclass_dump( ivar, p);
      ++ivar;
   }
}


/*
 *
 */
static void   loadclass_walk( struct _mulle_objc_loadclass *p,
                              struct _mulle_objc_loadinfo *info)
{
   if( filter_classid && p->classid != filter_classid)
      return;

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
   case dump_properties :
      propertylist_loadclass_dump( p->properties, p, info);
      break;

   case dump_ivars :
      ivarlist_loadclass_dump( p->instancevariables, p, info);
      break;

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
   if( filter_categoryid && p->categoryid != filter_categoryid)
      return;
   if( filter_classid && p->classid != filter_classid)
      return;

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
   case dump_properties :
      propertylist_loadcategory_dump( p->properties, p, info);
      break;

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
         mulle_objc_loadinfo_csvdump_terse_to_fp( info, stdout);
         return( 0);
      }

      loadclasslist_walk( info->loadclasslist, info);
      loadcategorylist_walk( info->loadcategorylist, info);
   }
   return(0);
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
   char     *s;

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
   void                          *handle;
   void                          *adr;
   int                           i;
   int                           dlmode;
   char                          *path;
   struct _mulle_objc_universe   *(*p_mulle_objc_get_universe)( mulle_objc_universeid_t, char *);
   struct _mulle_objc_universe   *universe;
   char                          *universename;
   mulle_objc_universeid_t       universeid;

   universeid   = MULLE_OBJC_DEFAULTUNIVERSEID;
   universename = NULL;

#if defined( DEBUG) && defined( __MULLE_OBJC__)
   if( mulle_objc_global_check_universe( __MULLE_OBJC_UNIVERSENAME__) != mulle_objc_universe_is_ok)
   {
      extern void   mulle_objc_dotdump( void);

      universe = __mulle_objc_global_get_universe( __MULLE_OBJC_UNIVERSEID__);
      mulle_objc_class_dotdump_to_directory( universe, ".");
      abort();
   }
#endif

   if( argc < 2)
      usage();

   dlmode = RTLD_LOCAL;

   // this doesn't work
   // the dylib comes usually with its own global universe
   //
   // struct _mulle_objc_universe   *universe;
   // universe = __register_mulle_objc_universe();
   // universe->callbacks.should_load_loadinfo = __mulle_objc_loadinfo_callback;
   // but
   /*
    * This method can fail in an interesting way.
    *
    * Though we load with RTLD_LOCAL, the dylib we load
    * probably contains it's own mulle-objc. So "our"
    * mulle_objc_loadinfo_enqueue_nofail is not used.
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
     case 'a':
         ignore_empty_args = 1;
         break;

      case 'e':
         emit_sentinel = 1;
         break;

      case 'f':
         if( i + 1 >= argc)
            usage();
         ++i;

         filter_classid = strtol( argv[ i], NULL, 16);
         if( ! filter_classid && ! ignore_empty_args)
         {
            fprintf( stderr, "Could not parse \"%s\" as classid\n", argv[ i]);
            exit( 1);
         }
         break;

      case 'F':
         if( i + 1 >= argc)
            usage();
         ++i;

         filter_categoryid = strtol( argv[ i], NULL, 16);
         if( ! filter_categoryid && ! ignore_empty_args)
         {
            fprintf( stderr, "Could not parse \"%s\" as categoryid\n", argv[ i]);
            exit( 1);
         }
         break;

      case 'M':
         if( i + 1 >= argc)
            usage();
         ++i;

         filter_methodid = strtol( argv[ i], NULL, 16);
         if( ! filter_methodid && ! ignore_empty_args)
         {
            fprintf( stderr, "Could not parse \"%s\" as methodid\n", argv[ i]);
            exit( 1);
         }
         break;

      case 'l':
         if( i + 1 >= argc)
            usage();
         ++i;

         loader_classid = atoi( argv[ i]);
         break;

      case 'r':
         raw_types = 1;
         break;

      case 'u' :
         if( i + 1 >= argc)
            usage();
         ++i;

         universename = argv[ i];
         universeid   = mulle_objc_universeid_from_string( universename);
         break;

      case 'v':
         verbose = 1;
         break;

      case '@' :
         if( i + 1 >= argc)
            usage();
         ++i;

         {
            struct mulle_objc_typeinfo   typeinfo;

            if( mulle_objc_signature_supply_typeinfo( argv[ i], NULL, &typeinfo))
            {
               print_typeinfo( &typeinfo, NULL, NULL);
               return( 0);
            }
         }
         return( 1);

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

      case 'I':
         mode = dump_ivars;
         break;

      case 'L':
         mode = dump_loader;
         break;

      case 'm':
         mode = dump_methods;
         break;

      case 'R':
         mode = dump_callable_methods;
         break;

      case 'p':
         mode = dump_properties;
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

      case 'U' :
         _usage();
         exit( 1);
         break;

      default :
         fprintf( stderr, "Unknown command or option \"%s\"\n", argv[ i]);
         usage();
      }
   }

   if( i >= argc)
      usage();


   // ensure linker doesn't strip it out
   mulle_objc_loadinfo_enqueue_nofail( NULL);

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

      // get a global function
      universe = NULL;
      adr      = dlsym( handle, "mulle_objc_global_register_universe");
      if( adr)
      {
         p_mulle_objc_get_universe = adr;
         universe = (*p_mulle_objc_get_universe)( universeid, universename);
      }
      else
         if( verbose)
            fprintf( stderr, "Did not find \"mulle_objc_global_register_universe\" in \"%s\"\n", path);

      if( path != argv[ i])
         free( path);

      if( verbose && universe)
         fprintf( stderr, "Loaded \"%s\". (universe: \"%s\" %x (%p), dlmode: %u)\n",
               argv[ i],
               universe->universename ? universe->universename : "*default*",
               universe ? universe->universeid : 0,
               universe,
               dlmode);
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
      // only use __mulle_objc_global_get_universe because we want an address
      // and don't access it otherwise
      //
      universe = __mulle_objc_global_get_universe( universeid, universename);
      fprintf( stderr, "The mulle-objc-list universe is at %p\n", universe);
   }

   return( 0);
}


#ifndef STANDALONE_MULLE_OBJC

MULLE_C_CONST_RETURN  // always returns same value (in same thread)
struct _mulle_objc_universe  *
   __register_mulle_objc_universe( mulle_objc_universeid_t universeid,
                                   char *universename)
{
   struct _mulle_objc_universe  *universe;

   universe = __mulle_objc_global_get_universe( universeid, universename);
   if( _mulle_objc_universe_is_uninitialized( universe))
   {
      _mulle_objc_universe_bang( universe, 0, NULL, NULL);
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
