#include <iostream>
#include <spdlog/spdlog.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include "../MREmu/Memory.h"
#include "../MREmu/Cpu.h"
#include "../MREmu/Bridge.h"
#include "../MREmu/AppManager.h"
#include "../MREmu/MREngine/SIM.h"
#include "../MREmu/MREngine/System.h"
#include "../MREmu/MREngine/CharSet.h"
#include "../MREmu/MREngine/Audio.h"
#include "../MREmu/MREngine/Graphic.h"
#include "../MREmu/MREngine/IO.h"

AppManager g_appManager;

void web_main_loop() {
    static uint32_t last_tick = 0;
    uint32_t current_tick = 0; // Tick calculation placeholder
    uint32_t delta_ms = 16; // ~60 FPS update slice

    g_appManager.update(delta_ms);
}

int main(int argc, char** argv) {
    spdlog::info("MREmu WebAssembly Initialization...");

    Memory::init(32 * 1024 * 1024);
    Cpu::init();
    Bridge::init();

    MREngine::SIM::init();
    MREngine::System::init();
    MREngine::CharSet::init();
    MREngine::AppAudio::init();
    MREngine::IO::init();

    spdlog::info("Core MREmu modules initialized for WebAssembly target.");

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(web_main_loop, 0, 1);
#else
    spdlog::info("Web skeleton executed outside Emscripten environment.");
#endif

    return 0;
}
