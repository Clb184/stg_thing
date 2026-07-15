#include "XPFW.h"
#include "XASM2/VM.hpp"



size_t LoadData(const char* name, char** data) {
	FILE* fp = fopen(name, "rb");
	if(nullptr != fp) {
		fseek(fp, 0, SEEK_END);
		size_t sz = ftell(fp);
		rewind(fp);
		*data = (char*)calloc(sz, 1);
		fread(*data, sz, 1, fp);
		fclose(fp);
		return sz;
	} else {
		return 0;
	}
}

inline xasm2_vm_t vm;

void Move(window_t* window, float dt, void* data) {
	XASM2Move(&vm, dt, nullptr, nullptr);
	if(!(vm.flags & (XASM2VM_TERMINATE | XASM2VM_HALT))) {
		if(vm.wait_time <= 0.0) {
			printf("r1: %d dt: %.4f\n", vm.r1.i, dt);
		}
	}
}

void Draw(window_t* window, float dt, void* data) {

}

int main() {
	window_t window;
	char* dat = nullptr;
	size_t sz = LoadData("data.bin", &dat);
	vm.src_cmd = (uint8_t*)dat;
	vm.cmd = (uint8_t*)dat + *(uint32_t*)(dat + 8);
	XASM2RandomInit(123);
	if(false == CreateGLWindow("Test", 1280, 960, false, &window)) {
		printf("Failed to initialize game\n");
		return -1;
	}
	RunMainLoopDT(&window, nullptr, Move, Draw);
	return 0;
}
