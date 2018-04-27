/* (C) Copyright N.M. Maclaren 1993, 1994, 1995
   (C) Copyright the University of Cambridge 1993, 1994, 1995
   ALL RIGHTS RESERVED

ebcdic.h  -  everything to do with EBCDIC to ASCII conversion.  Warning:
this is used by check, loadtape and mvstape as well as the IBM conversion
library. */



/* Kludge the routines to handle 16-bit ints, after a fashion.  These will not
work with MVStape, but are supported by the conversion library.  This whole
areas is revolting, but then it follows C in this respect. */

#include <limits.h>
#if INT_MAX >= 2147483647
#define UINT unsigned int
#else
#define UINT unsigned long
#endif



/* Workspace and functions must be local for the library and global for the
main program.  Other definitions for the program do not matter when imported
into the library, because they are not reexported.  The INTERN kludge is needed
to bypass a gratuitous and perverse misreading of the ANSI standard by Sun's
Solaris 2 compiler. */

#ifdef CONVERSION_LIBRARY
#define EXTERN static
#define INTERN static
#else
#define EXTERN extern
#define INTERN
#endif



/* Define some EBCDIC characters that are used in unpick.c. */

#define EBCDIC_SPACE    0x40    /* ANSI control characters - what else? */
#define EBCDIC_PLUS     0x4e
#define EBCDIC_ONE      0xf1
#define EBCDIC_ZERO     0xf0
#define EBCDIC_MINUS    0x60



/* Define a macro to translate EBCDIC, for efficiency.  Note that it uses -1 as
the invalid character. */

#define EBCDIC_to_ASCII(x) (ebcdic_to_ascii[(unsigned char)(x)])
extern short (EBCDIC_to_ASCII) (int character);



/* Define the code page lookup table format. */

typedef struct {
    const char *name;                 /* The name of the code page */
    const short *table;               /* The address of the lookup table */
} CODE_PAGE;



/* Define the external function and arrays in ebcdic.c.  The kludge for the
conversion library is needed to bypass a fairly common compiler bug, which is 
present in IBM's CSet/2. */

EXTERN void initialize_code_pages (void);

EXTERN CODE_PAGE *select_code_page (const char *name);

#ifndef CONVERSION_LIBRARY
EXTERN CODE_PAGE code_pages[];         /* The code page lookup table */
#endif
EXTERN const short *ebcdic_to_ascii;   /* The translation table in use */



/* Define types and macros for handling big-endian shorts and ints on any type
of architecture.  This has been tested mainly on big-endian machines. */

typedef unsigned char BE_short[2];    /* Big-endian short */
typedef unsigned char BE_int[4];      /* Big-endian int */
typedef unsigned char *BE_ptr;        /* Yuck, C */

#define get_BE_short(x) (((const BE_ptr)(x))[1]+\
    ((unsigned int)((const BE_ptr)(x))[0]<<8))
extern unsigned int (get_BE_short) (const BE_ptr);

#define get_BE_int(x) \
    (((const BE_ptr)(x))[3]+((((const BE_ptr)(x))[2]+((((const BE_ptr)(x))[1]+\
    ((UINT)((const BE_ptr)(x))[0]<<8))<<8))<<8))
extern UINT (get_BE_int) (const BE_ptr);

#define put_BE_short(x,y) \
    (((BE_ptr)(x))[0] = (y)>>8, ((BE_ptr)(x))[1] = (y)&0xff)
extern void (put_BE_short) (BE_short, unsigned int);

#define put_BE_int(x,y) \
    (((BE_ptr)(x))[0] = (y)>>24, ((BE_ptr)(x))[1] = ((y)>>16)&0xff, \
    ((BE_ptr)(x))[2] = ((y)>>8)&0xff, ((BE_ptr)(x))[3] = (y)&0xff)
extern void (put_BE_int) (BE_int, UINT);



