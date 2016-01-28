#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT extern
#endif

#if defined(_MSC_VER) && _MSC_VER < 1800
#  define bool unsigned char
#  define true  1
#  define false 0
#  define __bool_true_false_are_defined 1
#else
#  include <stdbool.h>
#endif

#include <stddef.h>

typedef struct {
    char  foo1;
    int   foo2;
    short foo3;
    short foo4;
} Foo;

typedef struct {
    char  bar1;
    short bar2;
    char  bar3;
    int   bar4;
    short bar5;
} Bar;

typedef struct {
    char  bar1;
    short bar2;
    char  bar3;
    int   bar4;
    short bar5;
    long  baz6;
    int   bar7;
} Baz;

typedef struct {
    char  buz1;
} Buz;

DLLEXPORT int SizeofFoo() {
    return sizeof(Foo);
}

DLLEXPORT int SizeofBar() {
    return sizeof(Bar);
}

DLLEXPORT int SizeofBaz() {
    return sizeof(Baz);
}

DLLEXPORT int SizeofBuz() {
    return sizeof(Buz);
}

DLLEXPORT int SizeofInt() {
    return sizeof(int);
}

DLLEXPORT int SizeofLng() {
    return sizeof(long);
}

DLLEXPORT int SizeofPtr() {
    return sizeof(void *);
}

DLLEXPORT int SizeofBool() {
    return sizeof(bool);
}

DLLEXPORT int SizeofSizeT() {
    return sizeof(size_t);
}
