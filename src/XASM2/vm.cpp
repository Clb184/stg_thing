#include "XASM2/Command.hpp"
#include "XASM2/VM.hpp"
#include "cstdio"
#include "cstdlib"
#include "cmath"

#include "cassert"
#include "random"

// Random devices
std::mt19937_64 g_RandomDevice;
std::uniform_int_distribution<int> g_RandIntGenerator;
std::uniform_real_distribution<float> g_RandFloatGenerator(0.0f, 1.0f);
std::uniform_real_distribution<float> g_RandFloat2Generator(-1.0f, 1.0f);
std::uniform_real_distribution<float> g_RandAngleGenerator(-3.14159f, 3.14159f);

void XASM2RandomInit(uint64_t seed) {
	g_RandomDevice.seed(seed);
}

int XASM2Move(xasm2_vm_t* vm, float dt, xasm2_vm_ext extension, void* data) {
	assert(nullptr != vm);
	assert(nullptr != vm->src_cmd);
	assert(nullptr != vm->cmd);
	assert(nullptr != vm->stack);
	// Or if VM is halted or terminated
	if((XASM2VM_HALT | XASM2VM_TERMINATE) & vm->flags) {
		return 0;
	}

	//Before moving logic, check if there's time pending
	if(vm->wait_time > 0) {
		vm->wait_time -= dt;
		return 0;
	} else {
	//	printf("finish sleep\n");
	}

start:
	// Setup all
	uint8_t* cmd = vm->cmd;

	// Begin cmd
	switch(*cmd) {
	case XASM2_NOP:
		vm->cmd++;
		goto start;

	case XASM2_LOAD:
		vm->cmd++;
		switch(*vm->cmd) {
			case 0: vm->r1 = vm->stack[vm->frame_ptr + *(int*)(vm->cmd + 1)]; break;
			case 1: vm->r2 = vm->stack[vm->frame_ptr + *(int*)(vm->cmd + 1)]; break;
			case 2: vm->r3 = vm->stack[vm->frame_ptr + *(int*)(vm->cmd + 1)]; break;
			case 3: vm->r4 = vm->stack[vm->frame_ptr + *(int*)(vm->cmd + 1)]; break;
		}
		
		vm->cmd += 1 + sizeof(int);
		goto start;

	case XASM2_LOADC:
		vm->cmd++;
		switch(*vm->cmd) {
			case 0: vm->r1 = *(xasm2_num_t*)(vm->cmd + 1); break;
			case 1: vm->r2 = *(xasm2_num_t*)(vm->cmd + 1); break;
			case 2: vm->r3 = *(xasm2_num_t*)(vm->cmd + 1); break;
			case 3: vm->r4 = *(xasm2_num_t*)(vm->cmd + 1); break;
		}
		
		vm->cmd += 1 + sizeof(int);
		goto start;
	
	case XASM2_STORE:
		vm->cmd++;
		vm->stack[vm->frame_ptr + *(int*)vm->cmd] = vm->r1;
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_LOADM:
		assert(nullptr != vm->member_reg);
		vm->cmd++;
		switch(*vm->cmd) {
			case 0: vm->r1 = (xasm2_num_t)vm->member_reg[*(int*)(vm->cmd + 1)]; break;
			case 1: vm->r2 = (xasm2_num_t)vm->member_reg[*(int*)(vm->cmd + 1)]; break;
			case 2: vm->r3 = (xasm2_num_t)vm->member_reg[*(int*)(vm->cmd + 1)]; break;
			case 3: vm->r4 = (xasm2_num_t)vm->member_reg[*(int*)(vm->cmd + 1)]; break;
		}
		vm->cmd += 1 + sizeof(int);
		goto start;
	case XASM2_LOADG:
		assert(nullptr != vm->global_reg);
		vm->cmd++;
		switch(*vm->cmd) {
			case 0: vm->r1 = (xasm2_num_t)vm->global_reg[*(int*)(vm->cmd + 1)]; break;
			case 1: vm->r2 = (xasm2_num_t)vm->global_reg[*(int*)(vm->cmd + 1)]; break;
			case 2: vm->r3 = (xasm2_num_t)vm->global_reg[*(int*)(vm->cmd + 1)]; break;
			case 3: vm->r4 = (xasm2_num_t)vm->global_reg[*(int*)(vm->cmd + 1)]; break;
		}
		vm->cmd += 1 + sizeof(int);
		goto start;

	case XASM2_STOREM:
		assert(nullptr != vm->member_reg);
		vm->cmd++;
		vm->member_reg[*(int*)vm->cmd] = vm->r1;
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_STOREG:
		assert(nullptr != vm->global_reg);
		vm->cmd++;
		vm->global_reg[*(int*)vm->cmd] = vm->r1;
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_SET:
		vm->cmd++;
		vm->stack[vm->frame_ptr + *(int*)vm->cmd] = *(xasm2_num_t*)(vm->cmd + sizeof(int));
		vm->cmd += sizeof(int) + sizeof(xasm2_num_t);
		goto start;
	
	case XASM2_MOV:
		vm->cmd++;
		vm->stack[vm->frame_ptr + *(int*)vm->cmd] = vm->stack[vm->frame_ptr + *(int*)(vm->cmd + sizeof(int))];
		vm->cmd += 2 * sizeof(int);
		goto start;

	case XASM2_PUSH:
		vm->cmd++;
		vm->stack[vm->stack_ptr++] = vm->r1;
		goto start;
	
	case XASM2_POP:
		vm->cmd++;
		vm->stack_ptr--;
		switch(*vm->cmd) {
			case 0: vm->r1 = vm->stack[vm->stack_ptr]; break;
			case 1: vm->r2 = vm->stack[vm->stack_ptr]; break;
			case 2: vm->r3 = vm->stack[vm->stack_ptr]; break;
			case 3: vm->r4 = vm->stack[vm->stack_ptr]; break;
		}

		vm->cmd++;
		goto start;
	
	case XASM2_POP2:
		vm->cmd++;
		vm->stack_ptr--;
		goto start;

	case XASM2_ENTER:
		vm->cmd++;
		vm->stack_ptr += *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_LEAVE:
		vm->cmd++;
		goto start;

	// Call procedure
	case XASM2_CALLT:
		if(vm->r1.i) goto call_proc;
		vm->cmd += 1 + sizeof(int);
		goto start;

	case XASM2_CALLF:
		if(!vm->r1.i) goto call_proc;
		vm->cmd += 1 + sizeof(int);
		goto start;


	case XASM2_CALL:
	call_proc:
		vm->cmd++;
		vm->stack[vm->stack_ptr++] = int((vm->cmd + 4) - vm->src_cmd);
		printf("Return offset -> %x\n", vm->stack[vm->stack_ptr - 1]);
		printf("Src script: %p\n", vm->src_cmd);
		printf("Going to: %p\n", vm->src_cmd + *(uint32_t*)vm->cmd);
		printf("This: %p, Return: %x\n", vm->cmd - 1, int((vm->cmd + 4) - vm->src_cmd));
		vm->stack[vm->stack_ptr++] = vm->frame_ptr;
		vm->frame_ptr = vm->stack_ptr;

		vm->cmd = vm->src_cmd + *(uint32_t*)vm->cmd;
		goto start;

	case XASM2_RET:
		vm->stack_ptr = vm->frame_ptr;
		printf("Return offset -> %x\n", vm->stack[vm->stack_ptr - 2]);
		vm->stack_ptr--;
		vm->frame_ptr = vm->stack[vm->stack_ptr--];
		printf("Returning to %p\n", vm->src_cmd + int(vm->stack[vm->stack_ptr]));
		vm->cmd = vm->src_cmd + int(vm->stack[vm->stack_ptr]);
		
		goto start;

	// Jump to offset
	case XASM2_JMPT:
		if(vm->r1.i) goto jmp_offset;
		vm->cmd += 1 + sizeof(int);
		goto start;

	case XASM2_JMPF:
		if(!vm->r1.i) goto jmp_offset;
		vm->cmd += 1 + sizeof(int);
		goto start;


	case XASM2_JMP:
	jmp_offset:
		vm->cmd++;
		vm->cmd = vm->src_cmd + *(uint8_t*)vm->cmd;
		goto start;

	case XASM2_WAIT:
		vm->cmd++;
		vm->wait_time = *(float*)vm->cmd;
		vm->cmd += sizeof(float);
		goto exit;
	
	case XASM2_HALT:
		vm->cmd++;
		vm->flags |= XASM2VM_HALT;
		printf("VM WILL HALT P: %p\n", vm);
		goto exit;

	case XASM2_EXIT:
		vm->cmd++;
		vm->flags |= XASM2VM_TERMINATE;
		goto terminate;	

	// Arithmetic
	case XASM2_ADD:
		vm->cmd++;
		vm->r1.i += vm->r2.i;
		goto start;

	case XASM2_ADDC:
		vm->cmd++;
		vm->r1.i += *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_SUB:
		vm->cmd++;
		vm->r1.i -= vm->r2.i;
		goto start;

	case XASM2_SUBC:
		vm->cmd++;
		vm->r1.i -= *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_MUL:
		vm->cmd++;
		vm->r1.i *= vm->r2.i;
		goto start;

	case XASM2_MULC:
		vm->cmd++;
		vm->r1.i *= *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_DIV:
		vm->cmd++;
		vm->r1.i /= vm->r2.i;
		goto start;

	case XASM2_DIVC:
		vm->cmd++;
		vm->r1.i /= *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_MOD:
		vm->cmd++;
		vm->r1.i %= vm->r2.i;
		goto start;
	
	case XASM2_MODC:
		vm->cmd++;
		vm->r1.i %= *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_SQRT:
		vm->cmd++;
		vm->r1.i = sqrt(vm->r1.i);
		goto start;
	
	case XASM2_ITOF:
		vm->cmd++;
		vm->r1.f = float(vm->r1.i);
		goto start;

	case XASM2_INC:
		vm->cmd++;
		vm->r1.i++;
		goto start;

	case XASM2_DEC:
		vm->cmd++;
		vm->r1.i--;
		goto start;
	
	case XASM2_GREAT:
		vm->cmd++;
		vm->r1.i = vm->r1.i > vm->r2.i;
		goto start;
	
	case XASM2_GREATC:
		vm->cmd++;
		vm->r1.i = vm->r1.i > *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_GEQ:
		vm->cmd++;
		vm->r1.i = vm->r1.i >= vm->r2.i;
		goto start;
	
	case XASM2_GEQC:
		vm->cmd++;
		vm->r1.i = vm->r1.i >= *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_LESS:
		vm->cmd++;
		vm->r1.i = vm->r1.i < vm->r2.i;
		goto start;
	
	case XASM2_LESSC:
		vm->cmd++;
		vm->r1.i = vm->r1.i < *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_LEQ:
		vm->cmd++;
		vm->r1.i = vm->r1.i <= vm->r2.i;
		goto start;
	
	case XASM2_LEQC:
		vm->cmd++;
		vm->r1.i = vm->r1.i <= *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_EQ:
		vm->cmd++;
		vm->r1.i = vm->r1.i == vm->r2.i;
		goto start;
	
	case XASM2_EQC:
		vm->cmd++;
		vm->r1.i = vm->r1.i == *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_NEQ:
		vm->cmd++;
		vm->r1.i = vm->r1.i != vm->r2.i;
		goto start;
	
	case XASM2_NEQC:
		vm->cmd++;
		vm->r1.i = vm->r1.i != *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_NEG:
		vm->cmd++;
		vm->r1.i = -vm->r1.i;
		goto start;
	
	case XASM2_AND:
		vm->cmd++;
		vm->r1.i &= vm->r2.i;
		goto start;
	
	case XASM2_ANDC:
		vm->cmd++;
		vm->r1.i &= *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_OR:
		vm->cmd++;
		vm->r1.i |= vm->r2.i;
		goto start;
	
	case XASM2_ORC:
		vm->cmd++;
		vm->r1.i |= *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_XOR:
		vm->cmd++;
		vm->r1.i ^= vm->r2.i;
		goto start;
	
	case XASM2_XORC:
		vm->cmd++;
		vm->r1.i ^= *(int*)(vm->cmd);
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_NOT:
		vm->cmd++;
		vm->r1.i = !vm->r1.i;
		goto start;
	
	case XASM2_RAND:
		vm->cmd++;
		vm->r1.i = g_RandIntGenerator(g_RandomDevice);
		goto start;

	case XASM2_ABS:
		vm->cmd++;
		if(vm->r1.i < 0) vm->r1.i = -vm->r1.i;
		goto start;
	
	case XASM2_MIN:
		vm->cmd++;
		vm->r1.i = (vm->r1.i < vm->r2.i) ? vm->r1.i : vm->r2.i;
		goto start;
	
	case XASM2_MINC:
		vm->cmd++;
		vm->r1.i = (vm->r1.i < *(int*)vm->cmd) ? vm->r1.i : *(int*)vm->cmd;
		vm->cmd += sizeof(int);
		goto start;

	case XASM2_MAX:
		vm->cmd++;
		vm->r1.i = (vm->r1.i > vm->r2.i) ? vm->r1.i : vm->r2.i;
		goto start;
	
	case XASM2_MAXC:
		vm->cmd++;
		vm->r1.i = (vm->r1.i > *(int*)vm->cmd) ? vm->r1.i : *(int*)vm->cmd;
		vm->cmd += sizeof(int);
		goto start;

	// Float ops
	case XASM2_ADDF:
		vm->cmd++;
		vm->r1.f += vm->r2.f;
		goto start;
	
	case XASM2_ADDFC:
		vm->cmd++;
		vm->r1.f += *(float*)vm->cmd;
		vm->cmd += sizeof(float);
		goto start;

	case XASM2_SUBF:
		vm->cmd++;
		vm->r1.f -= vm->r2.f;
		goto start;
	
	case XASM2_SUBFC:
		vm->cmd++;
		vm->r1.f -= *(float*)vm->cmd;
		vm->cmd += sizeof(float);
		goto start;

	case XASM2_MULF:
		vm->cmd++;
		vm->r1.f *= vm->r2.f;
		goto start;
	
	case XASM2_MULFC:
		vm->cmd++;
		vm->r1.f *= *(float*)vm->cmd;
		vm->cmd += sizeof(float);
		goto start;

	case XASM2_DIVF:
		vm->cmd++;
		vm->r1.f /= vm->r2.f;
		goto start;
	
	case XASM2_DIVFC:
		vm->cmd++;
		vm->r1.f /= *(float*)vm->cmd;
		vm->cmd += sizeof(float);
		goto start;

	case XASM2_MODF:
		vm->cmd++;
		vm->r1.f = fmodf(vm->r1.f, vm->r2.f);
		goto start;
	
	case XASM2_MODFC:
		vm->cmd++;
		vm->r1.f = fmodf(vm->r1.f, *(float*)vm->cmd);
		vm->cmd += sizeof(float);
		goto start;

	case XASM2_SQRTF:
		vm->cmd++;
		vm->r1.f = sqrtf(vm->r1.f);
		goto start;
	
	case XASM2_FTOI:
		vm->cmd++;
		vm->r1.i = int(vm->r1.f);
		goto start;
	
	case XASM2_GREATF:
		vm->cmd++;
		vm->r1.f = vm->r1.f > vm->r2.f;
		goto start;
	
	case XASM2_GREATFC:
		vm->cmd++;
		vm->r1.f = vm->r1.f > *(float*)vm->cmd;
		vm->cmd += sizeof(float);
		goto start;

	case XASM2_GEQF:
		vm->cmd++;
		vm->r1.f = vm->r1.f >= vm->r2.f;
		goto start;
	
	case XASM2_GEQFC:
		vm->cmd++;
		vm->r1.f = vm->r1.f >= *(float*)vm->cmd;
		vm->cmd += sizeof(float);
		goto start;

	case XASM2_LESSF:
		vm->cmd++;
		vm->r1.f = vm->r1.f < vm->r2.f;
		goto start;
	
	case XASM2_LESSFC:
		vm->cmd++;
		vm->r1.f = vm->r1.f < *(float*)vm->cmd;
		vm->cmd += sizeof(float);
		goto start;

	case XASM2_LEQF:
		vm->cmd++;
		vm->r1.f = vm->r1.f <= vm->r2.f;
		goto start;
	
	case XASM2_LEQFC:
		vm->cmd++;
		vm->r1.f = vm->r1.f <= *(float*)vm->cmd;
		vm->cmd += sizeof(float);
		goto start;

	case XASM2_EQF:
		vm->cmd++;
		vm->r1.f = vm->r1.f == vm->r2.f;
		goto start;
	
	case XASM2_EQFC:
		vm->cmd++;
		vm->r1.f = vm->r1.f == *(float*)vm->cmd;
		vm->cmd += sizeof(float);
		goto start;

	case XASM2_NEQF:
		vm->cmd++;
		vm->r1.f = vm->r1.f != vm->r2.f;
		goto start;
	
	case XASM2_NEQFC:
		vm->cmd++;
		vm->r1.f = vm->r1.f != *(float*)vm->cmd;
		vm->cmd += sizeof(float);
		goto start;

	case XASM2_NEGF:
		vm->cmd++;
		vm->r1.f = -vm->r1.f;
		goto start;
	
	case XASM2_RANDF:
		vm->cmd++;
		vm->r1.f = g_RandFloatGenerator(g_RandomDevice);
		goto start;
	
	case XASM2_RANDF2:
		vm->cmd++;
		vm->r1.f = g_RandFloat2Generator(g_RandomDevice);
		goto start;
	
	case XASM2_RANDANG:
		vm->cmd++;
		vm->r1.f = g_RandAngleGenerator(g_RandomDevice);
		goto start;
	
	case XASM2_ATAN2:
		vm->cmd++;
		vm->r1.f = atan2f(vm->r2.f, vm->r1.f);
		goto start;
	
	case XASM2_SIN:
		vm->cmd++;
		vm->r1.f = sinf(vm->r1.f);
		goto start;
	
	case XASM2_COS:
		vm->cmd++;
		vm->r1.f = cos(vm->r1.f);
		goto start;
	
	case XASM2_NORMRAD:
		vm->cmd++;
		if(vm->r1.f >= 3.14159f || vm->r1.f <= -3.14159f) {
			vm->r1.f = fmodf(vm->r1.f, 3.14159f);
		}
		goto start;
	
	case XASM2_ABSF:
		vm->cmd++;
		if(vm->r1.f < 0.0f) vm->r1.f = -vm->r1.f;
		goto start;
	
	case XASM2_MINF:
		vm->cmd++;
		vm->r1.f = (vm->r1.f < vm->r2.f) ? vm->r1.f : vm->r2.f;
		goto start;
	
	case XASM2_MINFC:
		vm->cmd++;
		vm->r1.f = (vm->r1.f < *(float*)vm->cmd) ? vm->r1.f : *(float*)vm->cmd;
		vm->cmd += sizeof(float);
		goto start;

	case XASM2_MAXF:
		vm->cmd++;
		vm->r1.f = (vm->r1.f > vm->r2.f) ? vm->r1.f : vm->r2.f;
		goto start;
	
	case XASM2_MAXFC:
		vm->cmd++;
		vm->r1.f = (vm->r1.f > *(float*)vm->cmd) ? vm->r1.f : *(float*)vm->cmd;
		vm->cmd += sizeof(float);
		goto start;

	default:
		vm->cmd++;
		if(0 != extension && (0 == extension(*cmd, vm, data))) {
			goto start;
		}
		goto exception;
	}

exception:
	fprintf(stdout, "XASM2 exception | CMD was: 0x%p (0x%x, %d)\n", cmd, *cmd, *cmd);
	fprintf(stdout, "r1 i: %d f: %f\n", vm->r1.i, vm->r1.f);
	fprintf(stdout, "r2 i: %d f: %f\n", vm->r2.i, vm->r2.f);
	fprintf(stdout, "r3 i: %d f: %f\n", vm->r3.i, vm->r3.f);
	fprintf(stdout, "r4 i: %d f: %f\n", vm->r4.i, vm->r4.f);
	fprintf(stdout, "Frame pointer was: %d\n", vm->frame_ptr);
	fprintf(stdout, "Stack pointer was: %d\n", vm->stack_ptr);
terminate:
	fprintf(stdout, "VM WILL TERMINATE\n");
	vm->flags |= XASM2VM_TERMINATE;

exit:

	// A return of 0 means OK
	return 0;
}
