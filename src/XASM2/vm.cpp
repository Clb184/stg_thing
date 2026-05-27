#include "XASM2/Command.hpp"
#include "XASM2/VM.hpp"

int XASM2Move(xasm2_vm_t* vm, float dt) {
	// Setup all
	uint8_t* cmd = vm->cmd;
	xasm2_num_t r1 = vm->r1, r2 = vm->r2, r3 = vm->r3, r4 = vm->r4;
	uint32_t frame_ptr = vm->frame_ptr, stack_ptr = vm->stack_ptr;
start:
	switch(*cmd) {
	case XASM2_NOP:
		break;
	case XASM2_LOAD:
		cmd++;
		switch(*cmd) {
			case 0: r1 = vm->stack[frame_ptr + *(int*)(cmd + 1)]; break;
			case 1: r2 = vm->stack[frame_ptr + *(int*)(cmd + 1)]; break;
			case 2: r3 = vm->stack[frame_ptr + *(int*)(cmd + 1)]; break;
			case 3: r4 = vm->stack[frame_ptr + *(int*)(cmd + 1)]; break;
		}
		
		cmd += 1 + sizeof(int);
		goto start;

	case XASM2_LOADC:
		cmd++;
		switch(*cmd) {
			case 0: r1 = *(xasm2_num_t*)(cmd + 1); break;
			case 1: r2 = *(xasm2_num_t*)(cmd + 1); break;
			case 2: r3 = *(xasm2_num_t*)(cmd + 1); break;
			case 3: r4 = *(xasm2_num_t*)(cmd + 1); break;
		}
		
		cmd += 1 + sizeof(int);
		goto start;
	
	case XASM2_STORE:
		cmd++;
		vm->stack[frame_ptr + *(int*)cmd] = r1;
		cmd += sizeof(int);
		goto start;

	case XASM2_LOADM:
		cmd++;
		cmd += 1 + sizeof(int);
		goto start;
	case XASM2_LOADG:
		cmd++;
		cmd += 1 + sizeof(int);
		goto start;

	case XASM2_STOREM:
		cmd++;
		cmd += sizeof(int);
		goto start;

	case XASM2_STOREG:
		cmd++;
		cmd += sizeof(int);
		goto start;

	case XASM2_SET:
		cmd++;
		vm->stack[frame_ptr + *(int*)cmd] = *(xasm2_num_t*)(cmd + sizeof(int));
		cmd += sizeof(int) + sizeof(xasm2_num_t);
		goto start;
	
	case XASM2_MOV:
		cmd++;
		vm->stack[frame_ptr + *(int*)cmd] = vm->stack[frame_ptr + *(int*)(cmd + sizeof(int))];
		cmd += 2 * sizeof(int);
		goto start;

	case XASM2_PUSH:
		cmd++;
		vm->stack[stack_ptr++] = r1;
		goto start;
	
	case XASM2_POP:
		cmd++;
		switch(*cmd) {
			case 0: r1 = vm->stack[stack_ptr--]; break;
			case 1: r2 = vm->stack[stack_ptr--]; break;
			case 2: r3 = vm->stack[stack_ptr--]; break;
			case 3: r4 = vm->stack[stack_ptr--]; break;
		}

		cmd++;
		goto start;
	
	case XASM2_POP2:
		cmd++;
		stack_ptr--;
		goto start;

	case XASM2_ENTER:
		cmd++;
		stack_ptr += *(int*)cmd;
		goto start;

	case XASM2_LEAVE:
		cmd++;
		stack_ptr = frame_ptr;
		goto start;

	// Call procedure
	case XASM2_CALLT:
		if(r1.i) goto call_proc;
		cmd += 1 + sizeof(int);
		goto start;

	case XASM2_CALLF:
		if(!r1.i) goto call_proc;
		cmd += 1 + sizeof(int);
		goto start;


	case XASM2_CALL:
	call_proc:
		cmd++;
		vm->stack[stack_ptr++] = int((cmd + 4) - src_cmd);
		vm->stack[stack_ptr++] = frame_ptr;
		frame_ptr = stack_ptr;

		cmd = src_cmd + *(uint8_t*)cmd;
		goto start;

	case XASM2_RET:
		stack_ptr--;
		frame_ptr = vm->stack[stack_ptr--];
		cmd = src_cmd + int(vm->stack[stack_ptr]);
		
		goto start;

	// Jump to offset
	case XASM2_JMPT:
		if(r1.i) goto jmp_offset;
		cmd += 1 + sizeof(int);
		goto start;

	case XASM2_JMPF:
		if(!r1.i) goto jmp_offset;
		cmd += 1 + sizeof(int);
		goto start;


	case XASM2_JMP:
	jmp_offset:
		cmd++;
		cmd = src_cmd + *(uint8_t*)cmd;
		goto start;

	case XASM2_WAIT:
		cmd++;
		cmd += sizeof(float);
		goto start;
	
	// Arithmetic
	case XASM2_ADD:
		cmd++;
		r1.i += r2.i;
		goto start;

	case XASM2_SUB:
		cmd++;
		r1.i -= r2.i;
		goto start;

	case XASM2_MUL:
		cmd++;
		r1.i *= r2.i;
		goto start;

	case XASM2_DIV:
		cmd++;
		r1.i /= r2.i;
		goto start;

	case XASM2_MOD:
		cmd++;
		r1.i %= r2.i;
		goto start;
	
	case XASM2_SQRT:
		cmd++;
		goto start;
	
	case XASM2_ITOF:
		cmd++;
		goto start;

	case XASM2_INC:
		cmd++;
		r1.i++;
		goto start;

	case XASM2_DEC:
		cmd++;
		r1.i--;
		goto start;

	}

	// A return of 0 means OK
	return 0;
}
