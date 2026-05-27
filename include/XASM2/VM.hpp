#ifndef XASM2VM_INCLUDED
#define XASM2VM_INCLUDED

#include "cstdint"

const int XASM2_STACK_SIZE = 64;

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
	constexpr operator float() { return f; }
	constexpr operator double() { return f; }
};

typedef struct {
	uint8_t* src_cmd = nullptr;
	uint8_t* cmd = nullptr;
	xasm2_num_t r1 = 0;
	xasm2_num_t r2 = 0;
	xasm2_num_t r3 = 0;
	xasm2_num_t r4 = 0;
	uint32_t frame_ptr = 0;
	uint32_t stack_ptr = 0;
	xasm2_num_t stack[XASM2_STACK_SIZE];
} xasm2_vm_t;

int XASM2Move(xasm2_vm_t* vm, float dt);

#endif
