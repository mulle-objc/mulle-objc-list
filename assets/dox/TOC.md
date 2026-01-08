# mulle-objc-list Library Documentation for AI

## 1. Introduction & Purpose

**mulle-objc-list** provides runtime metadata querying and enumeration utilities for mulle-objc. It enables inspection and listing of loaded classes, methods, protocols, and other runtime information. Useful for runtime introspection, dynamic code analysis, and documentation generation.

This library is particularly useful for:
- Runtime introspection of loaded classes
- Generating documentation from code
- Analyzing available methods and protocols
- Debugging runtime state
- Building tools that examine Objective-C code

## 2. Key Concepts & Design Philosophy

- **Runtime Metadata**: Query what's loaded in runtime
- **Enumeration**: Iterate over classes, methods, protocols
- **Inspection**: Examine runtime data structures
- **Non-Destructive**: Read-only queries, no side effects

## 3. Core API & Data Structures

### Class Enumeration

- `mulle_objc_class_list_t mulle_objc_list_allclasses()`
  - Get list of all loaded classes
  - Returns enumerable list

- `unsigned int objc_getClassList(Class *buffer, unsigned int bufferCount)`
  - Get classes into buffer
  - Returns count of classes

### Class Information

- `const char *class_getVersion(Class cls)`
  - Get class version string

- `const char *class_getName(Class cls)`
  - Get class name

- `Class class_getSuperclass(Class cls)`
  - Get superclass

### Method Enumeration

- `Method *class_copyMethodList(Class cls, unsigned int *outCount)`
  - Enumerate instance methods

- `Method *class_getClassMethods(Class cls, unsigned int *outCount)`
  - Enumerate class methods

- `const char *method_getName(Method m)`
  - Get method selector name

- `const char *method_getTypeEncoding(Method m)`
  - Get method type signature

### Protocol Enumeration

- `Protocol **class_copyProtocolList(Class cls, unsigned int *outCount)`
  - Get protocols implemented by class

- `Protocol **objc_copyProtocolList(unsigned int *outCount)`
  - Get all protocols

### Property Enumeration

- `objc_property_t *class_copyPropertyList(Class cls, unsigned int *outCount)`
  - Enumerate properties

## 4. Performance Characteristics

- **Enumeration**: O(n) where n = number of items
- **Memory**: Allocates temporary lists
- **Caching**: Results not cached; fresh on each call

## 5. Integration Examples

### Example 1: List All Classes
```c
unsigned int classCount = 0;
Class *classes = objc_copyClassList(&classCount);

for (unsigned int i = 0; i < classCount; i++) {
    printf("Class: %s\n", class_getName(classes[i]));
}

free(classes);
```

### Example 2: Inspect Methods
```c
Class cls = objc_getClass("NSString");
unsigned int methodCount = 0;
Method *methods = class_copyMethodList(cls, &methodCount);

for (unsigned int i = 0; i < methodCount; i++) {
    const char *name = method_getName(methods[i]);
    const char *types = method_getTypeEncoding(methods[i]);
    printf("- %s (%s)\n", name, types);
}

free(methods);
```

### Example 3: Generate Class Documentation
```c
unsigned int classCount = 0;
Class *classes = objc_copyClassList(&classCount);

for (unsigned int i = 0; i < classCount; i++) {
    Class cls = classes[i];
    printf("@interface %s", class_getName(cls));
    
    Class super = class_getSuperclass(cls);
    if (super) {
        printf(" : %s", class_getName(super));
    }
    printf("\n");
}

free(classes);
```

## 6. Dependencies

- **mulle-objc-runtime** - mulle runtime
- Standard C library

## 7. Version Information

mulle-objc-list version macro: `MULLE_OBJC_LIST_VERSION`
