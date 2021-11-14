#pragma once

#define __API__         __inline
#define _IN_
#define _OUT_
#define _C_CALL_        __attribute__((cdecl))      //# for C/Cpp function call format
#define _STD_CALL_      __attribute__((stdcall))    //# for Standard call format
#define _FAST_CALL_     __attribute__((fastcall))   //# for Fast call format, register store argument
#define _INLINE_        inline                      //# for inline function declare
#define _NOINLINE_      // __declspec(noinline)     //# for not inline function declare
#define _ALIGN_(x)      // __declspec(align(x))     //# for data alignment minimum in struct/class
#define _DLL_IMPORT_    // __declspec(dllimport)    //# for dll import declare
#define _DLL_EXPORT_    // __declspec(dllexport)    //# for dll export declare
#define _NAKED_         // __declspec(naked)        //# for not store register when enter function
#define _VAR_RESTRICT_  // __restrict               //# for variable restrict
#define _RESTRICT_      // __declspec(restrict)     //# for function with a return pointer and this return pointer is not aliased
#define _NOALIAS_       // __declspec(noalias)      //# for function insure that only function internal data or the data internal pointer point to are used in function
#define _NORETURN_      // __declspec(noreturn)     //# for function without return value
#define _NOTHROW_       // __declspec(nothrow)      //# for function insure that no exception would be throw internally
#define _NOVTABLE_      // __declspec(novtable)     //# for class that no virtual function table, only used in non-virtual class or pure-virtual class
#define _SELECTANY_     __attribute__((weak))  //# for variable definition in header file without causing redefine error

#define POINTER_BYTES (8)
#define POINTER_BITS  (64)

#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wformat="
#pragma GCC diagnostic ignored "-Wformat-security"
#pragma GCC diagnostic ignored "-Wnonnull"
#pragma GCC diagnostic ignored "-Woverflow"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wunused-variable"
