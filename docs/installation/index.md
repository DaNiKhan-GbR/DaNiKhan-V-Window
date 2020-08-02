# Installation instructions
[Back to home](../)

While you can [build](../build) Dnkvw yourself it is recommended to install the precompiled
binaries. This article describes how you can do that. Note that these precompiled binaries
come with all required dependencies. No further work required.

## Download manually

You can always download the latest precompiled binary on the 
[GitHub Release Page](https://github.com/DaNiKhan-GbR/DaNiKhan-V-Window/releases).
Select the right download for your system. This package includes the dynamic library,
the headers and a CMake configuration in case you want to use it with a CMake project.

## CMake: Automatic download

If you already use CMake and want to download Dnkvw automatically, use the following snippet:

```cmake
# Import Dnkvw
find_package(dnkvw 1.0.0 QUIET)
if (NOT dnkvw_FOUND) # If there's none, fetch dnkvw
    if(MSVC)
        set(dnkvw_url "https://github.com/DaNiKhan-GbR/DaNiKhan-V-Window/releases/latest/download/Dnkvw-Windows-MSVC.zip")
    elseif(APPLE)
        set(dnkvw_url "https://github.com/DaNiKhan-GbR/DaNiKhan-V-Window/releases/latest/download/Dnkvw-macOS.zip")
    else()
        message( FATAL_ERROR "There is no precompiled dnkvw library for your system." )
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/dnkvw")
    file(DOWNLOAD "${dnkvw_url}" "${CMAKE_BINARY_DIR}/dnkvw/dnkvw.zip" SHOW_PROGRESS)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xvf "${CMAKE_BINARY_DIR}/dnkvw/dnkvw.zip" 
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/dnkvw"
    )
    set(dnkvw_DIR "${CMAKE_BINARY_DIR}/dnkvw/lib/cmake/dnkvw" CACHE PATH "" FORCE)
    find_package(dnkvw 1.0.0 REQUIRED)
endif()
```

This snippet downloads the latest release from GitHub and unpacks it if needed.
Note that you can also set dnkvw_DIR directly if you already have Dnkvw installed.
