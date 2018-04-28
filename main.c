#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "gbTypes.h"
#include "cpu.h"


bool ParseRomHeader(uint8_t* memory);
void DumpInstructions(uint8_t *memory, uint16_t pc, uint16_t numToDump);
void Boot(char* rom);


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

void Boot(char* rom)
{
	CpuState *cpu = InitCpu();
	if (cpu == NULL)
		return;

	char str;
	while (true)
	{
		printf("Run instruction?\n");
		scanf("%c", &str);
		if (str == 'n' || str == 'N')
			break;

		Emulate(cpu);
	}
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

	// Super GameBoy Features Byte
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

void DumpInstructions(uint8_t *memory, uint16_t pc, uint16_t numToDump)
{
	printf("Dumping Instructions:\n\n");

	uint16_t i = 0;
	while (i++ < numToDump)
		pc += Disassemble(memory, pc);
}