#include "Cpu.h"
#include "Memory.h"
#include "Log.h"
#include <spdlog/spdlog.h>

#ifndef __EMSCRIPTEN__
#include <unicorn/unicorn.h>
uc_engine* uc = 0;
#else
// Stub Unicorn handle for WASM target until Unicorn v2.x WASM interpreter integration
void* uc = nullptr;
#endif

namespace Cpu {
	void* stack_p = 0;
	size_t stack_size = 128 * 1024;

	int cur_cpu_id = -1;

	struct hook_t {
		int type;
		void* callback;
		void* user_data;
		uint64_t begin;
		uint64_t end;
	};

	std::vector<hook_t> hooks;

	void printREG(void* uc_ptr) {
		spdlog::info("Cpu::printREG stub called");
	}

	void imgui_REG() {
	}

	void init() {
		stack_p = Memory::shared_malloc(stack_size);
		if (stack_p == 0)
			abort();

		push_cpu();
	}

	void push_cpu() {
		cur_cpu_id++;
	}

	void pop_cpu() {
		if (cur_cpu_id > 0)
			--cur_cpu_id;
	}

	void add_hook(int type, void* callback, void* user_data, uint64_t begin, uint64_t end) {
		hook_t hook = { type, callback, user_data, begin, end };
		hooks.push_back(hook);
	}

	void trace_on() {
	}

	void stop() {
	}
};
