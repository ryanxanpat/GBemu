#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

#define DEBUG 1

CpuState* InitCpu()
{
	CpuState *cpu = (CpuState *) malloc(sizeof(CpuState));
	if (cpu == NULL)
	{
		printf("Failed to allocate cpu");
		return NULL;
	}

	memset(cpu, 0, sizeof(CpuState));

	cpu->memory = (uint8_t *) malloc(MEMORY_SIZE * sizeof(uint8_t));
	if (cpu->memory == NULL)
	{
		printf("Failed to allocate space for Memory Map\n");
		free(cpu);
		return NULL;
	}

	memset(cpu->memory, 0, MEMORY_SIZE);

	FILE* rom = fopen("roms/boot.gb", "rb");
	if (rom == NULL)
	{
		printf("Could not open bootstrap ROM\n");
		free(cpu->memory);
		free(cpu);
		return NULL;
	}
	printf("Opened bootstrap rom\n\n");

	if (!fgets(cpu->memory, 257, rom))
	{
		printf("Couldn't read in bootstrap ROM\n");
		free(cpu->memory);
		free(cpu);
		return NULL;
	}

	fclose(rom);

	DumpMemory(cpu->memory, 0, 300);
	PrintContext(cpu);

	return cpu;
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
	printf("\n\n");
}

void Emulate(CpuState* cpu)
{
	if (DEBUG)
	{
		//printf("Addr      Op      Instruction\n");
		printf("Executing: ");
		Disassemble(cpu->memory, cpu->pc);
		printf("\n");
	}

	uint8_t *opCode = &cpu->memory[cpu->pc];
	switch (*opCode)
	{
		case 0x00: break;
		case 0x01:	// LD BC, d16
			cpu->c = opCode[1]; // Low Register
			cpu->b = opCode[2]; // High register
			cpu->pc += 2;
			break;
		case 0x02: UnimplementedInstruction(cpu); break;
		case 0x03: UnimplementedInstruction(cpu); break;
		case 0x04: cpu->b++; break;
		case 0x05: cpu->b--; break;
		case 0x06:	// LD B, d8
			cpu->b = opCode[1];
			cpu->pc++;
			break;
		case 0x07: UnimplementedInstruction(cpu); break;
		case 0x08: UnimplementedInstruction(cpu); break;
		case 0x09: UnimplementedInstruction(cpu); break;
		case 0x0A: UnimplementedInstruction(cpu); break;
		case 0x0B: UnimplementedInstruction(cpu); break;
		case 0x0C: cpu->c++; break;
		case 0x0D: cpu->c--; break;
		case 0x0E:	// LD C, d8
			cpu->c = opCode[1];
			cpu->pc++;
			break;
		case 0x0F: UnimplementedInstruction(cpu); break;


		case 0x10: UnimplementedInstruction(cpu); break;
		case 0x11: UnimplementedInstruction(cpu); break;
		case 0x12: UnimplementedInstruction(cpu); break;
		case 0x13: UnimplementedInstruction(cpu); break;
		case 0x14: cpu->d++; break;
		case 0x15: cpu->d--; break;
		case 0x16:	// LD D, d8
			cpu->d = opCode[1];
			cpu->pc++;
			break;
		case 0x17: UnimplementedInstruction(cpu); break;
		case 0x18: UnimplementedInstruction(cpu); break;
		case 0x19: UnimplementedInstruction(cpu); break;
		case 0x1A: UnimplementedInstruction(cpu); break;
		case 0x1B: UnimplementedInstruction(cpu); break;
		case 0x1C: cpu->e++; break;
		case 0x1D: cpu->e--; break;
		case 0x1E:
			cpu->e = opCode[1];
			cpu->pc++;
			break;
		case 0x1F: UnimplementedInstruction(cpu); break;


		case 0x20: UnimplementedInstruction(cpu); break;
		case 0x21: // LD HL, d16
			cpu->h = opCode[2]; // 50% chance this is right
			cpu->l = opCode[1];
			cpu->pc += 2;
			break;
		case 0x22: UnimplementedInstruction(cpu); break;
		case 0x23: UnimplementedInstruction(cpu); break;
		case 0x24: cpu->h++; break;
		case 0x25: cpu->h--; break;
		case 0x26:	// LD H, d8
			cpu->h = opCode[1];
			cpu->pc++;
			break;
		case 0x27: UnimplementedInstruction(cpu); break;
		case 0x28: UnimplementedInstruction(cpu); break;
		case 0x29: UnimplementedInstruction(cpu); break;
		case 0x2A: UnimplementedInstruction(cpu); break;
		case 0x2B: UnimplementedInstruction(cpu); break;
		case 0x2C: cpu->l++; break;
		case 0x2D: cpu->l--; break;
		case 0x2E:
			cpu->l = opCode[1];
			cpu->pc++;
			break;
		case 0x2F: UnimplementedInstruction(cpu); break;


		case 0x30: UnimplementedInstruction(cpu); break;
		case 0x31:
			cpu->sp = (opCode[2] << 8) | opCode[1];
			cpu->pc += 2;
			break;
		case 0x32: UnimplementedInstruction(cpu); break;
		case 0x33: cpu->sp++; break;
		case 0x34: UnimplementedInstruction(cpu); break;
		case 0x35: UnimplementedInstruction(cpu); break;
		case 0x36: UnimplementedInstruction(cpu); break;
		case 0x37: UnimplementedInstruction(cpu); break;
		case 0x38: UnimplementedInstruction(cpu); break;
		case 0x39: UnimplementedInstruction(cpu); break;
		case 0x3A: UnimplementedInstruction(cpu); break;
		case 0x3B: UnimplementedInstruction(cpu); break;
		case 0x3C: cpu->a++; break;
		case 0x3D: cpu->a--; break;
		case 0x3E:
			cpu->a = opCode[1];
			cpu->pc++;
			break;
		case 0x3F: UnimplementedInstruction(cpu); break;


		case 0x40: break; // LD B, B
		case 0x41: cpu->b = cpu->c; break;
		case 0x42: cpu->b = cpu->d; break;
		case 0x43: cpu->b = cpu->e; break;
		case 0x44: cpu->b = cpu->h; break;
		case 0x45: cpu->b = cpu->l; break;
		case 0x46: UnimplementedInstruction(cpu); break;
		case 0x47: cpu->b = cpu->a; break;
		case 0x48: cpu->c = cpu->b; break;
		case 0x49: break;
		case 0x4A: cpu->c = cpu->d; break;
		case 0x4B: cpu->c = cpu->e; break;
		case 0x4C: cpu->c = cpu->h; break;
		case 0x4D: cpu->c = cpu->l; break;
		case 0x4E: UnimplementedInstruction(cpu); break;
		case 0x4F: cpu->c = cpu->a; break;


		case 0x50: cpu->d = cpu->b; break;
		case 0x51: cpu->d = cpu->c; break;
		case 0x52: break;
		case 0x53: cpu->d = cpu->e; break;
		case 0x54: cpu->d = cpu->h; break;
		case 0x55: cpu->d = cpu->l; break;
		case 0x56: UnimplementedInstruction(cpu); break;
		case 0x57: cpu->d = cpu->a; break;
		case 0x58: cpu->e = cpu->b; break;
		case 0x59: cpu->e = cpu->c; break;
		case 0x5A: cpu->e = cpu->d; break;
		case 0x5B: break;
		case 0x5C: cpu->e = cpu->h; break;
		case 0x5D: cpu->e = cpu->l; break;
		case 0x5E: UnimplementedInstruction(cpu); break;
		case 0x5F: cpu->e = cpu->a; break;


		case 0x60: cpu->h = cpu->b; break;
		case 0x61: cpu->h = cpu->c; break;
		case 0x62: cpu->h = cpu->d; break;
		case 0x63: cpu->h = cpu->e; break;
		case 0x64: break;
		case 0x65: cpu->h = cpu->l; break;
		case 0x66: UnimplementedInstruction(cpu); break;
		case 0x67: cpu->h = cpu->a; break;
		case 0x6D:; break;
		case 0x6E: UnimplementedInstruction(cpu); break;

		case 0x68:
		case 0x70: cpu->l = cpu->b; break;

		case 0x69:
		case 0x71: cpu->l = cpu->c; break;

		case 0x6A:
		case 0x72: cpu->l = cpu->d; break;

		case 0x6B:
		case 0x73: cpu->l = cpu->e; break;

		case 0x6C:
		case 0x74: cpu->l = cpu->h; break;

		case 0x75: break;

		case 0x76: UnimplementedInstruction(cpu); break;

		case 0x6F:
		case 0x77: cpu->l = cpu->a; break;

		case 0x78: cpu->a = cpu->b; break;
		case 0x79: cpu->a = cpu->c; break;
		case 0x7A: cpu->a = cpu->d; break;
		case 0x7B: cpu->a = cpu->e; break;
		case 0x7C: cpu->a = cpu->h; break;
		case 0x7D: cpu->a = cpu->l; break;
		case 0x7E: UnimplementedInstruction(cpu); break;
		case 0x7F: cpu->a = cpu->a; break;


		case 0x80:
		case 0x81:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0x86:
		case 0x87:
		case 0x88:
		case 0x89:
		case 0x8A:
		case 0x8B:
		case 0x8C:
		case 0x8D:
		case 0x8E:
		case 0x8F:


		case 0x90:
		case 0x91:
		case 0x92:
		case 0x93:
		case 0x94:
		case 0x95:
		case 0x96:
		case 0x97:
		case 0x98:
		case 0x99:
		case 0x9A:
		case 0x9B:
		case 0x9C:
		case 0x9D:
		case 0x9E:
		case 0x9F: UnimplementedInstruction(cpu); break;


		case 0xA0:
			cpu->a &= cpu->b;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			cpu->flags.h = 1;
			break;
		case 0xA1:
			cpu->a &= cpu->c;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			cpu->flags.h = 1;
			break;
		case 0xA2:
			cpu->a &= cpu->d;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			cpu->flags.h = 1;
			break;
		case 0xA3:
			cpu->a &= cpu->e;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			cpu->flags.h = 1;
			break;
		case 0xA4:
			cpu->a &= cpu->h;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			cpu->flags.h = 1;
			break;
		case 0xA5:
			cpu->a &= cpu->l;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			cpu->flags.h = 1;
			break;
		case 0xA6: UnimplementedInstruction(cpu); break;
		case 0xA7:
			cpu->a &= cpu->a;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			cpu->flags.h = 1;
			break;
		case 0xA8: UnimplementedInstruction(cpu); break;
			cpu->a ^= cpu->b;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			break;
		case 0xA9:
			cpu->a ^= cpu->c;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			break;
		case 0xAA:
			cpu->a ^= cpu->d;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			break;
		case 0xAB:
			cpu->a ^= cpu->e;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			break;
		case 0xAC:
			cpu->a ^= cpu->h;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			break;
		case 0xAD:
			cpu->a ^= cpu->l;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			break;
		case 0xAE: UnimplementedInstruction(cpu); break;
		case 0xAF:
			cpu->a ^= cpu->a;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			break;


		case 0xB0:
			cpu->a |= cpu->b;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			break;
		case 0xB1:
			cpu->a |= cpu->c;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			break;
		case 0xB2:
			cpu->a |= cpu->d;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			break;
		case 0xB3:
			cpu->a |= cpu->e;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			break;
		case 0xB4:
			cpu->a |= cpu->h;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			break;
		case 0xB5:
			cpu->a |= cpu->l;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			break;
		case 0xB6: UnimplementedInstruction(cpu); break;
		case 0xB7:
			cpu->a |= cpu->a;
			memset(&cpu->flags, 0, sizeof(cpu->flags));
			if (cpu->a == 0)
				cpu->flags.z = 1;
			break;
		case 0xB8:
		case 0xB9:
		case 0xBA:
		case 0xBB:
		case 0xBC:
		case 0xBD:
		case 0xBE:
		case 0xBF:


		case 0xC0:
		case 0xC1:
		case 0xC2:
		case 0xC3:
		case 0xC4:
		case 0xC5:
		case 0xC6:
		case 0xC7:
		case 0xC8:
		case 0xC9:
		case 0xCA:
		case 0xCB:
		case 0xCC:
		case 0xCD:
		case 0xCE:
		case 0xCF:


		case 0xD0:
		case 0xD1:
		case 0xD2:
		case 0xD4:
		case 0xD5:
		case 0xD6:
		case 0xD7:
		case 0xD8:
		case 0xD9:
		case 0xDA:
		case 0xDC:
		case 0xDE:
		case 0xDF:


		case 0xE0:
		case 0xE1:
		case 0xE2:
		case 0xE5:
		case 0xE6:
		case 0xE7:
		case 0xE8:
		case 0xE9:
		case 0xEA:
		case 0xEE:
		case 0xEF:


		case 0xF0:
		case 0xF1:
		case 0xF2:
		case 0xF3:
		case 0xF5:
		case 0xF6:
		case 0xF7:
		case 0xF8:
		case 0xF9:
		case 0xFA:
		case 0xFB:
		case 0xFE:
		case 0xFF:

		default:   UnimplementedInstruction(cpu); break;
	}

	cpu->pc++;

	if (DEBUG)
		PrintContext(cpu);
}

void PrintContext(CpuState* cpu)
{
	printf("a = 0x%02X  b = 0x%02X  c = 0x%02X  d = 0x%02X\n", cpu->a, cpu->b, cpu->c, cpu->d);
	printf("e = 0x%02X  f = 0x%02X  h = 0x%02X  l = 0x%02X\n", cpu->e, cpu->f, cpu->h, cpu->l);
	printf("Flags: Z:%d N:%d H:%d C:%d\n", cpu->flags.z, cpu->flags.n, cpu->flags.h, cpu->flags.c);
	printf("sp = 0x%04X  pc = 0x%04X\n\n", cpu->sp, cpu->pc);
}

void UnimplementedInstruction(CpuState* cpu)
{
	printf("Error: Unimplemented instruction. Exiting...\n");
	exit(1);
}

uint8_t Disassemble(uint8_t *codeBuffer, uint16_t pc)
{
	uint8_t *opCode = &codeBuffer[pc];
	uint8_t opBytes;

	uint16_t imm;

	printf("0x%04X    0x%x    ", pc, *opCode);
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
			printf("LD HL, 0x%04X\n", (opCode[2] << 8) | opCode[1]);
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
			printf("LD SP, 0x%04X\n", (opCode[2] << 8) | opCode[1]);
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