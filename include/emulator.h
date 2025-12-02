#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>



typedef struct {

	uint8_t A;		//Accumulator
	uint8_t X;		//GP X register
	uint8_t Y;		//GP Y register
	uint16_t PC;	//Program Counter
	uint8_t S;		//Stack Pointer
	uint8_t P;		//Status

} CPU_registers;

typedef struct {

	uint8_t PPUCTRL;
	uint8_t PPUMASK;
	uint8_t PPUSTATUS;
	uint8_t OAMADDR;
	uint8_t OAMDATA;
	uint8_t PPUSCROLL;
	uint8_t PPUADDR;
	uint8_t PPUDATA;
	uint8_t OAMDMA;

	uint8_t PPUGenLatch;	//just read the doc; what the actual fuck
	//i guess TODO : after N CPU cycles (3-30ms) decay bit in PPUGenLatch ??
} PPU_registers;



//not definitive, haven't read up on bank switching yet
typedef struct {

	uint8_t* ram;	//$0000-$07FF
					//$0800-$1FFF	mirror
					//$2000-$2007	PPU registers
	uint8_t* wram;	//$6000-$7FFF	cartridge defined
	uint8_t* rom;	//$8000-$FFFF	cartridge defined

} CPU_memorymap;

typedef struct {

	CPU_registers cpu;
	PPU_registers ppu;
	CPU_memorymap cpu_mm;
	uint8_t* cartridge;
	uint64_t cycles;

} NES_state;


//TODO : struct for memory in order to be able to handle bank swaps etc
// uint8_t fetch(uint8_t* memory, uint16_t address);
uint8_t fetch(NES_state* nes, uint16_t address);
uint8_t flag_get_carry(NES_state* nes);

#endif
