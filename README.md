# mulle-objc-list

#### 📒 Lists mulle-objc runtime information contained in executables.

The functionality is based on `mulle-objc-list` which loads a shared
library and dumps the mulle-objc load information contained within as
[CSV, euro-style](https://en.wikipedia.org/wiki/Comma-separated_values).

| Release Version
|-----------------------------------
| ![Mulle kybernetiK tag](https://img.shields.io/github/tag/mulle-objc/mulle-objc-list.svg) [![Build Status](https://github.com/mulle-objc/mulle-objc-list/workflows/CI/badge.svg?branch=release)](https://github.com/mulle-objc/mulle-objc-list/workflows)


Also contains:

Executable               | Description
-------------------------|------------------------
`mulle-objc-loader-tool` | Create "objc-loader.inc" files for MulleObjCLoader categories, to facilitate dependency management of statically linked libraries
`mulle-objc-lista`       | Compiles a static library into a shared library, then uses `mulle-objc-list` to dump load info
`mulle-objc-optimize`    | Separate code into ObjC linkable and C linkable according to coverage information
`mulle-objc-searchid`    | Search through libraries for the name of a classid or @selector
`mulle-objc-unarchive`   | Unpack Objective-C library as a first step to omptimizing it using coverage information


## mulle-objc-list

This is a [mulle-objc](https://github.com/mulle-objc) project.

This tool loads a shared library or executable and prints a CSV list of all
class and categories or methods it finds. The binary must have been
compiled with **mulle-clang**.


### Class/Category List

For each class or category a line is printed. The format is:

`classid;classname;categoryid;categoryname`

Index | Column name  | Description
------|--------------|---------------------------------
1     | classid      |  the id of the class, 8 hex characters
2     | classname    |  the name of the class
3     | categoryid   |  optional: the id of the category
4     | categoryname |  optional: the name of the category


### Method List

Each line represents one exported method. The format is:

`classid;classname;categoryid;categoryname;methodid;+/-name;variadic;bits;tpe`

> This has changed in 8.0.0


Index | Column name  | Description
------|--------------|---------------------------------
1     | classid      |  the id of the class, 8 hex characters
2     | classname    |  the name of the class
3     | categoryid   |  optional: the id of the category
4     | categoryname |  optional: the name of the category
5     | methodid     |  the id of the method
6     | name         |  methodname like `takeValue:forKey:`  with +/- prefix
7     | variadic     |  either `...` or empty
8     | bits         |  bits as hex value
9     | type         |  decoded type of method, separated by comma
                     |  (may contain ';')

The intended use is to produce test skeleton code from this information.


### Terse Method List

Each line represents one exported method. The format is:

`classid;classname;categoryid;categoryname;methodid;+/-name`

Index | Column name  | Description
------|--------------|---------------------------------
1     | classid      |  the id of the class, 8 hex characters
2     | classname    |  the name of the class
3     | categoryid   |  optional: the id of the category
4     | categoryname |  optional: the name of the category
5     | methodid     |  the id of the method
6     | name         |  methodname like `takeValue:forKey:` with +/- prefix


This is the same format as the runtime coverage output, if you remove the first
two columns from the output. Then subtracting the modified coverage from this
list, would show you all unused methods.


### Objects Info

Each line represents a linked `-o` file with classes or categories.

`path;runtime-version;foundation-version;user;optilevel;bits`


Index | Column name  | Description
------|--------------|---------------------------------
1     | path         | source of compiled .o file
2     | runtime      | version of the runtime the .o file was compiled
3     | foundation   | version of the foundation the .o file was compiled
4     | user         | user supplied version during compilation
5     | optilevel    | optimization level of the compiled .o file
6     | bits         | load info bits as hex


Can be useful to find mixed-in old .o files


## Usage

```
usage: mulle-objc-list [options] [command] [libraries] <binary>

   The binary is listed. The preceeding libraries are
   explicitly loaded but their contents aren't listed.
   Implicitly loaded libraries by binary are listed.

Options:
   -e      : emit dependencies sentinel field
   -l <id> : specify loader-class id for -d (default is 0x58bd58d3)   -v      : verbose

Commands:
   -c      : list classes and categories
   -d      : list classes and categories as +dependencies. Skips loaders
   -i      : dump loadinfo version information
   -m      : list methods (default)
   -M      : list also root -methods as +methods
   -t      : terse list methods (coverage like)
   -T      : terse list methods with root -methods also as +methods
```


## Required Library

  Name         | Build Status | Release Version
---------------|--------------|---------------------------------
[mulle-objc-runtime](//github.com/mulle-objc/mulle-objc-runtime) | [![Build Status](https://github.com/mulle-objc/mulle-objc-runtime/workflows/CI/badge.svg?branch=release)](https://travis-ci.org/mulle-objc/mulle-objc-runtime) | ![Mulle kybernetiK tag](https://img.shields.io/github/tag/mulle-objc/mulle-objc-runtime.svg) [![Build Status](https://github.com/mulle-objc/mulle-objc-runtime/workflows/CI/badge.svg?branch=release)](https://travis-ci.org/mulle-objc/mulle-objc-runtime)

## Add

Use [mulle-sde](//github.com/mulle-sde) to add mulle-objc-list to your project:

```
mulle-sde dependency add --c --github mulle-objc mulle-objc-list
```

## Install

See [mulle-objc-developer](//github.com/mulle-objc/mulle-objc-developer) for the preferred
way to install mulle-objc-list.


### mulle-sde

Use [mulle-sde](//github.com/mulle-sde) to build and install mulle-objc-list and all dependencies:

```
mulle-sde install --prefix /usr/local \
   https://github.com/mulle-objc/mulle-objc-list/archive/latest.tar.gz
```

### Manual Installation

Install the requirements:

Requirements                                                     | Description
-----------------------------------------------------------------|-----------------------
[mulle-objc-runtime](//github.com/mulle-objc/mulle-objc-runtime) | Objective-C runtime

Install into `/usr/local`:

```
mkdir build 2> /dev/null
(
   cd build ;
   cmake -DCMAKE_INSTALL_PREFIX=/usr/local \
         -DCMAKE_PREFIX_PATH=/usr/local \
         -DCMAKE_BUILD_TYPE=Release .. ;
   make install
)
```

> #### musl incompatibility
>
> With musl static linking, mulle-objc-list lacks the capability to load
> shared libraries, rendering it useless.
>


## Author

[Nat!](//www.mulle-kybernetik.com/weblog) for
[Mulle kybernetiK](//www.mulle-kybernetik.com) and
[Codeon GmbH](//www.codeon.de)
