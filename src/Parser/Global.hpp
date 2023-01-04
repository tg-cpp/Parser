#pragma once

#if defined _MSC_VER

    #define B_OS_WINDOWS

    #define UNICODE
    #define _UNICODE

    #ifdef BUILD_SHARED_LIBS
        #define PARSER_EXPORT __declspec(dllexport)
        #define PARSER_IMPORT __declspec(dllimport)
    #else
        #define PARSER_EXPORT
        #define PARSER_IMPORT
    #endif

    #define PARSER_LOCAL

#elif defined __APPLE__

    #define B_OS_MACOS

    #ifdef BUILD_SHARED_LIBS
        #define PARSER_EXPORT __attribute__ ((visibility ("default")))
        #define PARSER_IMPORT
        #define PARSER_LOCAL __attribute__ ((visibility ("hidden")))
    #else
        #define PARSER_EXPORT
        #define PARSER_IMPORT
        #define PARSER_LOCAL
    #endif

#else

#define B_OS_LINUX

    #ifdef BUILD_SHARED_LIBS
        #define PARSER_EXPORT __attribute__ ((visibility ("default")))
        #define PARSER_IMPORT
        #define PARSER_LOCAL __attribute__ ((visibility ("hidden")))
    #else
        #define PARSER_EXPORT
        #define PARSER_IMPORT
        #define PARSER_LOCAL
    #endif

#endif