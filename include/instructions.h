#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>


typedef enum {
	LDA = 0,
	STA,
	
	LDX,
	STX,
	
	LDY,
	STY,
	
	TAX,
	TXA,
	TAY,
	TYA,

	ADC,
	SBC,

	INC,
	DEC,
	INX,
	DEX,
	INY,
	DEY,

	ASL,
	LSR,
	ROL,
	ROR,

	AND,
	ORA,
	EOR,
	BIT,

	CMP,
	CPX,
	CPY,
	
	BCC,
	BCS,
	BEQ,
	BNE,
	BPL,
	BMI,
	BVC,
	BVS,

	JMP,
	JMPABS,	//JMP (absolute)
	
	JSR,
	RTS,
	
	BRK,
	RTI,

	PHA,
	PLA,
	PHP,
	PLP,
	TXS,
	TSX,
	
	CLC,
	SEC,
	CLI,
	SEI,
	CLD,
	SED,
	CLV,

	NOP

} instruction_t;

typedef enum {
	
	IMPLIED=0,
	ACCUMULATOR,
	IMMEDIATE,
	ZERO_PAGE,	//load value at operand ($0000 -> $00FF)
	ZERO_PAGE_X,	//load value at (operand+X)&0xFF ($0000 -> $00FF)
	ZERO_PAGE_Y,	//load value at (operand+Y)&0xFF ($0000 -> $00FF) !! only for [LDX, STX]
	RELATIVE,	//for branch instruction, -128 -> +127 to add to PC if true (!! since always +2 during instruction -> -126 -> +129)
	ABSOLUTE,	//16 bit address (jumps and stuff)
	ABSOLUTE_X,	//16 bit address + value in X
	ABSOLUTE_Y,	//16 bit address + value in Y
	INDIRECT,	//16 bit pointer to a 16 bit address !!only JMP
	INDIRECT_X, //aka Indexed Indirect or (ZEROPAGE, X) : (operand + X)&0xFF points to target address (lives in ZERO_PAGE)
	INDIRECT_Y,	//aka Indirect Indexed or (ZEROPAGE), Y : operand points to 16bit address, to which Y is added -> addr = (*(op) + Y)
	INVALID_ADDRESSING_MODE
	// IMMEDIATE,
	// ZERO_PAGE,
	// ZERO_PAGE_X,
	// ZERO_PAGE_Y,
	// ABSOLUTE,
	// ABSOLUTE_X,
	// ABSOLUTE_Y,
	// INDIRECT,
	// INDIRECT_X,
	// INDIRECT_Y,
	// ACCUMULATOR,
	// RELATIVE

} addressing_mode_t;

typedef struct {

	instruction_t instruction;
	addressing_mode_t addr_mode;

} opcode_meta_t;

typedef struct {

	instruction_t instruction;
	uint8_t operandA;
	uint8_t operandB;
	
} operation_t;


#endif
