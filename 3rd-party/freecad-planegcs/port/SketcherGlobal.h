#pragma once

#if defined(_WIN32)
    #ifdef BUILDING_PLANETGCS
        #define SKETCHER_EXPORT __declspec(dllexport)
        #define SketcherExport __declspec(dllexport)
    #else
        #define SKETCHER_EXPORT __declspec(dllimport)
        #define SketcherExport __declspec(dllimport)
    #endif
#else
    #define SKETCHER_EXPORT __attribute__ ((visibility ("default")))
    #define SketcherExport __attribute__ ((visibility ("default")))
#endif
