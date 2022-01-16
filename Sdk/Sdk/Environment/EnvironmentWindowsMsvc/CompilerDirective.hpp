#pragma once

#define __API__         __inline
#define _IN_
#define _OUT_
#define _C_CALL_        __cdecl                //# for C/Cpp function call format
#define _STD_CALL_      __stdcall              //# for Standard call format
#define _FAST_CALL_     __fastcall             //# for Fast call format, register store argument
#define _INLINE_        __inline               //# for inline function declare
#define _NOINLINE_      __declspec(noinline)   //# for not inline function declare
#define _ALIGN_(x)      __declspec(align(x))   //# for data alignment minimum in struct/class
#define _DLL_IMPORT_    __declspec(dllimport)  //# for dll import declare
#define _DLL_EXPORT_    __declspec(dllexport)  //# for dll export declare
#define _NAKED_         __declspec(naked)      //# for not store register when enter function
#define _VAR_RESTRICT_  __restrict             //# for variable restrict 
#define _RESTRICT_      __declspec(restrict)   //# for function with a return pointer and this return pointer is not aliased
#define _NOALIAS_       __declspec(noalias)    //# for function insure that only function internal data or the data internal pointer point to are used in function
#define _NORETURN_      __declspec(noreturn)   //# for function without return value
#define _NOTHROW_       __declspec(nothrow)    //# for function insure that no exception would be throw internally
#define _NOVTABLE_      __declspec(novtable)   //# for class that no virtual function table, only used in non-virtual class or pure-virtual class
#define _SELECTANY_     __declspec(selectany)  //# for variable definition in header file without causing redefine error
#define _NODISCARD_     [[nodiscard]]

#define _Await_         co_await
#define _Retrun_        co_return

#define POINTER_BYTES (8)
#define POINTER_BITS  (64)

#pragma warning (disable : 4005)
#pragma warning (disable : 4018)
#pragma warning (disable : 4200)
#pragma warning (disable : 4244)
#pragma warning (disable : 4305)
#pragma warning (disable : 4307)
#pragma warning (disable : 4595)
#pragma warning (disable : 6011)
#pragma warning (disable : 6258)
#pragma warning (disable : 6262)
#pragma warning (disable : 6385)
#pragma warning (disable : 6386)
#pragma warning (disable : 26450)
#pragma warning (disable : 26454)
#pragma warning (disable : 26495)
#pragma warning (disable : 26812)
#pragma warning (disable : 28182)
