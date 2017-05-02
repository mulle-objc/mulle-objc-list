# mulle-objc-method-list

This is a [mulle-objc](https://github.com/mulle-nat) project.

This tool loads a shared library and prints a CSV list of all
class and categories or methods it finds. The shared library must have been
compiled with
**mulle-clang**.


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

`classid;classname;categoryid;categoryname;+/-;name;rvaltype;objtype;sel;params;variadic;bits`

Index | Column name  | Description
------|--------------|---------------------------------
1     | classid      |  the id of the class, 8 hex characters
2     | classname    |  the name of the class
3     | categoryid   |  optional: the id of the category
4     | categoryname |  optional: the name of the category
5     | +/-          |  either `+` or `-`
6     | name         |  methodname like `takeValue:forKey:`
7     | rvaltype     |  return value C/ObjC type without parantheses
8     | params       |  parameter C/ObjC types (if any) separated by comma
9     | variadic     |  either `...` or empty
10    | bits         |  bits as hex value

The intended use is to produce test skeleton code from this information.


## Usage

```
usage: mulle-objc-list [option] <libraries>

   The last library is listed. The preceeding libraries are
   loaded to satisfy the linker.

Options:
   -c : list classes and categories
   -m : list methods (default)
```


## Install Prequisites

Install
[mulle-objc-developer](https://github.com/mulle-nat/mulle-objc-developer).

This will give your the **mulle-clang** compiler and the **mulle-objc**
libraries that are necessary to build this project as well as
[mulle-bootstrap](https://github.com/mulle-nat/mulle-bootstrap)
to manage the projects dependencies.


### Install Prequisites on OS X:

Get [homebrew](https://brew.sh) then

```
brew install mulle-kybernetik/software/mulle-objc-developer
mulle-bootstrap
```


### Use **make** to build project

```
mkdir build.d
cd build.d
cmake -G "Unix Makefiles" ..
make
```

### Use Xcode to build project

```
mkdir build-xcode.d
cd build-xcode.d
cmake -G "Xcode" ..
xcodebuild
```

### Use Visual Studio to build project

```
mkdir build-vs.d
cd build-vs.d
cmake -G "Visual Studio 12" ..
msbuild
```

## Avoiding duplicate compiler installs

If you already have the **mulle-clang** compiler installed, you should edit
the file `.bootstrap/repositories` and change the line

`${MULLE_OBJC_REPOSITORIES}/mulle-objc-developer;;${MULLE_OBJC_DEVELOPER_BRANCH:-release}`

to

`${MULLE_OBJC_REPOSITORIES}/MulleObjC;;${MULLE_OBJC_BRANCH:-release}`

## Installing the compiler system-wide

Check the [mulle-clang](https://github.com/codeon-gmbh/mulle-clang) homepage
how to install **mulle-clang** on your system.

> On OS X you can install the compiler system-wide with
> `brew install codeon-gmbh/software/mulle-clang`

