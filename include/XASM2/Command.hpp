#ifndef XASM2COMMAND_INCLUDED
#define XASM2COMMAND_INCLUDED

#include "cstdint"

/*
 * This is XASM2, like an spiritual successor for a past assembly like language
 * I made for my first game engine when I was learning how to use OpenGL.
 *
 * I'm still a novice, but I think I got better with the past of time, so it should
 * suffice for this.
 */

enum XASM2_CMD : uint8_t {
	// LOAD STORE operations (Also no-op)
	XASM2_NOP = 0x00,
	XASM2_LOAD, // (byte) Reg, (int) Frame offset // Load value from stack
	XASM2_LOADC, // (byte) Reg, (number) value // Load constant into stack
	XASM2_STORE, // (int) Frame offset // Store value into stack
	XASM2_LOADM, // (byte) Reg, (int) Member offset // Load value from member
	XASM2_LOADG, // (byte) Reg, (int) Global offset // Load value from global
	XASM2_STOREM, // (int) Member offset // Store value on member
	XASM2_STOREG, // (int) Global offset // Store value on global
	XASM2_SET, // (int) Frame offset, (number) value // Set stack offset to value
	XASM2_MOV, // (int) Dst Frame offset, (int) Src Frame offset // Set stack offset to other offset value
	XASM2_PUSH, // Push reg 1 onto stack
	XASM2_POP, // (byte) reg // Pop onto reg
	XASM2_POP2, // Pop value

	// Stack manipulation and control flow
	XASM2_ENTER = 0x10, // (byte) cnt // Make space in stack for local variables
	XASM2_LEAVE, // Recover space in stack
	XASM2_CALL, // (int) offset // Call procedure
	XASM2_CALLT, // (int) offset // Call if true
	XASM2_CALLF, // (int) offset // Call if false
	XASM2_RET, // Return from procedure
	XASM2_JMP, // (int) offset // Jump to offset
	XASM2_JMPT, // (int) offset // Jump if true
	XASM2_JMPF, // (int) offset // Jump if false
	XASM2_WAIT, // (float) time // Wait for seconds
	XASM2_HALT, // Stops the VM
	XASM2_EXIT, // Terminates the VM

	// Math (Integer)
	XASM2_ADD = 0x20, // r1 += r2
	XASM2_ADDC, // r1 += c
	XASM2_SUB, // r1 -= r2
	XASM2_SUBC, // r1 -= c
	XASM2_MUL, // r1 *= r2
	XASM2_MULC, // r1 *= c
	XASM2_DIV, // r1 /= r2
	XASM2_DIVC, // r1 /= c
	XASM2_MOD, // r1 %= r2
	XASM2_MODC, // r1 %= c
	XASM2_SQRT, // sqrt(r1)
	XASM2_ITOF, // itof(r1)
	XASM2_INC, // r1++
	XASM2_DEC, // r1--
	XASM2_GREAT, // r1 > r2
	XASM2_GREATC, // r1 > c
	XASM2_GEQ, // r1 >= r2
	XASM2_GEQC, // r1 >= c
	XASM2_LESS, // r1 < r2
	XASM2_LESSC, // r1 < c
	XASM2_LEQ, // r1 <= r2
	XASM2_LEQC, // r1 <= c   
	XASM2_EQ, // r1 == r2
	XASM2_EQC, // r1 == c
	XASM2_NEQ, // r1 != r2
	XASM2_NEQC, // r1 != c
	XASM2_NEG, // -r1
	XASM2_AND, // r1 &= r2
	XASM2_ANDC, // r1 &= c
	XASM2_OR, // r1 |= r2
	XASM2_ORC, // r1 |= c
	XASM2_XOR, // r1 ^= r2
	XASM2_XORC, // r1 ^= c
	XASM2_NOT, // !r1
	XASM2_RAND, // Random int from device
	XASM2_ABS, // abs(r1)
	XASM2_SHL, // r1 >>= r2
	XASM2_SHLC, // r1 >>= c
	XASM2_SHR, // r1 <<= r2
	XASM2_SHRC, // r1 <<= c
	XASM2_MIN, // min(r1, r2)
	XASM2_MINC, // min(r1, c)
	XASM2_MAX, // max(r1, r2)
	XASM2_MAXC, // max(r1, c)

	// Math (float)
	XASM2_ADDF = 0x50,
	XASM2_ADDFC,
	XASM2_SUBF,
	XASM2_SUBFC,
	XASM2_MULF,
	XASM2_MULFC,
	XASM2_DIVF,
	XASM2_DIVFC,
	XASM2_MODF,
	XASM2_MODFC,
	XASM2_SQRTF,
	XASM2_FTOI,
	XASM2_GREATF,
	XASM2_GREATFC,
	XASM2_GEQF,
	XASM2_GEQFC,
	XASM2_LESSF,
	XASM2_LESSFC,
	XASM2_LEQF,
	XASM2_LEQFC,
	XASM2_EQF,
	XASM2_EQFC,
	XASM2_NEQF,
	XASM2_NEQFC,
	XASM2_NEGF,
	XASM2_RANDF,
	XASM2_RANDF2,
	XASM2_RANDANG,
	XASM2_ATAN2,
	XASM2_SIN,
	XASM2_COS,
	XASM2_NORMRAD,
	XASM2_ABSF,
	XASM2_MINF, // min(r1, r2)
	XASM2_MINFC, // min(r1, c)
	XASM2_MAXF, // max(r1, r2)
	XASM2_MAXFC, // max(r1, c)

};

#endif
