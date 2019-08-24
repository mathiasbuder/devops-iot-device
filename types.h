/*********************************************************************
    MODULE SPECIFICATION:

        $URL: file:///C:/Users/Nano/Desktop/TestRepo/main/hello_world_main.c $
        $Revision: 6 $
        $Author: Nano $
        $Date: 2019-08-24 10:31:10 +0200 (Sat, 24 Aug 2019) $

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2019

*********************************************************************/

#ifndef TYPES_H
    #define TYPES_H

    #ifdef __GNUC__
        #include <stdint.h>
    #endif

    /* =============================== */
    /*  Application-specific switches  */
    /* =============================== */

    /* =========================== */
    /* STANDARD TYPES & FUNCTIONS  */
    /* =========================== */

    #ifdef __GNUC__
        typedef uint8_t             U8;      /* 1 Byte */
        typedef uint16_t            U16;     /* 2 Byte */
        typedef uint32_t            U32;     /* 4 Byte */
        typedef uint64_t            U64;     /* 8 Byte */

        typedef int8_t              S8;      /* 1 Byte */
        typedef int16_t             S16;     /* 2 Byte */
        typedef int32_t             S32;     /* 4 Byte */
        typedef int64_t             S64;     /* 8 Byte */
    #else/* Type definitions - 5561 code */
        typedef unsigned char       U8;      /* 1 Byte */
        typedef unsigned short      U16;     /* 2 Byte */
        typedef unsigned long       U32;     /* 4 Byte */

        typedef signed char         S8;      /* 1 Byte */
        typedef signed short        S16;     /* 2 Byte */
        typedef signed long         S32;     /* 4 Byte */

        #ifdef __BORLANDC__
            typedef unsigned __int64    U64; /* 8 Byte */
            typedef __int64             S64; /* 8 Byte */
        #else
            typedef unsigned long long  U64; /* 8 Byte */
            typedef signed long long    S64; /* 8 Byte */
        #endif
    #endif

        typedef U8                  BOOLEAN; /* 1 Byte */

        typedef float               F32;     /* 4 Byte */
        typedef double              F64;     /* 8 Byte */

        typedef unsigned char*      U8ptr;   /* 4 Byte */
        typedef unsigned int        bit_fld; /* 4 Byte */


    #ifdef simulator
        #define __attribute__(x)
    #endif

    __attribute__((aligned(8))) typedef struct
    {
        F32     real;
        F32     imag;
    } COMPLEX;

        typedef struct
        {
            bit_fld flag7 : 1;
            bit_fld flag6 : 1;
            bit_fld flag5 : 1;
            bit_fld flag4 : 1;
            bit_fld flag3 : 1;
            bit_fld flag2 : 1;
            bit_fld flag1 : 1;
            bit_fld flag0 : 1;
        } __attribute__ ((packed)) t_flags;

        typedef union
        {
            t_flags flags;
            U8      flag_byte;
        } __attribute__ ((packed)) t_flag_type ;


    #ifndef NULL
        #define NULL    (void *)0
    #endif

    #ifndef FALSE
        #define FALSE               0U
    #endif
    #ifndef TRUE
        #define TRUE                1U
    #endif

    #define NOT_USED                0U

    #define ZERO                    0U

    #define CHANNEL_0               0U
    #define CHANNEL_1               1U

    /* Define a structure for the maps */
    struct MAPTYPE {
        U32 num_of_x_bpoints;
        U32 num_of_y_bpoints;
        void *x_bpoints;
        void *y_bpoints;
        void *map_pts;
    };

    #define MAX_RING_BUFFER         9
    #define MAX_BIG_RING_BUFFER     50
    #define MAX_RING_BUFFER_32      32

    typedef struct    /* ring buffer */
    {
        S8   buffer_vs8[MAX_RING_BUFFER_32];
        U8   next_free_index_u8;
        U8   current_length_u8;
    } RING_STRUCT_32;

    /* for operations on ring buffer see ring_structure.h */
    typedef struct RING_STRUCT_TAG   /* ring buffer */
    {
        F32 *buffer_vf32;
        U8   next_free_index_u8;
        U8   current_length_u8;
        U8   max_length_u8; /* length of the buffer array. */
    } RING_STRUCT;

    /*
    degree + 1 = DIMENSION
    Polynoms are represented by F32 arrays of size POLY_NR_DIMENSIONS
    poly = C + Bx + Ax^2 + dAx^3
    acces the elements using the enumeration
    */
    typedef enum {
        POLY_C, /* LF: Lane Fusion */
        POLY_B,
        POLY_A,
        POLY_dA,
        POLY_NR_DIMENSIONS
    } POLYNOM_PARAMETERS;

    /* constants */
    #define PI                  3.14159265359F

    #define SMALL_NUMBER        0.0001F
    #define BIG_NUMBER          10000000.0F
    #define INFINITE            1000000000000.0F

    #define RAD_TO_DEG          57.295780F      /*  convert radians to degrees */
    #define DEG_TO_RAD          0.17453293E-1F  /*  convert degrees to radians */

    /* approximation of trigonometric functions with Taylor expansion up to fourth order */
    #define SIN_FROM_DEG(X)     ((0.17453293E-1F*(X)) - (0.8860962E-6F*(X)*(X)*(X)))
    #define SIN_FROM_RAD(X)     ((X) - (0.16666667F*(X)*(X)*(X)))
    #define COS_FROM_DEG(X)     ((1.0F - (0.15230872E-3F*(X)*(X))) + (0.38663241E-8F*(X)*(X)*(X)*(X)))
    #define COS_FROM_RAD(X)     ((1.0F - (0.5F*(X)*(X))) + (0.41666667E-1F*(X)*(X)*(X)*(X)))
    #define TAN_FROM_DEG(X)     ((0.17453293E-1F*(X)) + (0.17721924E-5F*(X)*(X)*(X)))
    #define TAN_FROM_RAD(X)     ((X) + (0.33333333F*(X)*(X)*(X)))
    /* inverse functions */
    #define ASIN_TO_DEG(X)      ((0.5729578E2F*(X)) + (0.95492966E1F*(X)*(X)*(X)))
    #define ASIN_TO_RAD(X)      ((X) + (0.16666667F*(X)*(X)*(X)))
    /* note this is an expansion around 0 in the argument where angle = pi/2! */
    /* acos does not have an expansion around 1! */
    #define ACOS_TO_DEG(X)      (0.90000000E2F - (0.5729578E2F*(X)) - (0.95492966E1F*(X)*(X)*(X)))
    #define ACOS_TO_RAD(X)      ((0.15707964E1F-1.0F*(X)) - (0.16666667F*(X)*(X)*(X)))
    #define ATAN_TO_DEG(X)      ((0.5729578E2F*(X)) - (0.19098593E2F*(X)*(X)*(X)))
    #define ATAN_TO_RAD(X)      ((X) - (0.33333333F*(X)*(X)*(X)))
    /* other functions */
    #define EXP(X)              (1.0F + (X) + (0.5F*(X)*(X)) + (0.16666667F*(X)*(X)*(X)) + (0.41666667E-1F*(X)*(X)*(X)*(X)))
    #define TANH(X)             ((X) - (0.33333333F*(X)*(X)*(X)))

    /* Type ranges */
    #define MAX_S6            31L
    #define MIN_S6          (-32L)
    #define MAX_S7            63L
    #define MIN_S7          (-64L)
    #define MAX_S8           127L
    #define MIN_S8         (-128L)
    #define MAX_S9           255L
    #define MIN_S9         (-256L)
    #define MAX_S10          511L
    #define MIN_S10        (-512L)
    #define MAX_S11         1023L
    #define MIN_S11       (-1024L)
    #define MAX_S12         2047L
    #define MIN_S12       (-2048L)
    #define MAX_S13         4095L
    #define MIN_S13       (-4096L)
    #define MAX_S14         8191L
    #define MIN_S14       (-8192L)
    #define MAX_S15        16383L
    #define MIN_S15      (-16384L)
    #define MAX_S16        32767L
    #define MIN_S16      (-32768L)
    #define MAX_S32   2147483647L
    #define MIN_S32 (-2147483648L)
    #define MAX_U2            3UL
    #define MIN_U2            0UL
    #define MAX_U3            7UL
    #define MIN_U3            0UL
    #define MAX_U4           15UL
    #define MIN_U4            0UL
    #define MAX_U5           31UL
    #define MIN_U5            0UL
    #define MAX_U6           63UL
    #define MIN_U6            0UL
    #define MAX_U7          127UL
    #define MIN_U7            0UL
    #define MAX_U8          255UL
    #define MIN_U8            0UL
    #define MAX_U9          511UL
    #define MIN_U9            0UL
    #define MAX_U10        1023UL
    #define MIN_U10           0UL
    #define MAX_U11        2047UL
    #define MIN_U11           0UL
    #define MAX_U12        4095UL
    #define MIN_U12           0UL
    #define MAX_U13        8191UL
    #define MIN_U13           0UL
    #define MAX_U14       16383UL
    #define MIN_U14           0UL
    #define MAX_U15       32767UL
    #define MIN_U15           0UL
    #define MAX_U16       65535UL
    #define MIN_U16           0UL
    #define MAX_U32  4294967295UL
    #define MIN_U32           0UL

#endif /* TYPES_H */

