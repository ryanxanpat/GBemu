#ifndef GB_TYPES_H_
#define GB_TYPES_H_

#define MEMORY_SIZE		64000
#define RAMSIZE			32000
#define	VRAMSIZE		16000

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
	uint8_t flags; // Bit 7-Zero Flag, 6-N (Subtract Flag), 5-H (Half Carry Flag), 4-Carry

	uint8_t *memory;
} CpuState;

#endif