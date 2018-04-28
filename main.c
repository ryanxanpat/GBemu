#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "gbTypes.h"


bool ParseRomHeader(uint8_t* memory);
void DumpInstructions(uint8_t *memory, uint16_t pc, uint16_t numToDump);
void Boot(char* rom);
void DumpMemory(uint8_t* memory, uint16_t start, uint16_t size);


int main(int argc, char **argv)
{
	if (argc == 1)
	{
		printf("Supply path to rom\n");
		return 0;
	}
	else if (argc > 2)
	{
		printf("Too many args\n");
		return 0;
	}

	Boot(argv[1]);

	return 0;
}

bool Init()
{
	uint8_t *memory = (uint8_t *) malloc(MEMORY_SIZE * sizeof(uint8_t));
	if (memory == NULL)
	{
		printf("Failed to allocate space for Memory Map\n");
		return false;
	}

	memset(memory, 0, MEMORY_SIZE);

	FILE* rom = fopen("roms/boot.gb", "rb");
	if (rom == NULL)
	{
		printf("Could not open bootstrap ROM\n");
		return 0;
	}
	printf("Opened bootstrap rom\n");

	if (!fgets(memory, 257, rom))
	{
		printf("Couldn't read in bootstrap ROM\n");
		return false;
	}

	fclose(rom);

	// rom = fopen(romPath, "rb");
	// if (rom == NULL)
	// {
	// 	printf("Could not open rom\n");
	// 	return 0;
	// }
	// printf("Opened rom: %s\n\n", romPath);

	// This needs to be changed later
	// fseek(rom, 0, SEEK_END);
	// uint32_t codeSize = ftell(rom);
	// fseek(rom, 0, SEEK_SET);
	// if (!fgets(memory, codeSize, rom))
	// {
	// 	printf("Couldn't read ROM into memory\n");
	// 	return false;
	// }

	DumpMemory(memory, 0, 300);
	DumpInstructions(memory, 0, 50);

	// fclose(rom);

	// ParseRomHeader(memory);

	// Need to return reference to memory somehow. Also need to free memory later
	return true;
}

void Boot(char* rom)
{
	if (!Init())
		return;
}

const char* GetLicenseName(uint16_t code)
{
	switch(code)
	{
		case 00:
			return "None";
		case 0x3130:
			return "Nintendo";
		default:
			printf("License Code = 0x%04x\n", code);
			return "!!!NOT IMPLEMENTED!!!";
	}
}

const char* GetCartridgeType(uint8_t type)
{
	switch(type)
	{
		case 0x00: return "ROM";
		case 0x01: return "MBC1";
		case 0x02: return "MBC1+RAM";
		case 0x03: return "MBC1+RAM+BATTERY";
		case 0x05: return "MBC2";
		case 0x06: return "MBC2+BATTERY";
		case 0x08: return "ROM+RAM";
		case 0x09: return "ROM+RAM+BATTERY";
		case 0x0B: return "MMM01";
		case 0x0C: return "MMM01+RAM";
		case 0x0D: return "MMM01+RAM+BATTERY";
		case 0x0F: return "MBC3+TIMER+BATTERY";
		case 0x10: return "MBC3+TIMER+RAM+BATTERY";
		case 0x11: return "MBC3";
		case 0x12: return "MBC3+RAM";
		case 0x13: return "MBC3+RAM+BATTERY";
		case 0x15: return "MBC4";
		case 0x16: return "MBC4+RAM";
		case 0x17: return "MBC4+RAM+BATTERY";
		case 0x19: return "MBC5";
		case 0x1A: return "MBC5+RAM";
		case 0x1B: return "MBC5+RAM+BATTERY";
		case 0x1C: return "MBC5+RUMBLE";
		case 0x1D: return "MBC5+RUMBLE+RAM";
		case 0x1E: return "MBC5+RUMBLE+RAM+BATTERY";
		case 0xFC: return "POCKET CAMERA";
		case 0xFD: return "Bandai TAMA5";
		case 0xFE: return "HuC3";
		case 0xFF: return "HuC1+RAM+BATTERY";
		default: return "!!!NOT IMPLEMENTED!!!";
	}
}

bool ParseRomHeader(uint8_t *memory)
{
	printf("Parsing ROM Header...\n\n");

	// Name/Title
	printf("Title: %s\n", &memory[0x134]);

	// License Code
	uint16_t *doubleByte = (uint16_t *)&memory[0x144];
	printf("License: %s\n", GetLicenseName(*doubleByte)); 

	// Super Features GameBoy Byte
	uint8_t *byte = &memory[0x146];
	if (*byte == 0x3)
		printf("This ROM uses Super GameBoy Features\n");

	byte = &memory[0x147];
	printf("Cartridge Type: %s\n", GetCartridgeType(*byte));

	byte = &memory[0x148];
	printf("ROM size: ");
	switch(*byte)
	{
		case 0x0: printf("32k\n"); break;
		case 0x1: printf("64k\n"); break;
		case 0x2: printf("128k\n"); break;
		case 0x3: printf("256k\n"); break;
		case 0x4: printf("512k\n"); break;
		case 0x5: printf("1024k\n"); break;
		case 0x6: printf("2048k\n"); break;
		case 0x7: printf("4096k\n"); break;
		default: printf("Invalid value\n"); return false;
	}

	byte = &memory[0x149];
	printf("Save RAM size: ");
	switch(*byte)
	{
		case 0x0: printf("0k\n"); break;
		case 0x1: printf("2k\n"); break;
		case 0x2: printf("8k\n"); break;
		case 0x3: printf("32k\n"); break;
		default: printf("Invalid value\n"); return false;;
	}

	byte = &memory[0x14A];
	if (*byte == 0)
		printf("Country Code: Japan\n\n");
	else
		printf("Country Code: Non-Japan\n\n");

	return true;
}

uint8_t Disassemble(uint8_t *codeBuffer, uint16_t pc)
{
	uint8_t *opCode = &codeBuffer[pc];
	uint8_t opBytes;

	printf("0x%04X    ", pc);
	switch(*opCode)
	{
		case 0x00:
			printf("NOP\n");
			return 1;
		case 0x01:
			printf("LD BC, d16\n");
			return 3;
		case 0x02:
			printf("LD BC, A\n");
			return 1;
		case 0x03:
			printf("INC BC\n");
			return 1;
		case 0x04:
			printf("INC B\n");
			return 1;
		case 0x05:
			printf("DEC B\n");
			return 1;
		case 0x06:
			printf("LD B, d8\n");
			return 2;
		case 0x07:
			printf("RLCA\n");
			return 1;
		case 0x08:
			printf("LD (a16), SP\n");
			return 3;
		case 0x09:
			printf("ADD HL, BC\n");
			return 1;
		case 0x0A:
			printf("LD A, (BC)\n");
			return 1;
		case 0x0B:
			printf("DEC BC\n");
			return 1;
		case 0x0C:
			printf("INC C\n");
			return 1;
		case 0x0D:
			printf("DEC C\n");
			return 1;
		case 0x0E:
			printf("LD C, d8\n");
			return 2;
		case 0x0F:
			printf("RRCA\n");
			return 1;
		case 0x10:
			printf("STOP 0\n");
			return 2;
		case 0x11:
			printf("LD DE, d16\n");
			return 3;
		case 0x12:
			printf("LD (DE), A\n");
			return 1;
		case 0x13:
			printf("INC DE\n");
			return 1;
		case 0x14:
			printf("INC D\n");
			return 1;
		case 0x15:
			printf("DEC D\n");
			return 1;
		case 0x16:
			printf("LD D, d8\n");
			return 2;
		case 0x17:
			printf("RLA\n");
			return 1;
		case 0x18:
			printf("JR r8\n");
			return 2;
		case 0x19:
			printf("ADD HL, DE\n");
			return 1;
		case 0x1A:
			printf("LD A, (DE)\n");
			return 1;
		case 0x1B:
			printf("DEC DE\n");
			return 1;
		case 0x1C:
			printf("INC E\n");
			return 1;
		case 0x1D:
			printf("DEC E\n");
			return 1;
		case 0x1E:
			printf("LD E, d8\n");
			return 2;
		case 0x1F:
			printf("RRA\n");
			return 1;
		case 0x20:
			printf("JR NZ, r8\n");
			return 2;
		case 0x21:
			printf("LD HL, d16\n");
			return 3;
		case 0x22:
			printf("LD LD (HL+), A\n");
			return 1;
		case 0x23:
			printf("INC HL\n");
			return 1;
		case 0x24:
			printf("INC H\n");
			return 1;
		case 0x25:
			printf("DEC H\n");
			return 1;
		case 0x26:
			printf("LD H, d8\n");
			return 2;
		case 0x27:
			printf("DAA\n");
			return 1;
		case 0x28:
			printf("JR Z, r8\n");
			return 2;
		case 0x29:
			printf("ADD HL, HL\n");
			return 1;
		case 0x2A:
			printf("LD A, (HL+)\n");
			return 1;
		case 0x2B:
			printf("DEC HL\n");
			return 1;
		case 0x2C:
			printf("INC L\n");
			return 1;
		case 0x2D:
			printf("DEC L\n");
			return 1;
		case 0x2E:
			printf("LD L, d8\n");
			return 2;
		case 0x2F:
			printf("CPL\n");
			return 1;
		case 0x30:
			printf("JR NC, r8\n");
			return 2;
		case 0x31:
			printf("LD SP, d16\n");
			return 3;
		case 0x32:
			printf("LD (HL-), A\n");
			return 1;
		case 0x33:
			printf("INC SP\n");
			return 1;
		case 0x34:
			printf("INC (HL)\n");
			return 1;
		case 0x35:
			printf("DEC (HL)\n");
			return 1;
		case 0x36:
			printf("LD (HL), d8\n");
			return 2;
		case 0x37:
			printf("SCF\n");
			return 1;
		case 0x38:
			printf("JR C, r8\n");
			return 2;
		case 0x39:
			printf("ADD HL, SP\n");
			return 1;
		case 0x3A:
			printf("LD A, (HL-)\n");
			return 1;
		case 0x3B:
			printf("DEC SP\n");
			return 1;
		case 0x3C:
			printf("INC A\n");
			return 1;
		case 0x3D:
			printf("DEC A\n");
			return 1;
		case 0x3E:
			printf("LD A, d8\n");
			return 2;
		case 0x3F:
			printf("CCF\n");
			return 1;
		case 0x40:
			printf("LD B, B\n");
			return 1;
		case 0x41:
			printf("LD B, C\n");
			return 1;
		case 0x42:
			printf("LD B, D\n");
			return 1;
		case 0x43:
			printf("LD B, E\n");
			return 1;
		case 0x44:
			printf("LD B, H\n");
			return 1;
		case 0x45:
			printf("LD B, L\n");
			return 1;
		case 0x46:
			printf("LD B, (HL)\n");
			return 1;
		case 0x47:
			printf("LD B, A\n");
			return 1;
		case 0x48:
			printf("LD C, B\n");
			return 1;
		case 0x49:
			printf("LD C, C\n");
			return 1;
		case 0x4A:
			printf("LD C, D\n");
			return 1;
		case 0x4B:
			printf("LD C, E\n");
			return 1;
		case 0x4C:
			printf("LD C, H\n");
			return 1;
		case 0x4D:
			printf("LD C, L\n");
			return 1;
		case 0x4E:
			printf("LD C, (HL)\n");
			return 1;
		case 0x4F:
			printf("LD C, A\n");
			return 1;
		case 0x50:
			printf("LD D, B\n");
			return 1;
		case 0x51:
			printf("LD D, C\n");
			return 1;
		case 0x52:
			printf("LD D, D\n");
			return 1;
		case 0x53:
			printf("LD D, E\n");
			return 1;
		case 0x54:
			printf("LD D, H\n");
			return 1;
		case 0x55:
			printf("LD D, L\n");
			return 1;
		case 0x56:
			printf("LD D, (HL)\n");
			return 1;
		case 0x57:
			printf("LD D, A\n");
			return 1;
		case 0x58:
			printf("LD E, B\n");
			return 1;
		case 0x59:
			printf("LD E, C\n");
			return 1;
		case 0x5A:
			printf("LD E, D\n");
			return 1;
		case 0x5B:
			printf("LD E, E\n");
			return 1;
		case 0x5C:
			printf("LD E, H\n");
			return 1;
		case 0x5D:
			printf("LD E, L\n");
			return 1;
		case 0x5E:
			printf("LD E, (HL)\n");
			return 1;
		case 0x5F:
			printf("LD E, A\n");
			return 1;
		case 0x60:
			printf("LD H, B\n");
			return 1;
		case 0x61:
			printf("LD H, C\n");
			return 1;
		case 0x62:
			printf("LD H, D\n");
			return 1;
		case 0x63:
			printf("LD H, E\n");
			return 1;
		case 0x64:
			printf("LD H, H\n");
			return 1;
		case 0x65:
			printf("LD H, L\n");
			return 1;
		case 0x66:
			printf("LD H, (HL)\n");
			return 1;
		case 0x67:
			printf("LD H, A\n");
			return 1;
		case 0x68:
			printf("LD L, B\n");
			return 1;
		case 0x69:
			printf("LD L, C\n");
			return 1;
		case 0x6A:
			printf("LD L, D\n");
			return 1;
		case 0x6B:
			printf("LD L, E\n");
			return 1;
		case 0x6C:
			printf("LD L, H\n");
			return 1;
		case 0x6D:
			printf("LD L, L\n");
			return 1;
		case 0x6E:
			printf("LD L, (HL)\n");
			return 1;
		case 0x6F:
			printf("LD L, A\n");
			return 1;
		case 0x70:
			printf("LD (HL), B\n");
			return 1;
		case 0x71:
			printf("LD (HL), C\n");
			return 1;
		case 0x72:
			printf("LD (HL), D\n");
			return 1;
		case 0x73:
			printf("LD (HL), E\n");
			return 1;
		case 0x74:
			printf("LD (HL), H\n");
			return 1;
		case 0x75:
			printf("LD (HL), L\n");
			return 1;
		case 0x76:
			printf("HALT\n");
			return 1;
		case 0x77:
			printf("LD (HL), A\n");
			return 1;
		case 0x78:
			printf("LD A, B\n");
			return 1;
		case 0x79:
			printf("LD A, C\n");
			return 1;
		case 0x7A:
			printf("LD A, D\n");
			return 1;
		case 0x7B:
			printf("LD A, E\n");
			return 1;
		case 0x7C:
			printf("LD A, H\n");
			return 1;
		case 0x7D:
			printf("LD A, L\n");
			return 1;
		case 0x7E:
			printf("LD A, (HL)\n");
			return 1;
		case 0x7F:
			printf("LD A, A\n");
			return 1;
		case 0x80:
			printf("ADD A, B\n");
			return 1;
		case 0x81:
			printf("ADD A, C\n");
			return 1;
		case 0x82:
			printf("ADD A, D\n");
			return 1;
		case 0x83:
			printf("ADD A, E\n");
			return 1;
		case 0x84:
			printf("ADD A, H\n");
			return 1;
		case 0x85:
			printf("ADD A, L\n");
			return 1;
		case 0x86:
			printf("ADD A, (HL)\n");
			return 1;
		case 0x87:
			printf("ADD A, A\n");
			return 1;
		case 0x88:
			printf("ADC A, B\n");
			return 1;
		case 0x89:
			printf("ADC A, C\n");
			return 1;
		case 0x8A:
			printf("ADC A, D\n");
			return 1;
		case 0x8B:
			printf("ADC A, E\n");
			return 1;
		case 0x8C:
			printf("ADC A, H\n");
			return 1;
		case 0x8D:
			printf("ADC A, L\n");
			return 1;
		case 0x8E:
			printf("ADC A, (HL)\n");
			return 1;
		case 0x8F:
			printf("ADC A, A\n");
			return 1;
		case 0x90:
			printf("SUB B\n");
			return 1;
		case 0x91:
			printf("SUB C\n");
			return 1;
		case 0x92:
			printf("SUB D\n");
			return 1;
		case 0x93:
			printf("SUB E\n");
			return 1;
		case 0x94:
			printf("SUB H\n");
			return 1;
		case 0x95:
			printf("SUB L\n");
			return 1;
		case 0x96:
			printf("SUB (HL)\n");
			return 1;
		case 0x97:
			printf("SUB A\n");
			return 1;
		case 0x98:
			printf("SBC A, B\n");
			return 1;
		case 0x99:
			printf("SBC A, C\n");
			return 1;
		case 0x9A:
			printf("SBC A, D\n");
			return 1;
		case 0x9B:
			printf("SBC A, E\n");
			return 1;
		case 0x9C:
			printf("SBC A, H\n");
			return 1;
		case 0x9D:
			printf("SBC A, L\n");
			return 1;
		case 0x9E:
			printf("SBC A, (HL)\n");
			return 1;
		case 0x9F:
			printf("SBC A, A\n");
			return 1;
		case 0xA0:
			printf("AND B\n");
			return 1;
		case 0xA1:
			printf("AND C\n");
			return 1;
		case 0xA2:
			printf("AND D\n");
			return 1;
		case 0xA3:
			printf("AND E\n");
			return 1;
		case 0xA4:
			printf("AND H\n");
			return 1;
		case 0xA5:
			printf("AND L\n");
			return 1;
		case 0xA6:
			printf("AND (HL)\n");
			return 1;
		case 0xA7:
			printf("AND A\n");
			return 1;
		case 0xA8:
			printf("XOR A, B\n");
			return 1;
		case 0xA9:
			printf("XOR A, C\n");
			return 1;
		case 0xAA:
			printf("XOR A, D\n");
			return 1;
		case 0xAB:
			printf("XOR A, E\n");
			return 1;
		case 0xAC:
			printf("XOR A, H\n");
			return 1;
		case 0xAD:
			printf("XOR A, L\n");
			return 1;
		case 0xAE:
			printf("XOR A, (HL)\n");
			return 1;
		case 0xAF:
			printf("XOR A, A\n");
			return 1;
		case 0xB0:
			printf("OR B\n");
			return 1;
		case 0xB1:
			printf("OR C\n");
			return 1;
		case 0xB2:
			printf("OR D\n");
			return 1;
		case 0xB3:
			printf("OR E\n");
			return 1;
		case 0xB4:
			printf("OR H\n");
			return 1;
		case 0xB5:
			printf("OR L\n");
			return 1;
		case 0xB6:
			printf("OR (HL)\n");
			return 1;
		case 0xB7:
			printf("OR A\n");
			return 1;
		case 0xB8:
			printf("CP A, B\n");
			return 1;
		case 0xB9:
			printf("CP A, C\n");
			return 1;
		case 0xBA:
			printf("CP A, D\n");
			return 1;
		case 0xBB:
			printf("CP A, E\n");
			return 1;
		case 0xBC:
			printf("CP A, H\n");
			return 1;
		case 0xBD:
			printf("CP A, L\n");
			return 1;
		case 0xBE:
			printf("CP A, (HL)\n");
			return 1;
		case 0xBF:
			printf("CP A, A\n");
			return 1;
		case 0xC0:
			printf("RET NZ\n");
			return 1;
		case 0xC1:
			printf("POP BC\n");
			return 1;
		case 0xC2:
			printf("JP NZ, a16\n");
			return 3;
		case 0xC3:
			printf("JP a16\n");
			return 3;
		case 0xC4:
			printf("CALL NZ, a16\n");
			return 3;
		case 0xC5:
			printf("PUSH BC\n");
			return 1;
		case 0xC6:
			printf("ADD A, d8\n");
			return 2;
		case 0xC7:
			printf("RST 00H\n");
			return 1;
		case 0xC8:
			printf("RET Z\n");
			return 1;
		case 0xC9:
			printf("RET\n");
			return 1;
		case 0xCA:
			printf("JP Z, a16\n");
			return 3;
		case 0xCB:
			printf("PREFIX CB\n");
			return 1;
		case 0xCC:
			printf("CALL Z, a16\n");
			return 3;
		case 0xCD:
			printf("CALL a16\n");
			return 3;
		case 0xCE:
			printf("ADC A, d8\n");
			return 2;
		case 0xCF:
			printf("RST 08H\n");
			return 1;
		case 0xD0:
			printf("RET NC\n");
			return 1;
		case 0xD1:
			printf("POP DE\n");
			return 1;
		case 0xD2:
			printf("JP NC, a16\n");
			return 3;
		case 0xD4:
			printf("CALL NC, a16\n");
			return 3;
		case 0xD5:
			printf("PUSH DE\n");
			return 1;
		case 0xD6:
			printf("SUB d8\n");
			return 2;
		case 0xD7:
			printf("RST 10H\n");
			return 1;
		case 0xD8:
			printf("RET C\n");
			return 1;
		case 0xD9:
			printf("RETI\n");
			return 1;
		case 0xDA:
			printf("JP C, a16\n");
			return 3;
		case 0xDC:
			printf("CALL C, a16\n");
			return 3;
		case 0xDE:
			printf("SBC A, d8\n");
			return 2;
		case 0xDF:
			printf("RST 18H\n");
			return 1;
		case 0xE0:
			printf("LDH (a8), A\n");
			return 2;
		case 0xE1:
			printf("POP HL\n");
			return 1;
		case 0xE2:
			printf("LD (C), A\n");
			return 2;
		case 0xE5:
			printf("PUSH HL\n");
			return 1;
		case 0xE6:
			printf("AND d8\n");
			return 2;
		case 0xE7:
			printf("RST 20H\n");
			return 1;
		case 0xE8:
			printf("ADD SP, r8\n");
			return 2;
		case 0xE9:
			printf("JP (HL)\n");
			return 1;
		case 0xEA:
			printf("LD (a16), A\n");
			return 3;
		case 0xEE:
			printf("XOR d8\n");
			return 2;
		case 0xEF:
			printf("RST 28H\n");
			return 1;
		case 0xF0:
			printf("LDH A, (a8)\n");
			return 2;
		case 0xF1:
			printf("POP AF\n");
			return 1;
		case 0xF2:
			printf("LD A, (C)\n");
			return 2;
		case 0xF3:
			printf("DI\n");
			return 1;
		case 0xF5:
			printf("PUSH AF\n");
			return 1;
		case 0xF6:
			printf("OR d8\n");
			return 2;
		case 0xF7:
			printf("RST 30H\n");
			return 1;
		case 0xF8:
			printf("LD HL, SP+r8\n");
			return 2;
		case 0xF9:
			printf("LD SP, HL\n");
			return 1;
		case 0xFA:
			printf("LD A, (a16)\n");
			return 3;
		case 0xFB:
			printf("EI\n");
			return 1;
		case 0xFE:
			printf("CP d8\n");
			return 2;
		case 0xFF:
			printf("RST 38H\n");
			return 1;
		default:   
			printf("!!!NOT IMPLEMENTED!!! OPCODE: 0x%02x\n", *opCode); 
			return 1;
	}

	return true;
}

void DumpInstructions(uint8_t *memory, uint16_t pc, uint16_t numToDump)
{
	printf("Dumping Instructions:\n\n");

	int i = 0;
	int ret;
	while (i++ < numToDump)
		pc += Disassemble(memory, pc);
}

void DumpMemory(uint8_t* memory, uint16_t start, uint16_t size)
{
	uint16_t i = start, finish = start + size, t = 0;

	printf("0x%08X   ", i);
	while (i < size)
	{
	     printf("%02X ", memory[i++]);
	     if (t++ == 15 && i != finish)
	     {
	     	printf("\n0x%08X   ", i);
	     	t = 0;
	     }
	}
	printf("\n");
}