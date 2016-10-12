#ifndef H_DYNAMICLIBRARYHELPER
#define H_DYNAMICLIBRARYHELPER

#ifdef _WIN32
#ifdef _EXPORTING
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif
#else
#define EXPORT 
#endif

#endif