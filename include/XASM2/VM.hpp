#ifndef XASM2VM_INCLUDED
#define XASM2VM_INCLUDED

#include "cstdint"

const int XASM2_STACK_SIZE = 16;

union xasm2_num_t {
	int32_t i;
	float f;

	constexpr xasm2_num_t() : i(0) { }
	constexpr xasm2_num_t(int x) : i(x) { }
	constexpr xasm2_num_t(unsigned int x) : i(x) { }
	constexpr xasm2_num_t(short x) : i(x) { }
	constexpr xasm2_num_t(unsigned short x) : i(x) { }
	constexpr xasm2_num_t(char x) : i(x) { }
	constexpr xasm2_num_t(unsigned char x) : i(x) { }

	constexpr xasm2_num_t(float x) :f(x) { }
	constexpr xasm2_num_t(double x) :f(x)  { }

	constexpr int operator=(int x) { i = x; return x; }
	constexpr int operator=(unsigned int x) { i = x; return x; }
	constexpr float operator=(float x) { f = x; return x; }
	constexpr float operator=(double x) { f = x; return x; }

	constexpr operator int() { return i; }
	constexpr operator unsigned int() { return i; }
	constexpr operator float() { return f; }
	constexpr operator double() { return f; }
};

enum XASM2VM_FLAG : uint32_t {
	XASM2VM_HALT = 1,
	XASM2VM_TERMINATE,
};

struct xasm2_vm_t {
	uint8_t* src_cmd = nullptr;
	uint8_t* cmd = nullptr;
	int* member_reg = nullptr;
	int* global_reg = nullptr;
	uint32_t flags = 0;
	uint32_t interrupt = 0;
	float wait_time = 0.0f;
	float life_time = 0.0f;
	xasm2_num_t r1 = 0;
	xasm2_num_t r2 = 0;
	xasm2_num_t r3 = 0;
	xasm2_num_t r4 = 0;
	uint32_t frame_ptr = 0;
	uint32_t stack_ptr = 0;
	xasm2_num_t stack[XASM2_STACK_SIZE];
};

typedef int(*xasm2_vm_ext)(uint8_t, xasm2_vm_t*, void*);

void XASM2RandomInit(uint64_t seed);

int XASM2Move(xasm2_vm_t* vm, float dt, xasm2_vm_ext extension, void* data);

int XASM2TriggerInterrupt(xasm2_vm_t* vm);

#endif
