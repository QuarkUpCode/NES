#include "emulator.h"
#include "instructions.h"
#include <stdint.h>

#include "lib/quarklib/qprint/qprint.h"

//tysm llx.com/Neil/a2/opcodes.html

// cc = 01
const uint8_t GROUP_1_OP[0b1000] = {
	ORA, AND, EOR, ADC, STA, LDA, CMP, SBC
};
const uint8_t GROUP_1_AM[0b1000] = {
	INDIRECT_X, ZERO_PAGE, IMMEDIATE, ABSOLUTE, INDIRECT_Y, ZERO_PAGE_X, ABSOLUTE_Y, ABSOLUTE_X
};

// cc = 10
const uint8_t GROUP_2_OP[0b1000] = {
	ASL, ROL, LSR, ROR, STX, LDX, DEC, INC
};
const uint8_t GROUP_2_AM[0b1000] = {
	IMMEDIATE, ZERO_PAGE, ACCUMULATOR, ABSOLUTE, INVALID_ADDRESSING_MODE, ZERO_PAGE_X, INVALID_ADDRESSING_MODE, ABSOLUTE_X
};

// cc = 00
const uint8_t GROUP_3_OP[0b1000] = {
	NOP, BIT, JMP, JMPABS, STY, LDY, CPY, CPX
};
const uint8_t GROUP_3_AM[0b1000] = {
	IMMEDIATE, ZERO_PAGE, INVALID_ADDRESSING_MODE, ABSOLUTE, INVALID_ADDRESSING_MODE, ZERO_PAGE_X, INVALID_ADDRESSING_MODE, ABSOLUTE_X
};

const uint8_t COND_BR[0b1000] = {
	BPL, BMI, BVC, BVS, BCC, BCS, BNE, BEQ
};

opcode_meta_t parse_opcode(uint8_t opcode){
	
	opcode_meta_t meta = {NOP, INVALID_ADDRESSING_MODE};

	const uint8_t A = (opcode&0b11100000)>>5;
	const uint8_t B = (opcode&0b00011100)>>2;
	const uint8_t C = (opcode&0b00000011)>>0;
	
	/*
		!!! for example, i store TXA as STX, ACCUMULATOR
	*/

	switch(C){
		case 0b01:
			meta.instruction = GROUP_1_OP[A];
			meta.addr_mode = GROUP_1_AM[B];
			break;
		case 0b10:
			meta.instruction = GROUP_2_OP[A];
			meta.addr_mode = GROUP_2_AM[B];
			break;
		case 0b00:
			//conditional branch xxy10000
			// if((opcode&0b00011111) == 0b00010000){
			if(B == 0b100){
				meta.instruction = COND_BR[A];
				meta.addr_mode = RELATIVE;
			}
			else{
				meta.instruction = GROUP_3_OP[A];
				meta.addr_mode = GROUP_3_AM[B];
			}
			break;
		case 0b11:
			break;
		default:
			qerror("Reached unreachable branch\n");
			break;
	}

	// if(C == 0b11){
	// 	qerror("C == 0b11\n");
	// 	qdebug("opcode : $%2x\n", opcode);
	// 	return meta;
	// }

	// meta.instruction = GROUPS_OP[C][A];
	// meta.addr_mode = GROUPS_AM[C][B];

	qdebug("Intruction : %x\n", meta.instruction);

	return meta;
}

//TODO : instead of parsing opcodes with they mnemotecnic structure, just go with my first intuition of just having a big ass 256 bytes lookup table

void step(uint8_t* memory, CPU_registers* CPU){

	uint8_t opcode = fetch(memory, CPU->PC); CPU->PC++;
	opcode_meta_t meta = parse_opcode(opcode);
	

}
