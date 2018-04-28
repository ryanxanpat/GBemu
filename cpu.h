#ifndef CPU_H_
#define CPU_H_

#include "gbTypes.h"

CpuState* InitCpu();
void DumpMemory(uint8_t* memory, uint16_t start, uint16_t size);
void Emulate(CpuState* cpu);
void UnimplementedInstruction(CpuState* cpu);
uint8_t Disassemble(uint8_t *codeBuffer, uint16_t pc);

#endif