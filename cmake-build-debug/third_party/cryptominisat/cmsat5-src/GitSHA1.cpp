/******************************************
Copyright (c) 2017, Mate Soos

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
***********************************************/

#include "src/GitSHA1.h"

const char* CMSat::get_version_sha1()
{
    static const char myversion_sha1[] = "f8190ceed309613e22424fe4ce12e4ba93adcba9";
    return myversion_sha1;
}

const char* CMSat::get_version_tag()
{
    static const char myversion_tag[] = "5.11.14";
    return myversion_tag;
}

const char* CMSat::get_compilation_env()
{
    static const char compilation_env[] =
    "CMAKE_CXX_COMPILER = /usr/bin/c++ | "
    "CMAKE_CXX_FLAGS =  -Wall -Wextra -Wunused -Wsign-compare -fno-omit-frame-pointer -Wtype-limits -Wuninitialized -Wno-deprecated -Wstrict-aliasing -Wpointer-arith -Wpointer-arith -Wformat-nonliteral -Winit-self -Wparentheses -Wunreachable-code -g -Wno-class-memaccess -mpopcnt -msse4.2 -ggdb3 -Wlogical-op -Wrestrict -Wnull-dereference -Wdouble-promotion -Wshadow -Wformat=2 -Wextra-semi -pedantic | "
    "COMPILE_DEFINES =  | "
    "STATICCOMPILE = OFF | "
    "ONLY_SIMPLE = ON | "
    "Boost_FOUND = FALSE | "
    "STATS = OFF | "
    "SQLITE3_FOUND =  | "
    "ZLIB_FOUND = TRUE | "
    "VALGRIND_FOUND =  | "
    "ENABLE_TESTING = ON | "
    "SLOW_DEBUG = OFF | "
    "ENABLE_ASSERTIONS = ON | "
    "PYTHON_EXECUTABLE = /usr/bin/python | "
    "PYTHON_LIBRARY =  | "
    "PYTHON_INCLUDE_DIRS =  | "
    "MY_TARGETS =  | "
    "LARGEMEM = OFF | "
    "LIMITMEM = OFF | "
    "BREAKID_LIBRARIES =  | "
    "BREAKID-VER = . | "
    "BOSPHORUS_LIBRARIES =  | "
    "BOSPH-VER = . | "
    "compilation date time = " __DATE__ " " __TIME__
    ""
    ;
    return compilation_env;
}
