/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#define MEMORY_START 0x200

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * Decodes two bytes of a binary code starting at
 * position specified in parameters.
 *
 * @param code_buffer		the buffer containing the binary code
 * @param program_counter	the position where the function should decode
 *							the two next bytes
 * @since					1.0
 */
void decode_instruction(uint8_t* code_buffer, int program_counter)
{
	uint8_t* code = &code_buffer[program_counter];
	uint8_t first_nibble = (code[0] >> 4);

	printf("%04x %02x %02x ", program_counter, code[0], code[1]);
	switch (first_nibble)
	{
	case 0x00:
	{
		if (code[1] == 0xE0)
		{
			printf("%-10s", "CLR");
		}
		else
		{
			printf("%-10s", "RET");
		}
		break;
	}
	case 0x01:
	{
		printf("%-10s 0x%01x%02x", "JMP", (code[0] & 0x0F), code[1]);
		break;
	}
	case 0x02: printf("2 not handled yet."); break;
	case 0x03: printf("3 not handled yet."); break;
	case 0x04: printf("4 not handled yet."); break;
	case 0x05: printf("5 not handled yet."); break;
	case 0x06:
	{
		uint8_t reg_position = code[0] & 0x0F;
		printf("%-10s V%01x,0x%02x", "MVI", reg_position, code[1]);
		break;
	}
	case 0x07:
	{
		uint8_t reg_position = code[0] & 0x0F;
		printf("%-10s V%01x,0x%02x", "ADD", reg_position, code[1]);
		break;
	}
	case 0x08: printf("8 not handled yet."); break;
	case 0x09: printf("9 not handled yet."); break;
	}
}

/**
 * Main function which iterates over each two bytes part
 * of a binary code read from a file which path is specified
 * in first argument and decode it.
 *
 * @since 1.0
 */
int main(int argc, char** argv)
{
	FILE* rom;
	fopen_s(&rom, argv[1], "rb");

	if (rom == NULL)
	{
		printf("Can't read rom %s.", argv[1]);
		exit(1);
	}

	// Obtaining file size.
	fseek(rom, 0L, SEEK_END);
	int file_size = ftell(rom);
	// Return to file begining.
	fseek(rom, 0L, SEEK_SET);

	// Store the binary code of the file with fread.
	unsigned char *code_buffer = malloc(file_size + MEMORY_START);
	fread(code_buffer + MEMORY_START, file_size, 1, rom);
	fclose(rom);

	int program_counter = MEMORY_START;
	while (program_counter < (file_size + MEMORY_START))
	{
		decode_instruction(code_buffer, program_counter);
		program_counter += 2;
		printf("\n");
	}

	return EXIT_SUCCESS;
}