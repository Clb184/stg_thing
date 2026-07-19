#include "GameCore.hpp"

/*
void Move(window_t* window, float dt, void* data) {
	XASM2Move(&vm, dt, nullptr, nullptr);
	if(!(vm.flags & (XASM2VM_TERMINATE | XASM2VM_HALT))) {
		if(vm.wait_time <= 0.0) {
			printf("r1: %d dt: %.4f\n", vm.r1.i, dt);
		}
	}
}*/


int main() {
	GameCore* core = new GameCore();

	if(nullptr == core) {
		printf("Out of memory\n");
		return -1;
	}

	if(false == core->InitGame()) {
		printf("Failed to initialize game\n");
		return -1;
	}
	return 0;
}
