# CoreCpp
A collection of core macros and nifty functions I use all the time in C++.
The files should work similarly on Windows (MSVC / mingw / cygwin gcc), MacOS (gcc / clang) and Linux (gcc / clang).
The target standard at least C++ 11 (and probably will be C++14).

## Documentation

### `CoreMacros.hpp`
This is the base file, which should be included before any other files. This files defines three types of macros
- Macros for platform detection (know on which compiler, platform etc. you are running)  and abstraction (allowing to use the same macros for some intrinsics that are comon across compilers but differ in name or syntax.
- Generic macro building blocks utility
- Debug macros : assert and error macros, that are useful for debug checks

Additionally, it defines common `typedef`s.

### Platform detection and abstraction
One of the following will be defined depending on the compiler :
- `COMPILER_CLANG` for clang
- `COMPILER_GCC` for gcc
- `COMPILER_MSVC` for Visual Studio

One of the following will be defined depending on the OS :
- `OS_WINDOWS` for Windows
- `OS_MACOS` for MacOS
- `OS_LINUX` for Linux and cygwin

One of the following will be defined depending on the target architecture
- `ARCH_X86` for 32-bit
- `ARCH_X64` for 64-bits

An effort is made to ensure consistency of debug macros.
- `CORE_DEBUG`
- `CORE_RELEASE`

Additionally, there is a `ON_DEBUG()` macro which only activates the code inside on debug builds.

Calling convention macros (only relevant for x86 builds) :
- `CDECL`
- `STDCALL`
- `FASTCALL`

Branch prediction macros:
- `LIKELY( expr )`
- `UNLIKELY( expr )`

These macros give hints to the compiler's branch predictor about the likeliness of their argument evaluating to `true`.
Example usage :

```c++
if (UNLIKELY(x == nullptr)) {
  std::cout<<"x is null !;
}
```
As branch prediction intrinsics are not available with Visual Studio, this macro has no effect in MSVC builds.

Alignment macros (note : these macros are now part of standard c++ and will probably be removed)
- `ALIGN_OF( var )` : the alignment of a variable 
- `ALIGNED( decl, align )` Declare a variable with the given alignment. Example usage : `ALIGN( float x, 16);` will declare a float variable with 16-bytes alignment.

Inline macros to give additionnal hints to the compiler about inlining
- `ALWAYS_INLINE` : strongest inline requirement possible (possibly even in debug builds)
- `STRONG_INLINE`: at least equal or stronger than `inline`
- `NO_INLINE`: never inline the function, even in optimized builds.

Export keywords : `DLL_EXPORT` and `DLL_IMPORT` are defined to the corresponding declaration on MSVC and to nothing on other compilers (where they are not needed).


### Generic macros
A few macros are defined to use in building other complex macros

- `MACRO_START` and `MACRO_END` are helpers that wrap multiline macros in a block to avoid side effects to the code and allow them to be followed by a semicolon like a regular function call.

Example usage
```c++
#define AWESOME_MACRO( x, y, x ) \
MACRO_START \
if (x == y) { z; }\
MACRO_END
//...
AWESOME_MACRO(x, 42, printf("LOL"));
```
- `CORE_UNUSED(x)` is useful to mark a variable as unused in a function without having a compiler warning.
- `CONCATENATE(x,y)`joins two expressions together, even if one of them is itself a macro call
- `STRINGIFY(x)` makes its argument into a string
- `BREAKPOINT(x)` triggers a break in the program, which is useful for setting programmable breaks in a debugger (even in release), or forcing a core dump.
- `STATIC_SIZEOF(x)` : using this macro will make the compiler print the actual size of a type in an error message.

### Assert and error macros

Good assert macros are a very useful tool. Their behaviour is turned on automatically on debug builds, or by defining `CORE_ENABLE_ASSERT`, to have a release with assert builds (which might be useful).

The main macro is `CORE_ASSERT( expr, desc )`. If asserts are enabled, and `expr` evaluates to `false`, an error message indicating the file and line of the assert is printed, along with the supplied `desc` which should help describe the error.
After printing, the macro triggers a breakpoint, which should let the debugger point to the line of error (as opposed to somewhere in `assert.c`).

Similarly, `CORE_WARN_IF( expr, desc )` will print the same message as the assert (but if `expr` evaluates to `true`). However, it will not break, and let the program continue.

If asserts are disabled, these macros have no effect.

The `ON_ASSERT( expr )` macro executes the code in its argument only if asserts are enabled, which is useful for catching error codes for functions and asserting on them, while avoiding to create useless variables and warnings in non-assert builds.

```c++
ON_ASSERT(int result =) snprintf(buffer, size, "format %u %f", x, y); 
CORE_ASSERT( result > 0, "Error in snprinf (buffer too small ?)");
```

Finally, `CORE_ERROR( desc )` and `CORE_ERROR_IF( expr, desc )` trigger breakpoints even if asserts are disabled, and then call `exit(EXIT_FAILURE)` to abort the program.

### Typedefs

All unsigned integral types have been typedefed to a shortcut name : `uchar`, `ushort`, `uint` and `ulong`.
All integral types with explicit bit-size have beed typedefed to a shortcut name  : `int8`, `int16`, `int32` and `int64` for signed types, and `uint8`, `uint16`, `uint32` and `uint64` for unsigned types.

