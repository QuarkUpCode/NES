#include "emulator.h"
#include "instructions.h"
#include <stdint.h>

#include "lib/quarklib/qprint/qprint.h"

//this might be totally wrong, came from an old project of mine
const opcode_meta_t OPCODE_LOOKUP[256] = {{BRK, IMPLIED, 1},{ORA, INDIRECT_X, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{ORA, ZERO_PAGE, 2},{ASL, ZERO_PAGE, 2},{ZZZ, -1, 1},{PHP, INVALID_ADDRESSING_MODE, 1},{ORA, IMMEDIATE, 2},{ASL, ACCUMULATOR, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{ORA, ABSOLUTE, 3},{ASL, ABSOLUTE, 3},{ZZZ, -1, 1},{BPL, ABSOLUTE, 2},{ORA, INDIRECT_Y, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{ORA, ZERO_PAGE_X, 2},{ASL, ZERO_PAGE_X, 2},{ZZZ, -1, 1},{CLC, INVALID_ADDRESSING_MODE, 1},{ORA, ABSOLUTE_Y, 3},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{ORA, ABSOLUTE_X, 3},{ASL, ABSOLUTE_X, 3},{ZZZ, -1, 1},{JSR, ABSOLUTE, 3},{AND, INDIRECT_X, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{BIT, ZERO_PAGE, 2},{AND, ZERO_PAGE, 2},{ROL, ZERO_PAGE, 2},{ZZZ, -1, 1},{PLP, INVALID_ADDRESSING_MODE, 1},{AND, IMMEDIATE, 2},{ROL, ACCUMULATOR, 1},{ZZZ, -1, 1},{BIT, ABSOLUTE, 3},{AND, ABSOLUTE, 3},{ROL, ABSOLUTE, 3},{ZZZ, -1, 1},{BMI, ABSOLUTE, 2},{AND, INDIRECT_Y, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{AND, ZERO_PAGE_X, 2},{ROL, ZERO_PAGE_X, 2},{ZZZ, -1, 1},{SEC, INVALID_ADDRESSING_MODE, 1},{AND, ABSOLUTE_Y, 3},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{AND, ABSOLUTE_X, 3},{ROL, ABSOLUTE_X, 3},{ZZZ, -1, 1},{RTI, IMPLIED, 1},{EOR, INDIRECT_X, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{EOR, ZERO_PAGE, 2},{LSR, ZERO_PAGE, 2},{ZZZ, -1, 1},{PHA, INVALID_ADDRESSING_MODE, 1},{EOR, IMMEDIATE, 2},{LSR, ACCUMULATOR, 1},{ZZZ, -1, 1},{JMP, ABSOLUTE, 3},{EOR, ABSOLUTE, 3},{LSR, ABSOLUTE, 3},{ZZZ, -1, 1},{BVC, ABSOLUTE, 2},{EOR, INDIRECT_Y, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{EOR, ZERO_PAGE_X, 2},{LSR, ZERO_PAGE_X, 2},{ZZZ, -1, 1},{CLI, INVALID_ADDRESSING_MODE, 1},{EOR, ABSOLUTE_Y, 3},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{EOR, ABSOLUTE_X, 3},{LSR, ABSOLUTE_X, 3},{ZZZ, -1, 1},{RTS, IMPLIED, 1},{ADC, INDIRECT_X, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{ADC, ZERO_PAGE, 2},{ROR, ZERO_PAGE, 2},{ZZZ, -1, 1},{PLA, INVALID_ADDRESSING_MODE, 1},{ADC, IMMEDIATE, 2},{ROR, ACCUMULATOR, 1},{ZZZ, -1, 1},{JMP, INDIRECT, 3},{ADC, ABSOLUTE, 3},{ROR, ABSOLUTE, 3},{ZZZ, -1, 1},{BVS, ABSOLUTE, 2},{ADC, INDIRECT_Y, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{ADC, ZERO_PAGE_X, 2},{ROR, ZERO_PAGE_X, 2},{ZZZ, -1, 1},{SEI, INVALID_ADDRESSING_MODE, 1},{ADC, ABSOLUTE_Y, 3},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{ADC, ABSOLUTE_X, 3},{ROR, ABSOLUTE_X, 3},{ZZZ, -1, 1},{ZZZ, -1, 1},{STA, INDIRECT_X, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{STY, ZERO_PAGE, 2},{STA, ZERO_PAGE, 2},{STX, ZERO_PAGE, 2},{ZZZ, -1, 1},{DEY, INVALID_ADDRESSING_MODE, 1},{ZZZ, -1, 1},{TXA, INVALID_ADDRESSING_MODE, 1},{ZZZ, -1, 1},{STY, ABSOLUTE, 3},{STA, ABSOLUTE, 3},{STX, ABSOLUTE, 3},{ZZZ, -1, 1},{BCC, ABSOLUTE, 2},{STA, INDIRECT_Y, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{STY, ZERO_PAGE_X, 2},{STA, ZERO_PAGE_X, 2},{STX, ZERO_PAGE_Y, 2},{ZZZ, -1, 1},{TYA, INVALID_ADDRESSING_MODE, 1},{STA, ABSOLUTE_Y, 3},{TXS, INVALID_ADDRESSING_MODE, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{STA, ABSOLUTE_X, 3},{ZZZ, -1, 1},{ZZZ, -1, 1},{LDY, IMMEDIATE, 2},{LDA, INDIRECT_X, 2},{LDX, IMMEDIATE, 2},{ZZZ, -1, 1},{LDY, ZERO_PAGE, 2},{LDA, ZERO_PAGE, 2},{LDX, ZERO_PAGE, 2},{ZZZ, -1, 1},{TAY, INVALID_ADDRESSING_MODE, 1},{LDA, IMMEDIATE, 2},{TAX, INVALID_ADDRESSING_MODE, 1},{ZZZ, -1, 1},{LDY, ABSOLUTE, 3},{LDA, ABSOLUTE, 3},{LDX, ABSOLUTE, 3},{ZZZ, -1, 1},{BCS, ABSOLUTE, 2},{LDA, INDIRECT_Y, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{LDY, ZERO_PAGE_X, 2},{LDA, ZERO_PAGE_X, 2},{LDX, ZERO_PAGE_Y, 2},{ZZZ, -1, 1},{CLV, INVALID_ADDRESSING_MODE, 1},{LDA, ABSOLUTE_Y, 3},{TSX, INVALID_ADDRESSING_MODE, 1},{ZZZ, -1, 1},{LDY, ABSOLUTE_X, 3},{LDA, ABSOLUTE_X, 3},{LDX, ABSOLUTE_Y, 3},{ZZZ, -1, 1},{CPY, IMMEDIATE, 2},{CMP, INDIRECT_X, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{CPY, ZERO_PAGE, 2},{CMP, ZERO_PAGE, 2},{DEC, ZERO_PAGE, 2},{ZZZ, -1, 1},{INY, INVALID_ADDRESSING_MODE, 1},{CMP, IMMEDIATE, 2},{DEX, INVALID_ADDRESSING_MODE, 1},{ZZZ, -1, 1},{CPY, ABSOLUTE, 3},{CMP, ABSOLUTE, 3},{DEC, ABSOLUTE, 3},{ZZZ, -1, 1},{BNE, ABSOLUTE, 2},{CMP, INDIRECT_Y, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{CMP, ZERO_PAGE_X, 2},{DEC, ZERO_PAGE_X, 2},{ZZZ, -1, 1},{CLD, INVALID_ADDRESSING_MODE, 1},{CMP, ABSOLUTE_Y, 3},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{CMP, ABSOLUTE_X, 3},{DEC, ABSOLUTE_X, 3},{ZZZ, -1, 1},{CPX, IMMEDIATE, 2},{SBC, INDIRECT_X, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{CPX, ZERO_PAGE, 2},{SBC, ZERO_PAGE, 2},{INC, ZERO_PAGE, 2},{ZZZ, -1, 1},{INX, INVALID_ADDRESSING_MODE, 1},{SBC, IMMEDIATE, 2},{NOP, IMPLIED, 1},{ZZZ, -1, 1},{CPX, ABSOLUTE, 3},{SBC, ABSOLUTE, 3},{INC, ABSOLUTE, 3},{ZZZ, -1, 1},{BEQ, ABSOLUTE, 2},{SBC, INDIRECT_Y, 2},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{SBC, ZERO_PAGE_X, 2},{INC, ZERO_PAGE_X, 2},{ZZZ, -1, 1},{SED, INVALID_ADDRESSING_MODE, 1},{SBC, ABSOLUTE_Y, 3},{ZZZ, -1, 1},{ZZZ, -1, 1},{ZZZ, -1, 1},{SBC, ABSOLUTE_X, 3},{INC, ABSOLUTE_X, 3},{ZZZ, -1, 1}};



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

opcode_meta_t __unused_parse_opcode(uint8_t opcode){
	
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






















opcode_meta_t parse_opcode(uint8_t opcode){
	return OPCODE_LOOKUP[opcode];
}

//probably one of the worst
void execute_instruction(NES_state* nes, operation_t op){
	
	uint16_t tmp;

	switch(op.instruction){
		/* Access */
		case LDA:
			nes->cpu.A = op.value;
			flag_set_zero(nes, op.value == 0);
			flag_set_negative(nes, (op.value&0b10000000) != 0);
			break;
		case STA:
			writemm(nes, op.address, nes->cpu.A);
			break;
		case LDX:
			nes->cpu.X = op.value;
			flag_set_zero(nes, op.value == 0);
			flag_set_negative(nes, (op.value&0b10000000) != 0);
			break;
		case STX:
			writemm(nes, op.address, nes->cpu.X);
			break;
		case LDY:
			nes->cpu.Y = op.value;
			flag_set_zero(nes, op.value == 0);
			flag_set_negative(nes, (op.value&0b10000000) != 0);
			break;
		case STY:
			writemm(nes, op.address, nes->cpu.Y);
			break;

		/* Transfer */
		case TAX:
			nes->cpu.X = nes->cpu.A;
			flag_set_zero(nes, nes->cpu.A == 0);
			flag_set_negative(nes, (nes->cpu.A&0b10000000) != 0);
			break;
		case TXA:
			nes->cpu.A = nes->cpu.X;
			flag_set_zero(nes, nes->cpu.X == 0);
			flag_set_negative(nes, (nes->cpu.X&0b10000000) != 0);
			break;
		case TAY:
			nes->cpu.Y = nes->cpu.A;
			flag_set_zero(nes, nes->cpu.A == 0);
			flag_set_negative(nes, (nes->cpu.A&0b10000000) != 0);
			break;
		case TYA:
			nes->cpu.A = nes->cpu.Y;
			flag_set_zero(nes, nes->cpu.Y == 0);
			flag_set_negative(nes, (nes->cpu.Y&0b10000000) != 0);
			break;

		/* Arithmetic */
		case ADC:
			tmp = nes->cpu.A + op.value + flag_get_carry(nes);
			flag_set_carry(nes, tmp > 0xFF);
			flag_set_zero(nes, nes->cpu.A == 0);
			flag_set_overflow(nes, (tmp ^ nes->cpu.A) & (tmp ^ op.value) & 0x80);
			flag_set_negative(nes, (nes->cpu.A&0b10000000) != 0);
			// if(tmp > 0xFF) flag_set_carry(nes, 1);
			nes->cpu.A = tmp&0xFF;
			break;
		//could invert op.value then jump to ADC
		case SBC:
			tmp = nes->cpu.A + (~op.value) + flag_get_carry(nes);
			flag_set_carry(nes, tmp > 0xFF);
			flag_set_zero(nes, nes->cpu.A == 0);
			flag_set_overflow(nes, (tmp ^ nes->cpu.A) & (tmp ^ (~op.value)) & 0x80);
			flag_set_negative(nes, (nes->cpu.A&0b10000000) != 0);
			nes->cpu.A = tmp&0xFF;
			break;
		case INC:
			tmp = (op.value + 1)&0xFF;
			flag_set_zero(nes, tmp == 0x00);
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			writemm(nes, op.address, tmp);
			break;
		case DEC:
			tmp = (op.value - 1)&0xFF;
			flag_set_zero(nes, tmp == 0x00);
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			writemm(nes, op.address, tmp);
			break;
		case INX:
			tmp = (nes->cpu.X + 1)&0xFF;
			flag_set_zero(nes, tmp == 0x00);
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			nes->cpu.X = tmp;
			break;
		case DEX:
			tmp = (nes->cpu.X - 1)&0xFF;
			flag_set_zero(nes, tmp == 0x00);
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			nes->cpu.X = tmp;
			break;
		case INY:
			tmp = (nes->cpu.Y + 1)&0xFF;
			flag_set_zero(nes, tmp == 0x00);
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			nes->cpu.Y = tmp;
			break;
		case DEY:
			tmp = (nes->cpu.Y - 1)&0xFF;
			flag_set_zero(nes, tmp == 0x00);
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			nes->cpu.Y = tmp;
			break;
	
		/* Shift */
		case ASL:
			tmp = op.value << 1;
			flag_set_carry(nes, (op.value&0b10000000) != 0);
			flag_set_zero(nes, tmp == 0);
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			if(op.fucking_accumulator) nes->cpu.A = tmp;
			else writemm(nes, op.address, tmp);
			break;
		case LSR:
			tmp = op.value >> 1;
			flag_set_carry(nes, op.value&0x01);
			flag_set_zero(nes, tmp == 0);
			flag_set_negative(nes, 0);
			if(op.fucking_accumulator) nes->cpu.A = tmp;
			else writemm(nes, op.address, tmp);
			break;
		case ROL:
			//not sure about carry order
			tmp = op.value << 1;
			tmp |= flag_get_carry(nes);
			flag_set_carry(nes, (op.value&0b10000000) != 0);
			if(op.fucking_accumulator) nes->cpu.A = tmp;
			else{
				//dummy write bc read-modify-write writes og value first then write new value i think ??
				writemm(nes, op.address, op.value);
				writemm(nes, op.address, tmp);
			}
			break;
		case ROR:
			//not sure about carry order
			tmp = op.value >> 1;
			tmp |= flag_get_carry(nes)<<7;
			flag_set_carry(nes, op.value&0x01);
			if(op.fucking_accumulator) nes->cpu.A = tmp;
			else{
				//dummy write bc read-modify-write writes og value first then write new value i think ??
				writemm(nes, op.address, op.value);
				writemm(nes, op.address, tmp);
			}
			break;

		/* Bitwise */
		case AND:
			tmp = nes->cpu.A & op.value;	//convoluted but i dont like setting flags by calling the register directly
			flag_set_zero(nes, tmp == 0);
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			nes->cpu.A = tmp;
			break;
		case ORA:
			tmp = nes->cpu.A | op.value;	//convoluted but i dont like setting flags by calling the register directly
			flag_set_zero(nes, tmp == 0);
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			nes->cpu.A = tmp;
			break;
		case EOR:
			tmp = nes->cpu.A ^ op.value;	//convoluted but i dont like setting flags by calling the register directly
			flag_set_zero(nes, tmp == 0);
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			nes->cpu.A = tmp;
			break;
		case BIT:
			//only sets flags
			tmp = nes->cpu.A & op.value;
			flag_set_zero(nes, tmp == 0);
			flag_set_overflow(nes, (op.value&0b01000000) != 0);
			flag_set_negative(nes, (op.value&0b10000000) != 0);
			break;

		/* Compare */
		case CMP:	//should use tmp instead of comparing using nes->cpu.R
			tmp = nes->cpu.A - op.value;
			flag_set_carry(nes, nes->cpu.A>=op.value);
			flag_set_zero(nes, nes->cpu.A == op.value);	//could do !tmp;
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			break;
		case CPX:	//if i do the bit parsing instead of lookup i can just call the same as CMP but with x instead of a
			tmp = nes->cpu.X - op.value;
			flag_set_carry(nes, nes->cpu.X>=op.value);
			flag_set_zero(nes, nes->cpu.X == op.value);	//could do !tmp;
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			break;
		case CPY:
			tmp = nes->cpu.Y - op.value;
			flag_set_carry(nes, nes->cpu.Y>=op.value);
			flag_set_zero(nes, nes->cpu.Y == op.value);	//could do !tmp;
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			break;

		/* Branch */
		case BCC:	//im pretty sure i dont need to add +2 because i already call nes->cpu.PC++ twice before reaching here
			if(!flag_get_carry(nes)) nes->cpu.PC += (int8_t)op.value;	//could also check bit 7 and do two's copmlement 8bit to 16bit by hand but eh compiler's probably smarter than me
			break;
		case BCS:
			if(flag_get_carry(nes)) nes->cpu.PC += (int8_t)op.value;	//could also check bit 7 and do two's copmlement 8bit to 16bit by hand but eh compiler's probably smarter than me
			break;
		case BEQ:
			if(flag_get_zero(nes)) nes->cpu.PC += (int8_t)op.value;	//could also check bit 7 and do two's copmlement 8bit to 16bit by hand but eh compiler's probably smarter than me
			break;
		case BNE:
			if(!flag_get_zero(nes)) nes->cpu.PC += (int8_t)op.value;	//could also check bit 7 and do two's copmlement 8bit to 16bit by hand but eh compiler's probably smarter than me
			break;
		case BPL:
			if(!flag_get_negative(nes)) nes->cpu.PC += (int8_t)op.value;	//could also check bit 7 and do two's copmlement 8bit to 16bit by hand but eh compiler's probably smarter than me
			break;
		case BMI:
			if(flag_get_negative(nes)) nes->cpu.PC += (int8_t)op.value;	//could also check bit 7 and do two's copmlement 8bit to 16bit by hand but eh compiler's probably smarter than me
			break;
		case BVC:
			if(!flag_get_overflow(nes)) nes->cpu.PC += (int8_t)op.value;	//could also check bit 7 and do two's copmlement 8bit to 16bit by hand but eh compiler's probably smarter than me
			break;
		case BVS:
			if(flag_get_overflow(nes)) nes->cpu.PC += (int8_t)op.value;	//could also check bit 7 and do two's copmlement 8bit to 16bit by hand but eh compiler's probably smarter than me
			break;

		/* Jump */
		case JMP:
			nes->cpu.PC = op.address;
			break;
		case JSR:
			spush(nes, (nes->cpu.PC + 2) >> 8);
			spush(nes, (nes->cpu.PC + 2) & 0xFF);
			nes->cpu.PC = op.address;
			break;
		case RTS:
			nes->cpu.PC = spull(nes);
			nes->cpu.PC |= spull(nes) << 8;
			break;
		case BRK:

			/*
				For future me: dont forget : wiki says
				"Unfortunately, a 6502 bug allows the BRK IRQ to be overridden by an NMI occurring at the same time. In this case, only the NMI handler is called; the IRQ handler is skipped. However, the break flag is still set in the flags byte pushed to the stack, so the NMI handler can detect that this occurred (albeit slowly) by checking this flag."
			*/

			spush(nes, (nes->cpu.PC + 2) >> 8);	//im writing it down like this rn cause im basically just copying the wiki page for the instructions, its probably the same as juste doing 0100+s = x, s--, and i do this explicitely in stack instructions so i should go with one or the other but not both cos it'd be ugly af
			spush(nes, (nes->cpu.PC + 2) & 0xFF);
			flag_set_break(nes, 1);	//prolly redundant since i do flags|0b00110000
			// spush(nes, nes->cpu.flags);
			spush(nes, flag_get_NV00DIZC(nes)|0b00110000);//same here, prolly same as the PHP instruction so yeah
			flag_set_interrupt_disable(nes, 1);
			nes->cpu.PC = 0xFFFE;
			break;
		case RTI:
			flag_set_NVxxDIZC(nes, spull(nes), 0);
			nes->cpu.PC = spull(nes);
			nes->cpu.PC |= spull(nes) << 8;
			break;

		/* Stack */
		case PHA:
			writemm(nes, 0x0100 + nes->cpu.S, nes->cpu.A);
			nes->cpu.S--;
			break;
		case PLA:
			nes->cpu.S++;
			tmp = fetch(nes, 0x0100 + nes->cpu.S);
			flag_set_zero(nes, tmp == 0);
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			nes->cpu.A = tmp;
			break;
		case PHP:	//eww php
			flag_set_break(nes, 1);	//prolly redundant since i do flags|0b00110000
			writemm(nes, 0x0100 + nes->cpu.S, flag_get_NV00DIZC(nes)|0b00110000);
			nes->cpu.S--;
			break;
		case PLP:
			nes->cpu.S++;
			flag_set_NVxxDIZC(nes, fetch(nes, 0x0100 + nes->cpu.S), 1);
			break;
		case TXS:
			nes->cpu.S = nes->cpu.X;
			break;
		case TSX:
			tmp = nes->cpu.S;
			flag_set_zero(nes, tmp == 0);
			flag_set_negative(nes, (tmp&0b10000000) != 0);
			nes->cpu.X = tmp;
			break;			

		/* Flags */
		case CLC:
			flag_set_carry(nes, 0);
			break;
		case SEC:
			flag_set_carry(nes, 0);
			break;
		case CLI:
			nes->delay_I_value = 0;
			nes->delay_I = 1;
			break;
		case SEI:
			nes->delay_I_value = 1;
			nes->delay_I = 1;
			break;
		case CLD:
			flag_set_decimal(nes, 0);
			break;
		case SED:
			flag_set_decimal(nes, 1);
			break;
		case CLV:
			flag_set_overflow(nes, 0);
			break;

		/* Other */
		case NOP:
			// :p
			break;

		default:
			break;
	}
	return;
invalid_am:
	qerror("Invalid Addressing mode\n");
	return;
}



////TODO : instead of parsing opcodes with they mnemotecnic structure, just go with my first intuition of just having a big ass 256 bytes lookup table
//update might go back to bit parsing

void step(NES_state* nes){

	// uint8_t opcode = fetch(nes, nes->cpu.PC);nes->cpu.PC++;
	if(nes->delay_I&0x01) nes->delay_I = 0x80;
	uint8_t opcode = fetch(nes, nes->cpu.PC++);
	opcode_meta_t meta = parse_opcode(opcode);
	
	operation_t operation = {0, 0, 0, 0};

	uint8_t operand;
	uint16_t target;

	switch(meta.addr_mode){

		case ACCUMULATOR:
			operation.value = nes->cpu.A;
			// operation.ptr = &(nes->cpu.A);
			operation.fucking_accumulator = 1;
			break;
		case IMMEDIATE:
			operation.value = fetch(nes, nes->cpu.PC++);
			break;
		case ZERO_PAGE:
			operand = fetch(nes, nes->cpu.PC++);
			operation.value = fetch(nes, operand);
			operation.address = 0x0000 | operand;
			break;
		case ZERO_PAGE_X:
			operand = fetch(nes, nes->cpu.PC++);
			target = 0x0000 | ((operand + nes->cpu.X)&0xFF);
			operation.value = fetch(nes, target);
			operation.address = target;
			break;
		case ZERO_PAGE_Y:
			operand = fetch(nes, nes->cpu.PC++);
			target = 0x0000 | ((operand + nes->cpu.Y)&0xFF);
			operation.value = fetch(nes, target);
			operation.address = target;
			break;
		case RELATIVE:
			operand = fetch(nes, nes->cpu.PC++);
			operation.value = operand;
			break;
		case ABSOLUTE:
			operand = fetch(nes, nes->cpu.PC++);
			operation.address = operand;
			operand = fetch(nes, nes->cpu.PC++);
			operation.address |= operand << 8;
			break;
		case ABSOLUTE_X:
			operand = fetch(nes, nes->cpu.PC++);
			operation.address = operand;
			operand = fetch(nes, nes->cpu.PC++);
			operation.address |= operand << 8;
			operation.address += nes->cpu.X;
			break;
		case ABSOLUTE_Y:
			operand = fetch(nes, nes->cpu.PC++);
			operation.address = operand;
			operand = fetch(nes, nes->cpu.PC++);
			operation.address |= operand << 8;
			operation.address += nes->cpu.Y;
			break;
		case INDIRECT:
			operand = fetch(nes, nes->cpu.PC++);
			target = operand;
			operand = fetch(nes, nes->cpu.PC++);
			target |= operand << 8;
			// target = fetch(nes, target);
			operation.address = fetch(nes, target);
			// operation.value = target&0xFF;
			break;
		//PROBABLY WRONG
		case INDIRECT_X:
			operand = fetch(nes, nes->cpu.PC++);
			target = 0x0000 | ((operand + nes->cpu.X)&0xFF);
			target = fetch(nes, target) | (fetch(nes, target+1));
			// operation.value = fetch(nes, target);
			//maybe i should %0xFF when i target+1, not sure, should read about it
			operation.value = fetch(nes, target);
			operation.address = target;
			break;
		//PROBABLY WRONG
		case INDIRECT_Y:
			operand = fetch(nes, nes->cpu.PC++);
			target = fetch(nes, 0x0000 | operand) | (fetch(nes, 0x0000 | ((operand+1)&0xFF)));
			target += nes->cpu.Y;
			operation.value = fetch(nes, target);
			operation.address = target;
			break;
		case INVALID_ADDRESSING_MODE:
			qerror("INVALID ADDRESSING MODE\n");
			break;
		default:
			qerror("HOW EVEN THE FUCK DO YOU MANAGE TO REACH THIS\n");
			break;
	}

	// uint8_t opcode = fetch(memory, CPU->PC); CPU->PC++;
	// opcode_meta_t meta = parse_opcode(opcode);
	execute_instruction(nes, operation);
	if(nes->delay_I&0x80){
		flag_set_interrupt_disable(nes, nes->delay_I_value);
		nes->delay_I &= ~0x80;
	}

}









