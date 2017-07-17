
# A collection of tools to inspect Objective-C object files

The functionality is based on `mulle-objc-list` which loads a shared
library and dumps the mulle-objc load information contained within as
[CSV, euro-style](https://en.wikipedia.org/wiki/Comma-separated_values).

Also contains:

Executable              | Description
------------------------|------------------------
* mulle-objc-create-dependencies-inc | Create "dependencies.inc" files for MulleObjCLoader categories, to facilitate dependency management of statically linked libraries
* mulle-objc-lista      | Compiles a static library into a shared library, then uses `mulle-objc-list` to dump load info
* mulle-objc-optimize   | Separate code into ObjC linkable and C linkable according to coverage information
* mulle-objc-searchid   | Search through libraries for the name of a classid or @selector
* mulle-objc-unarchive  | Unpack Objective-C library as a first step to omptimizing it using coverage information


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

`classid;classname;categoryid;categoryname;methodid;+/-name;rvaltype;objtype;sel;params;variadic;bits`

Index | Column name  | Description
------|--------------|---------------------------------
1     | classid      |  the id of the class, 8 hex characters
2     | classname    |  the name of the class
3     | categoryid   |  optional: the id of the category
4     | categoryname |  optional: the name of the category
5     | methodid     |  the id of the method
6     | name         |  methodname like `takeValue:forKey:`
7     | rvaltype     |  return value C/ObjC type without parantheses
8     | objctype     |  "classname" as a pointer
9     | sel          |  always ':', the selector
10    | params       |  parameter C/ObjC types (if any) separated by comma
11    | variadic     |  either `...` or empty
12    | bits         |  bits as hex value

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


## Install Prequisites

Install [mulle-objc-developer](https://github.com/mulle-obj/mulle-objc-developer).


### Use **mulle-build** to build the project

```
mulle-build --bootstrap
```
