#ifndef GB_TYPES_H_
#define GB_TYPES_H_

#define MEMORY_SIZE		64000
#define RAMSIZE			32000
#define	VRAMSIZE		16000

typedef struct CpuFlags {
	uint8_t z:1; // Zero
	uint8_t n:1; // Subtract
	uint8_t h:1; // Half Carry
	uint8_t c:1; // Carry
	uint8_t pad:4; // Not Used
} CpuFlags;

typedef struct CpuState {
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t f;
	uint8_t h;
	uint8_t l;
	uint16_t sp;
	uint16_t pc;
	CpuFlags flags;

	uint8_t *memory;
} CpuState;

#endif