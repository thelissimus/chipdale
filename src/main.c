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

void chip8_init(struct Chip8 *self);
bool chip8_load_rom(struct Chip8 *self, const char *filename);

void
chip8_init(struct Chip8 *self)
{
	*self = (struct Chip8) { 0 };
	self->pc = START_ADDRESS;
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

int
main(void)
{
	return 0;
}
