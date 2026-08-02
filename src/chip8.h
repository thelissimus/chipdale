#pragma once

#include <stdbool.h>
#include <stdint.h>

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

void chip8_init(struct Chip8 *self);
bool chip8_load_rom(struct Chip8 *self, const char *filename) __attribute__((warn_unused_result));

void chip8_op_00E0(struct Chip8 *self);
void chip8_op_00EE(struct Chip8 *self);
void chip8_op_1nnn(struct Chip8 *self, uint16_t opcode);
void chip8_op_2nnn(struct Chip8 *self, uint16_t opcode);
void chip8_op_3xkk(struct Chip8 *self, uint16_t opcode);
void chip8_op_4xkk(struct Chip8 *self, uint16_t opcode);
void chip8_op_5xy0(struct Chip8 *self, uint16_t opcode);
void chip8_op_6xkk(struct Chip8 *self, uint16_t opcode);
void chip8_op_7xkk(struct Chip8 *self, uint16_t opcode);
void chip8_op_8xy0(struct Chip8 *self, uint16_t opcode);
void chip8_op_8xy1(struct Chip8 *self, uint16_t opcode);
void chip8_op_8xy2(struct Chip8 *self, uint16_t opcode);
void chip8_op_8xy3(struct Chip8 *self, uint16_t opcode);
void chip8_op_8xy4(struct Chip8 *self, uint16_t opcode);
void chip8_op_8xy5(struct Chip8 *self, uint16_t opcode);
void chip8_op_8xy6(struct Chip8 *self, uint16_t opcode);
void chip8_op_8xy7(struct Chip8 *self, uint16_t opcode);
