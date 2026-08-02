#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Chip8 {
	uint8_t registers[16];
	uint8_t memory[4096];
	uint16_t index;
	uint16_t pc;
	uint16_t stack[16];
	uint8_t sp;
	uint8_t delay_timer;
	uint8_t sound_timer;
	uint8_t keypad[16];
	uint8_t video[32][64];
};

static const size_t START_ADDRESS = 0x200;
static const size_t FONTSET_START_ADDRESS = 0x50;

static const uint8_t FONTSET[] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

_Static_assert(sizeof FONTSET == 16 * 5, "CHIP-8 fontset has the wrong size");

void chip8_init(struct Chip8 *self);
bool chip8_load_rom(struct Chip8 *self, const char *filename) __attribute__((warn_unused_result));

void chip8_op_00E0(struct Chip8 *self);
void chip8_op_00EE(struct Chip8 *self);
void chip8_op_1nnn(struct Chip8 *self, uint16_t opcode);
void chip8_op_2nnn(struct Chip8 *self, uint16_t opcode);

void
chip8_init(struct Chip8 *self)
{
	*self = (struct Chip8) { 0 };
	self->pc = START_ADDRESS;

	memcpy(&self->memory[FONTSET_START_ADDRESS], FONTSET, sizeof FONTSET);
}

bool
chip8_load_rom(struct Chip8 *self, const char *filename)
{
	bool success = false;
	uint8_t *buffer = NULL;

	FILE *file = fopen(filename, "rb");
	if (file == NULL) {
		return success;
	}
	if (fseek(file, 0, SEEK_END) != 0) {
		goto cleanup;
	}
	long size = ftell(file);
	if (size <= 0) {
		goto cleanup;
	}
	long capacity = (long) (sizeof self->memory - START_ADDRESS);
	if (size > capacity) {
		goto cleanup;
	}
	if (fseek(file, 0, SEEK_SET) != 0) {
		goto cleanup;
	}
	buffer = malloc(sizeof(uint8_t) * (size_t) size);
	if (buffer == NULL) {
		goto cleanup;
	}
	size_t bytes = fread(buffer, sizeof(uint8_t), (size_t) size, file);
	if (bytes != (size_t) size) {
		goto cleanup;
	}
	memcpy(&self->memory[START_ADDRESS], buffer, size);
	success = true;

cleanup:
	free(buffer);
	fclose(file);
	return success;
}

void
chip8_op_00E0(struct Chip8 *self)
{
	memset(self->video, 0, sizeof self->video);
}

/*@
  requires \valid(self);
  requires 0 < self->sp <= 16;

  assigns self->sp, self->pc;

  ensures self->sp == \old(self->sp) - 1;
  ensures self->pc == \old(self->stack[self->sp - 1]);
*/
void
chip8_op_00EE(struct Chip8 *self)
{
	self->sp -= 1;
	self->pc = self->stack[self->sp];
}

void
chip8_op_1nnn(struct Chip8 *self, uint16_t opcode)
{
	uint16_t address = opcode & 0x0FFFu;
	self->pc = address;
}

void
chip8_op_2nnn(struct Chip8 *self, uint16_t opcode)
{
	uint16_t address = opcode & 0x0FFFu;
	self->stack[self->sp] = self->pc;
	self->sp += 1;
	self->pc = address;
}

int
main(void)
{
	struct Chip8 vm;
	chip8_init(&vm);
	if (!chip8_load_rom(&vm, "")) {
	}
	return 0;
}
