/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   disc_png;
    const int            disc_pngSize = 88485;

    extern const char*   knob_png;
    const int            knob_pngSize = 11348;

    extern const char*   loop_png;
    const int            loop_pngSize = 22483;

    extern const char*   next_png;
    const int            next_pngSize = 13761;

    extern const char*   pause_png;
    const int            pause_pngSize = 25882;

    extern const char*   play_png;
    const int            play_pngSize = 26052;

    extern const char*   previous_png;
    const int            previous_pngSize = 24216;

    extern const char*   stop_png;
    const int            stop_pngSize = 20461;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 8;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
