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

	// Math (Integer)
	XASM2_ADD = 0x20, // Add (r1 += r2)
	XASM2_SUB, // Sub (r1 -= r2)
	XASM2_MUL, // Mul (r1 *= r2)
	XASM2_DIV, // Div (r1 /= r2)
	XASM2_MOD, // Modulo (r1 %= r2)
	XASM2_SQRT, // Sqrt (r1 = sqrt(r1))
	XASM2_ITOF, // Itof (r1 = itof(r1))
	XASM2_INC, // r1++
	XASM2_DEC, // r1--
};

#endif
