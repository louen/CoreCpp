/*  ========================================================================= *
 *                             CoreStrings.hpp                                *
 *                core strings functions and macros for  C++                  *
 *                          Valentin Roussellet                               *
 *  ========================================================================= */

// Minimum C++ version : C++11
// requires CoreMacros.hpp for the assert and compiler macros

#ifndef CORE_STRING_HPP_
#define CORE_STRING_HPP_

#include "CoreMacros.hpp"

#include <string>
#include <sstream>
#include <codecvt>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <memory>

#ifdef COMPILER_MSVC
    #define NOMINMAX                // Avoid C2039 MSVC compiler error
#endif


namespace core
{
    /// Split a string in multiple substrings according to a token.
    /// Example : splitString("a,b,c and d", ',') returns { "a","b","c and d"}.
    inline std::vector<std::string> splitString( const std::string& std, char token )
    {
        std::stringstream ss(str);
        std::string element;
        std::vector<std::string> result;

        while ( std::getline(ss, element, token)) { result.push_back(element); }
        return result; 
    }

    /// Convert a std::string to wstring, the right way
    inline std::wstring s2ws(const std::string& str)
    {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;
        return converterX.from_bytes(str);
    }

    // Convert a std::wstring to string, the right way
    inline std::string ws2s(const std::wstring& wstr)
    {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;
        return converterX.to_bytes(wstr);
    }

    // Printf-to-string functions.

    // Each function expecting varargs has a va_list equivalent
    // which does the actual job (because you can't pas varargs to another
    // function). See http://c-faq.com/varargs/handoff.html

    int stringvPrintf( std::string& str, const char* fmt, va_list args )
    {
        // Random guessing value from the size of the format string.
        size_t size = strlen( fmt ) * 2;
        int finalSize = 0;
        str.clear();
        char* buffer = nullptr;

        while ( 1 )
        {
            // Dynamically allocate a string.
            delete[] buffer;
            buffer = new char[size];

            // Attempt to printf into the buffer
            va_list argsCopy;
            va_copy(argsCopy,args);
            finalSize = vsnprintf( buffer, size, fmt, argsCopy );
            va_end(argsCopy);

            // vsnprinf can return -1 in case of an encoding error on some platforms.
            CORE_ASSERT( finalSize >= 0, "Encoding error");

            // If our buffer was too small, we know that final_size
            // gives us the required buffer size.
            if ( uint( finalSize ) >= size )
            {
                size = std::max( size + 1, size_t ( finalSize ) );
            }
            else
            {
                break;
            }
        }
        if ( finalSize > 0 )
        {
            str = std::string( buffer );
        }
        delete[] buffer;
        return finalSize;
    }

    int appendvPrintf( std::string& str, const char* fmt, va_list args )
    {
        std::string toAppend;
        int result = stringvPrintf( toAppend, fmt, args );
        str += toAppend;
        return result;
    }

    // These functions are exposed to the interface, but they just call
    // the va_list versions above.
    int stringPrintf( std::string& str, const char* fmt, ... )
    {
        va_list args;
        va_start(args,fmt);
        int result = stringvPrintf(str,fmt,args);
        va_end(args);
        return result;
    }

    int appendPrintf( std::string& str, const char* fmt, ... )
    {
        va_list args;
        va_start(args, fmt);
        int result = appendvPrintf(str, fmt, args);
        va_end(args);
        return result;
    }

} // end namespace core

#endif // CORE_STRING_HPP_

//
// Credits  - References
// 

// split string : https://stackoverflow.com/a/236803/746547
// string/wstring conversion : https://stackoverflow.com/a/18374698/746547
// printf to string : I wrote this originally for RadiumEngine : https://github.com/STORM-IRIT/Radium-Engine

