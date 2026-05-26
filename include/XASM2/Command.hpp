#ifndef XASM2COMMAND_INCLUDED
#define XASM2COMMAND_INCLUDED

#include "stdint"

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
	XASM2_LOAD,
	XASM2_LOADC,
	XASM2_STORE,
	XASM2_LOADM,
	XASM2_LOADG,
	XASM2_STOREM,
	XASM2_STOREG,
	XASM2_STKAL,
	XASM2_STKDAL,
	XASM2_PUSH,
	XASM2_POP,
	XASM2_POP2,

	// Stack manipulation and control flow
	XASM2_ENTER = 0x10,
	XASM2_LEAVE,
	XASM2_CALL,
	XASM2_CALLT,
	XASM2_CALLF,
	XASM2_JMP,
	XASM2_JMPT,
	XASM2_JMPF,
	XASM2_WAIT,

	// Math (Integer)
	XASM2_ADD = 0x20,
	XASM2_SUB,
	XASM2_MUL,
	XASM2_DIV,
	XASM2_MOD,
	XASM2_SQRT,
	XASM2_ITOF,
	XASM2_INC,
	XASM2_DEC,
};

#endif
