#ifndef XASM2VM_INCLUDED
#define XASM2VM_INCLUDED

#include "stdint"

const int XASM2_STACK_SIZE = 64;

typedef union {
	int32_t i;
	float f;
} xasm2_num_t;

typedef struct {
	uint8_t* cmd = nullptr;
	xasm2_num_t r1 = 0;
	xasm2_num_t r2 = 0;
	xasm2_num_t r3 = 0;
	xasm2_num_t r4 = 0;
	uint32_t frame_ptr = 0;
	uint32_t stack_ptr = 0;
	xasm2_num_t stack[XASM2_STACK_SIZE];
} xasm2_vm_t;

int XASM2Move(xasm2_vmt_* vm, float dt);

#endif
