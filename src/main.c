#include "chip8.h"

int
main(void)
{
	struct Chip8 vm;
	chip8_init(&vm);
	if (!chip8_load_rom(&vm, "")) {
	}
	return 0;
}
