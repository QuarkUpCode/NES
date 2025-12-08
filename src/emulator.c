#include <stdlib.h>

#include "emulator.h"

#include "names.h"


uint8_t fetch(NES_state* nes, uint16_t address){

	const int RAM_SIZE = 0x800;

	if(address < 0x2000){
		return nes->cpu_mm.ram[address%RAM_SIZE];
	}
	// if(address < 0x4000){
	// 	if((0x2000 | (address % 8)) == PPUSTATUS){
	// 		//clear PPu internal W register
	// 	}
	// }
	return 0x00;
}

uint8_t writemm(NES_state* nes, uint16_t address, uint8_t value){

	const int RAM_SIZE = 0x800;
	if(address < 0x2000){
		nes->cpu_mm.ram[address%RAM_SIZE] = value;
	}

	return 0;
}




NES_state new_nes(){
	
	NES_state nes = {0};
	
	const int RAM_SIZE = 0x800;
	nes.cpu_mm.ram = malloc(sizeof(uint8_t) * RAM_SIZE);
	// nes.

	return nes;
}



void load_cardridge(NES_state* nes){

}



void spush(NES_state* nes, uint8_t value){
	writemm(nes, 0x0100 + nes->cpu.S, value);
	nes->cpu.S--;
}
uint8_t spull(NES_state* nes){
	nes->cpu.S++;
	return fetch(nes, 0x0100 + nes->cpu.S);
}

uint8_t flag_get_carry(NES_state* nes){
	return (nes->cpu.flags&FLAG_C) != 0;
}
void flag_set_carry(NES_state* nes, uint8_t value){
	nes->cpu.flags &= ~FLAG_C;
	nes->cpu.flags |= FLAG_C * (value!=0);
}

uint8_t flag_get_zero(NES_state* nes){
	return (nes->cpu.flags&FLAG_Z) != 0;
}
void flag_set_zero(NES_state* nes, uint8_t value){
	nes->cpu.flags &= ~FLAG_Z;
	nes->cpu.flags |= FLAG_Z * (value!=0);
}

uint8_t flag_get_interrupt_disable(NES_state* nes){
	return (nes->cpu.flags&FLAG_I) != 0;
}
void flag_set_interrupt_disable(NES_state* nes, uint8_t value){
	nes->cpu.flags &= ~FLAG_I;
	nes->cpu.flags |= FLAG_I * (value!=0);
}

uint8_t flag_get_decimal(NES_state* nes){
	return (nes->cpu.flags&FLAG_D) != 0;
}
void flag_set_decimal(NES_state* nes, uint8_t value){
	nes->cpu.flags &= ~FLAG_D;
	nes->cpu.flags |= FLAG_D * (value!=0);
}

uint8_t flag_get_break(NES_state* nes){
	return (nes->cpu.flags&FLAG_B) != 0;
}
void flag_set_break(NES_state* nes, uint8_t value){
	nes->cpu.flags &= ~FLAG_B;
	nes->cpu.flags |= FLAG_B * (value!=0);
}

uint8_t flag_get_overflow(NES_state* nes){
	return (nes->cpu.flags&FLAG_V) != 0;
}
void flag_set_overflow(NES_state* nes, uint8_t value){
	nes->cpu.flags &= ~FLAG_V;
	nes->cpu.flags |= FLAG_V * (value!=0);
}

uint8_t flag_get_negative(NES_state* nes){
	return (nes->cpu.flags&FLAG_N) != 0;
}
void flag_set_negative(NES_state* nes, uint8_t value){
	nes->cpu.flags &= ~FLAG_N;
	nes->cpu.flags |= FLAG_N * (value!=0);
}

void flag_set_NVxxDIZC(NES_state* nes, uint8_t NVxxDIZC, uint8_t delay_I){
	nes->cpu.flags = NVxxDIZC&0b11001011;
	if(delay_I){
		nes->delay_I = 1;
		nes->delay_I_value = (NVxxDIZC&FLAG_I) != 0;
	}
	else{
		nes->cpu.flags |= NVxxDIZC&FLAG_I;
	}
	//i think B should always be set at 0 but not sure
}


uint8_t flag_get_NV00DIZC(NES_state* nes){
	return nes->cpu.flags&0b11001111;
}
