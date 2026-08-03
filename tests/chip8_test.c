#include <criterion/criterion.h>

#include "chip8.h"

Test(chip8, init_sets_program_counter)
{
	struct Chip8 vm;
	chip8_init(&vm);

	cr_expect_eq(vm.pc, 0x200u);
}
