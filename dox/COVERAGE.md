# Generating coverage files is easy

Assume that you have a **mulle-objc** executable **myexe** is a that links
against `libMulleObjC.a`. Coverage files are generated wqhen you run myexe
with the environment variable  **MULLE_OBJC_COVERAGE** set to YES:

```
MULLE_OBJC_COVERAGE=YES myexe
```

This will produce two coverage files `class-coverage.csv` and
`method-coverage.csv`.

> Note: Your executable needs to properly terminate, to generate
> coverage information. If that isn't possible,
> call `mulle_objc_csvdump_methodcoverage()` and
> `mulle_objc_csvdump_classcoverage()` yourself, when you know the runtime
> system is quiescent.


## Generating complete coverage

But generating good coverage files is kinda tricky, because you should hit
all execution paths of your executable. Well isn't that what testing is for ?
Maybe so, but tests with 100% coverage are rare.

Things to remember:

0. A second "coverage" run will append to previously existing coverage files.
1. You can easily concatenate coverage files from various source with `cat`.
Collect coverage files from your testers and concatenate them.
Concatenated coverage files can become fairly big (several MBs), but it
shouldn't be a problem. It's fine to `sort -u` them.
2. If you are implementing your own class clusters, you should cover all
possible  subclasses. (That's done automatically for you for NSString and
NSNumber)
3. Removing libraries from the link, that you don't need, is obviously
preferable.
4. The granularity is the `.o` file. It is best to have an .o file for
each category and class files.
5. If you have written your own +dependencies for +load code, be sure that
those dependencies are covered. They should be, because, if they are not,
why are they dependencies in the first place ?
6. Preferably optimize all ObjC libraries and not just a subset.
7. Coverage can't predict the future. If you use a plugin architecture using **NSBundle**, then using coverage optimization is not a good idea.


## Dealing with unoptimizable Libraries

Linking unoptimizable libraries can be problem. The library may contain
classes, whose subclasses may not appear in the coverage. Yet those classes
will be stuck loading at runtume, waiting for their superclasses.

In this case you could generate these missing classes manually with
`mulle-objc-printline` and append them to your coverage files.



