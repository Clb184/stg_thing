#include "Command.hpp"
#include "unordered_map"
#include "map"
#include "string"
#include "vector"

/*
* As of now, the base commands ocupy at least 128 spaces
* thus, leaving other 128 slots for new commands
* All of it starts at 0x80 (128), these commands can also
* arguments and also not
* These extra commands should be written in a JSON added via
* the "m" switch, and the syntax is simple:
*
* "cmd" : [
*            {
*		"name" : "command_name", <--- Note that command names shouldn't
*						 have spaces in between, this key is mandatory
*
*		"value" : "0xXX", <--- Command values are in base 16 only (hex)
*					default value is 0x80 and it increases by one if
*					following command declarations miss this key
*					This key is not mandatory
*
*		"args" : [           <--- Argument array, the following values as shown
*			"register",		 are admitted. If missing key, no arguments
*			"number",		are added
*			"integer",
*			"float",
*			"address"
*		]
*	     }
*    ]
*/

inline std::unordered_map<std::string, uint8_t> cmd2byteEx = {}; // Table to be filled with extra commands
inline const std::unordered_map<std::string, uint8_t> cmd2byte = { // Table with base command values
	{"nop", XASM2_NOP},
	{"load", XASM2_LOAD},
	{"loadc", XASM2_LOADC},
	{"store", XASM2_STORE},
	{"loadm", XASM2_LOADM},
	{"loadg", XASM2_LOADG},
	{"storem", XASM2_STOREM},
	{"storeg", XASM2_STOREG},
 	{"set", XASM2_SET},
	{"mov", XASM2_MOV},
	{"push", XASM2_PUSH},
	{"pop", XASM2_POP},
	{"pop2", XASM2_POP2},

	{"enter", XASM2_ENTER},
	{"leave", XASM2_LEAVE},
	{"call", XASM2_CALL},
	{"callt", XASM2_CALLT},
	{"callf", XASM2_CALLF},
	{"ret", XASM2_RET},
	{"jmp", XASM2_JMP},
	{"jmpt", XASM2_JMPT},
	{"jmpf", XASM2_JMPF},
	{"wait", XASM2_WAIT},
	{"halt", XASM2_HALT},
	{"exit", XASM2_EXIT},
	{"sti", XASM2_STI},
	{"cli", XASM2_CLI},
		
	{"add", XASM2_ADD},
	{"addc", XASM2_ADDC},
	{"sub", XASM2_SUB},
	{"subc", XASM2_SUBC},
	{"mul", XASM2_MUL},
	{"mulc", XASM2_MULC},
	{"div", XASM2_DIV},
	{"divc", XASM2_DIVC},
	{"mod", XASM2_MOD},
	{"modc", XASM2_MODC},
	{"sqrt", XASM2_SQRT},
	{"itof", XASM2_ITOF},
	{"inc", XASM2_INC},
	{"dec", XASM2_DEC},
	{"great", XASM2_GREAT},
	{"greatc", XASM2_GREATC},
	{"geq", XASM2_GEQ},
	{"geqc", XASM2_GEQC},
	{"less", XASM2_LESS},
	{"lessc", XASM2_LESSC},
	{"leq", XASM2_LEQ},
	{"leqc", XASM2_LEQC},
	{"eq", XASM2_EQ},
	{"eqc", XASM2_EQC},
	{"neq", XASM2_NEQ},
	{"neqc", XASM2_NEQC},
	{"neg", XASM2_NEG},
	{"and", XASM2_AND},
	{"andc", XASM2_ANDC},
	{"or", XASM2_OR},
	{"orc", XASM2_ORC},
	{"xor", XASM2_XOR},
	{"xorc", XASM2_XORC},
	{"not", XASM2_NOT},
	{"rand", XASM2_RAND},
	{"abs", XASM2_ABS},
	{"shl", XASM2_SHL},
	{"shlc", XASM2_SHLC},
	{"shr", XASM2_SHR},
	{"shrc", XASM2_SHRC},
	{"min", XASM2_MIN},
	{"minc", XASM2_MINC},
	{"max", XASM2_MAX},
	{"maxc", XASM2_MAXC},
	
	{"addf", XASM2_ADDF},
	{"addfc", XASM2_ADDFC},
	{"subf", XASM2_SUBF},
	{"subfc", XASM2_SUBFC},
	{"mulf", XASM2_MULF},
	{"mulfc", XASM2_MULFC},
	{"divf", XASM2_DIVF},
	{"divfc", XASM2_DIVFC},
	{"modf", XASM2_MODF},
	{"modfc", XASM2_MODFC},
	{"sqrtf", XASM2_SQRTF},
	{"ftoi", XASM2_FTOI},
	{"greatf", XASM2_GREATF},
	{"greatfc", XASM2_GREATFC},
	{"geqf", XASM2_GEQF},
	{"geqfc", XASM2_GEQFC},
	{"lessf", XASM2_LESSF},
	{"lessfc", XASM2_LESSFC},
	{"leqf", XASM2_LEQF},
	{"leqfc", XASM2_LEQFC},
	{"eqf", XASM2_EQF},
	{"eqfc", XASM2_EQFC},
	{"neqf", XASM2_NEQF},
	{"neqfc", XASM2_NEQFC},
	{"negf", XASM2_NEGF},
	{"randf", XASM2_RANDF},
	{"randf2", XASM2_RANDF2},
	{"randang", XASM2_RANDANG},
	{"atan2", XASM2_ATAN2},
	{"sin", XASM2_SIN},
	{"cos", XASM2_COS},
	{"normrad", XASM2_NORMRAD},
	{"absf", XASM2_ABSF},
	{"minf", XASM2_MINF},
	{"minfc", XASM2_MINFC},
	{"maxf", XASM2_MAXF},
	{"maxfc", XASM2_MAXFC},
};

// Argument types
enum ARG_TYPE : uint8_t {
	ARG_REGISTER = 0,
	ARG_NUMBER,
	ARG_INTEGER,
	ARG_FLOAT,
	ARG_ADDRESS,
	ARG_UNKNOWN,
};

inline std::map<uint8_t, std::vector<ARG_TYPE>> g_ExtraCmdArgs= {}; // Extra commands arguments
inline const std::map<uint8_t, std::vector<ARG_TYPE>> g_Args = { // Default base commands
	{XASM2_LOAD, {ARG_REGISTER, ARG_INTEGER}},
	{XASM2_LOADC, {ARG_REGISTER, ARG_NUMBER}},
	{XASM2_STORE, {ARG_INTEGER}},
	{XASM2_LOADM, {ARG_REGISTER, ARG_INTEGER}},
	{XASM2_LOADG, {ARG_REGISTER, ARG_INTEGER}},
	{XASM2_STOREM, {ARG_INTEGER}},
	{XASM2_STOREG, {ARG_INTEGER}},
	{XASM2_SET, {ARG_INTEGER, ARG_NUMBER}},
	{XASM2_MOV, {ARG_INTEGER, ARG_INTEGER}},
	{XASM2_POP, {ARG_REGISTER}},

	{XASM2_ENTER, {ARG_INTEGER}},
	{XASM2_CALL, {ARG_ADDRESS}},
	{XASM2_CALLT, {ARG_ADDRESS}},
	{XASM2_CALLF, {ARG_ADDRESS}},
	{XASM2_JMP, {ARG_ADDRESS}},
	{XASM2_JMPT, {ARG_ADDRESS}},
	{XASM2_JMPF, {ARG_ADDRESS}},
	{XASM2_WAIT, {ARG_FLOAT}},
	{XASM2_STI, {ARG_ADDRESS}},

	{XASM2_ADDC, {ARG_INTEGER}},
	{XASM2_SUBC, {ARG_INTEGER}},
	{XASM2_MULC, {ARG_INTEGER}},
	{XASM2_DIVC, {ARG_INTEGER}},
	{XASM2_MODC, {ARG_INTEGER}},
	{XASM2_GREATC, {ARG_INTEGER}},
	{XASM2_GEQC, {ARG_INTEGER}},
	{XASM2_LESSC, {ARG_INTEGER}},
	{XASM2_LEQC, {ARG_INTEGER}},
	{XASM2_EQC, {ARG_INTEGER}},
	{XASM2_NEQC, {ARG_INTEGER}},
	{XASM2_ANDC, {ARG_INTEGER}},
	{XASM2_ORC, {ARG_INTEGER}},
	{XASM2_XORC, {ARG_INTEGER}},
	{XASM2_SHLC, {ARG_INTEGER}},
	{XASM2_SHRC, {ARG_INTEGER}},
	{XASM2_MINC, {ARG_INTEGER}},
	{XASM2_MAXC, {ARG_INTEGER}},

	{XASM2_ADDFC, {ARG_INTEGER}},
	{XASM2_SUBFC, {ARG_INTEGER}},
	{XASM2_MULFC, {ARG_INTEGER}},
	{XASM2_DIVFC, {ARG_INTEGER}},
	{XASM2_MODFC, {ARG_INTEGER}},
	{XASM2_GREATFC, {ARG_INTEGER}},
	{XASM2_GEQFC, {ARG_INTEGER}},
	{XASM2_LESSFC, {ARG_INTEGER}},
	{XASM2_LEQFC, {ARG_INTEGER}},
	{XASM2_EQFC, {ARG_INTEGER}},
	{XASM2_NEQFC, {ARG_INTEGER}},
	{XASM2_MINFC, {ARG_INTEGER}},
	{XASM2_MAXFC, {ARG_INTEGER}},
};

// Symbols, like labels and procedures
inline std::unordered_map<std::string, std::vector<uint32_t>> g_SymbolTable = {};
inline std::unordered_map<std::string, uint32_t> g_AddressTable = {};

// Constants
inline std::map<std::string, std::string> g_ValueTable = {};
