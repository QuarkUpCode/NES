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
