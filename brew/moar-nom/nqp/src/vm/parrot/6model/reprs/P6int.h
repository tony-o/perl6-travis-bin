#ifndef P6INT_H_GUARD
#define P6INT_H_GUARD

#define P6INT_C_TYPE_CHAR      -1
#define P6INT_C_TYPE_SHORT     -2
#define P6INT_C_TYPE_INT       -3
#define P6INT_C_TYPE_LONG      -4
#define P6INT_C_TYPE_LONGLONG  -5

typedef struct {
    /* Integer storage slot. */
    INTVAL value;
} P6intBody;

/* This is how an instance with the P6int representation looks. */
typedef struct {
    SixModelObjectCommonalities common;
    P6intBody body;
} P6intInstance;

/* The bit width requirement is shared for all instances of the same type. */
typedef struct {
    INTVAL bits;
    INTVAL is_unsigned;
} P6intREPRData;

/* Initializes the P6int REPR. */
REPROps * P6int_initialize(PARROT_INTERP);

#endif
