cmake_minimum_required(VERSION 3.13)

project(MREmuWeb C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DLIBICONV_PLUG)

include_directories(
    "${CMAKE_SOURCE_DIR}/../include"
    "${CMAKE_SOURCE_DIR}/../MREmu"
    "${CMAKE_SOURCE_DIR}/../deps/spdlog/include"
    "${CMAKE_SOURCE_DIR}/../deps/libiconv-cmake/include"
    "${CMAKE_SOURCE_DIR}/../deps/libADLMIDI/include"
    "${CMAKE_SOURCE_DIR}/../deps/unicorn/include"
)

# MREngine Static Library for WASM
set(MRE_ENGINE_SOURCES
    "../MREmu/MREngine/Graphic.cpp"
    "../MREmu/MREngine/GraphicOld.cpp"
    "../MREmu/MREngine/Canvas.cpp"
    "../MREmu/MREngine/Image.cpp"
    "../MREmu/MREngine/Blt.cpp"
    "../MREmu/MREngine/System.cpp"
    "../MREmu/MREngine/Resources.cpp"
    "../MREmu/MREngine/Timer.cpp"
    "../MREmu/MREngine/ProgramManager.cpp"
    "../MREmu/MREngine/IO.cpp"
    "../MREmu/MREngine/SIM.cpp"
    "../MREmu/MREngine/Log.cpp"
    "../MREmu/MREngine/Sock.cpp"
    "../MREmu/MREngine/Sensor.cpp"
    "../MREmu/MREngine/Audio.cpp"
    "../MREmu/MREngine/AudioBitstream.cpp"
    "../MREmu/MREngine/AudioBitstreamRec.cpp"
    "../MREmu/MREngine/CharSet.cpp"
    "../MREmu/MREngine/Textout.cpp"
    "../MREmu/MREngine/STDLib.cpp"
)

add_library(MREngineWeb STATIC ${MRE_ENGINE_SOURCES})

# Include Submodules
set(BUILD_SHARED_LIBS FALSE CACHE BOOL "" FORCE)

add_subdirectory("../deps/libiconv-cmake" "deps/libiconv")
add_subdirectory("../deps/spdlog" "deps/spdlog")
add_subdirectory("../deps/libADLMIDI" "deps/libADLMIDI")

set(UNICORN_ARCH "arm" CACHE STRING "Supported architectures")
add_subdirectory("../deps/unicorn" "deps/unicorn")

# Core Emulator Web Executable
set(WEB_SOURCES
    "main_web.cpp"
    "../MREmu/Memory.cpp"
    "../MREmu/Cpu.cpp"
    "../MREmu/Bridge.cpp"
    "../MREmu/App.cpp"
    "../MREmu/ArmApp.cpp"
    "../MREmu/NativeApp.cpp"
    "../MREmu/MreTags.cpp"
    "../MREmu/ARModule.cpp"
    "../MREmu/AppManager.cpp"
    "../MREmu/Keyboard.cpp"
    "../MREmu/Touch.cpp"
    "../MREmu/Log.cpp"
    "../MREmu/miniz.c"
    "../MREmu/NativeApps/Menu/AppSelector.cpp"
)

add_executable(mremu_web ${WEB_SOURCES})

target_link_libraries(mremu_web
    MREngineWeb
    unicorn
    ADLMIDI_static
    libiconv
    spdlog
)

if (EMSCRIPTEN)
    set_target_properties(mremu_web PROPERTIES
        OUTPUT_NAME "mremu"
        SUFFIX ".html"
    )

    target_link_options(mremu_web PRIVATE
        "-sUSE_SDL=2"
        "-sUSE_MALLOC__IMPLEMENTATION=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sINITIAL_MEMORY=67108864"
        "-sFORCE_FILESYSTEM=1"
        "-sEXPORTED_FUNCTIONS=['_main','_malloc','_free']"
        "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap','FS']"
        "--shell-file=${CMAKE_CURRENT_SOURCE_DIR}/shell_minimal.html"
    )
endif()
