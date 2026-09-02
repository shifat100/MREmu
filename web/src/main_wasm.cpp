#include <iostream>
#include <spdlog/spdlog.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include "Memory.h"
#include "Cpu.h"
#include "Bridge.h"
#include "AppManager.h"
#include "MREngine/SIM.h"
#include "MREngine/System.h"
#include "MREngine/CharSet.h"
#include "MREngine/Audio.h"
#include "MREngine/Graphic.h"
#include "MREngine/IO.h"

AppManager* g_webAppManager = nullptr;

void web_main_loop() {
    static uint32_t last_tick = 0;
    uint32_t delta_ms = 16; // ~60 FPS frame tick slice

    if (g_webAppManager) {
        g_webAppManager->update(delta_ms);
    }
}

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    int load_vxp_from_memory(const char* file_path) {
        spdlog::info("Loading VXP application in WASM: {}", file_path);
        if (g_webAppManager) {
            g_webAppManager->add_app_for_launch(file_path, false);
            return 1;
        }
        return 0;
    }
}

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Initializing MREmu WebAssembly Core Subsystems...");

    static AppManager appManager;
    g_webAppManager = &appManager;

    Memory::init(32 * 1024 * 1024);
    Cpu::init();
    Bridge::init();

    MREngine::SIM::init();
    MREngine::System::init();
    MREngine::CharSet::init();
    MREngine::AppAudio::init();
    MREngine::IO::init();

    spdlog::info("MREmu Core initialized successfully for WebAssembly.");

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(web_main_loop, 0, 1);
#else
    spdlog::info("Web main loop skipped outside Emscripten environment.");
#endif

    return 0;
}
