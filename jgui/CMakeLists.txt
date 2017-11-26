set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW Examples" FORCE )
set( GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW Tests" FORCE )
set( GLFW_INSTALL OFF CACHE BOOL "Build GLFW Installer" FORCE )
set( GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW Docs" FORCE )
add_subdirectory( ../thirdparty/glfw glfwlib )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_EXECUTABLE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin )


function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

FILE(GLOB_RECURSE jgui_SRC RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "include/**.h"
    "src/**.cpp"
	"src/**.c"
	"../public/**.h"
	"../thirdparty/bx/src/amalgamated.cpp"
	"../thirdparty/bimg/src/**.cpp"
	"../thirdparty/bimg/3rdparty/**.cpp"
	"../thirdparty/bimg/3rdparty/**.c"
	"../thirdparty/bgfx/src/amalgamated.cpp"
	"../thirdparty/bgfx/examples/common/nanovg/**.cpp"
	"../thirdparty/bgfx/examples/common/nanovg/**.h"
)

list(REMOVE_ITEM jgui_SRC "../thirdparty/bimg/3rdparty/lodepng/lodepng.cpp")

add_library( jgui SHARED ${jgui_SRC} )

assign_source_group( ${jgui_SRC} )

target_include_directories( jgui PRIVATE ../public )

target_include_directories( jgui PRIVATE ../thirdparty/stb )

target_compile_definitions( jgui PRIVATE __STDC_FORMAT_MACROS JGUI_DLL JGUI_USE_GLFW JGUI_USE_BGFX )

target_include_directories( jgui PRIVATE ../thirdparty/bx/3rdparty )
target_include_directories( jgui PRIVATE ../thirdparty/bx/include )
if (MSVC)
target_include_directories( jgui PRIVATE ../thirdparty/bx/include/compat/msvc )
endif()

target_include_directories( jgui PRIVATE ../thirdparty/bimg/include )
target_include_directories( jgui PRIVATE ../thirdparty/bimg/3rdparty )
target_include_directories( jgui PRIVATE ../thirdparty/bimg/3rdparty/iqa/include )
target_include_directories( jgui PRIVATE ../thirdparty/bimg/3rdparty/nvtt )
target_include_directories( jgui PRIVATE ../thirdparty/bgfx/include )
target_include_directories( jgui PRIVATE ../thirdparty/bgfx/3rdparty )
target_include_directories( jgui PRIVATE ../thirdparty/bgfx/3rdparty/dxsdk/include )
target_include_directories( jgui PRIVATE ../thirdparty/bgfx/3rdparty/khronos )
target_include_directories( jgui PRIVATE ../thirdparty/bgfx/examples/common/nanovg )

target_include_directories( jgui PRIVATE include )
target_link_libraries( jgui glfw )