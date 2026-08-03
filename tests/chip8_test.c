#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "chip8.h"

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

_Static_assert(sizeof FONTSET == (size_t) 16 * 5, "CHIP-8 fontset has the wrong size");

Test(chip8, init_sets_program_counter)
{
	struct Chip8 vm;
	chip8_init(&vm);

	cr_expect(eq(u16, vm.pc, 0x200u));
}

Test(chip8, init_loads_fontset)
{
	struct Chip8 vm;
	chip8_init(&vm);

	cr_expect_arr_eq(&vm.memory[0x50], FONTSET, sizeof FONTSET);
}

Test(chip8, op_8xy5_equality_is_not_a_borrow)
{
	struct Chip8 vm = { 0 };
	vm.registers[1] = 42;
	vm.registers[2] = 42;

	chip8_op_8xy5(&vm, 0x8125);

	cr_expect(eq(u8, vm.registers[1], 0));
	cr_expect(eq(u8, vm.registers[0xF], 1));
}

Test(chip8, op_8xy5_preserves_vf_as_x_operand)
{
	struct Chip8 vm = { 0 };
	vm.registers[0xF] = 5;
	vm.registers[2] = 4;

	chip8_op_8xy5(&vm, 0x8F25);

	cr_expect(eq(u8, vm.registers[0xF], 1));
}

Test(chip8, op_8xy5_preserves_vf_as_y_operand)
{
	struct Chip8 vm = { 0 };
	vm.registers[1] = 5;
	vm.registers[0xF] = 4;

	chip8_op_8xy5(&vm, 0x81F5);

	cr_expect(eq(u8, vm.registers[1], 1));
	cr_expect(eq(u8, vm.registers[0xF], 1));
}

Test(chip8, op_8xy6_preserves_vf_as_x_operand)
{
	struct Chip8 vm = { 0 };
	vm.registers[0xF] = 1;

	chip8_op_8xy6(&vm, 0x8F06);

	cr_expect(eq(u8, vm.registers[0xF], 1));
}

Test(chip8, op_8xy7_equality_is_not_a_borrow)
{
	struct Chip8 vm = { 0 };
	vm.registers[1] = 42;
	vm.registers[2] = 42;

	chip8_op_8xy7(&vm, 0x8127);

	cr_expect(eq(u8, vm.registers[1], 0));
	cr_expect(eq(u8, vm.registers[0xF], 1));
}

Test(chip8, op_8xy7_preserves_vf_as_x_operand)
{
	struct Chip8 vm = { 0 };
	vm.registers[0xF] = 5;
	vm.registers[2] = 7;

	chip8_op_8xy7(&vm, 0x8F27);

	cr_expect(eq(u8, vm.registers[0xF], 1));
}

Test(chip8, op_8xy7_preserves_vf_as_y_operand)
{
	struct Chip8 vm = { 0 };
	vm.registers[1] = 5;
	vm.registers[0xF] = 7;

	chip8_op_8xy7(&vm, 0x81F7);

	cr_expect(eq(u8, vm.registers[1], 2));
	cr_expect(eq(u8, vm.registers[0xF], 1));
}

Test(chip8, op_Cxkk_masks_a_random_byte)
{
	struct Chip8 vm;
	chip8_init(&vm);

	chip8_op_Cxkk(&vm, 0xC1A5);

	cr_expect(eq(u8, vm.registers[1], 0x5));
	cr_expect(eq(u32, vm.random_state, 0x477D20B7));
}
