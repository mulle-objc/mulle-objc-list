## 0.22.0



* adapt to changes in mulle-objc-runtime


### 0.21.2

* fix for cosmopolitan

### 0.21.1

* fix output to long for 16 bit systems

## 0.21.0

* rewrote optimizing scripts


### 0.20.2

* Various small improvements

### 0.20.1

* don't install mulle-objc-list binaries, when musl based
* various changes to support Windows

## 0.20.0

* Various small improvements


## 0.19.0

* Various small improvements


## 0.18.0

* adapt to changes in mulle-objc-runtime


### 0.17.1

* new mulle-sde project structure

## 0.17.0

* adjust to new signature enumeration in mulle-objc-runtime
* now supports ``_C_BOOL`` as BOOL (not `_Bool\!)`
* remove some compile ugliness


## 0.16.0

* Various small improvements


## 0.15.0

* is aware of withZone suffix now for NSZone
* new tool mulle-objc-encode
* massive changes in the way signatures are interpreted and dumped
* added method filter option
* -M root methods flag renamed to -R, as -M is used by the method filter now
* -C is now the class filter, to match -M in capitalization
* dump properties and ivars too now
* tools now search for sibling executables before searching PATH


### 0.14.1

* modernized mulle-sde with .mulle folder

## 0.14.0

* Various small improvements


## 0.13.0

* migrated to mulle-sde


### 0.11.3

* try to fix stuff for linux and windows

## 0.9.1

* various little fixes, especially for linux
* adapt to runtime changes
* do not emit MulleObjCLoader classes and categories when dumping dependencies
* emit classid,categorid as comment when dumping dependencies
* package gains script mulle-objc-create-dependencies-inc
* improved error output
* new +M dump method for the optimizer
* new experimental scripts mulle-objc-unarchive and mulle-objc-optimize

### 0.8.3

* modernize project

### 0.8.2

* community release
