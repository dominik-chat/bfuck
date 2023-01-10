/*
 * Copyright 2023 Dominik Chat
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


#define MEM_SIZE 65536
#define printfe(...) fprintf (stderr, __VA_ARGS__)


char *code_mem;
size_t code_size;
size_t code_ptr;
unsigned char data_mem[MEM_SIZE];
size_t data_ptr;


int jump(void)
{
	size_t l_cnt = 1;
	size_t r_cnt = 0;

	do {
		if (code_ptr == code_size) {
			printfe("Tried to increment code_ptr=code_size\n");
			return EFBIG;
		}

		code_ptr++;
		if (code_mem[code_ptr] == '[') {
			l_cnt++;
		} else if (code_mem[code_ptr] == ']') {
			r_cnt++;
		}
	} while (l_cnt != r_cnt);

	return 0;
}

int jump_back(void)
{
	size_t l_cnt = 0;
	size_t r_cnt = 1;

	do {
		if (code_ptr == 0) {
			printfe("Tried to decrement code_ptr=0\n");
			return EFBIG;
		}

		code_ptr--;
		if (code_mem[code_ptr] == '[') {
			l_cnt++;
		} else if (code_mem[code_ptr] == ']') {
			r_cnt++;
		}
	} while (l_cnt != r_cnt);

	return 0;
}

int exe_instr(char c)
{
	int tmp;

	switch (c) {
	case '+':
		data_mem[data_ptr]++;
		break;
	case '-':
		data_mem[data_ptr]--;
		break;
	case '<':
		if (data_ptr == 0) {
			printfe("Tried to decrement data_ptr=0\n");
			return EFBIG;
		}

		data_ptr--;
		break;
	case '>':
		if (data_ptr == (MEM_SIZE - 1)) {
			printfe("Tried to decrement data_ptr=%d\n", (MEM_SIZE - 1));
			return EFBIG;
		}

		data_ptr++;
		break;
	case ',':
		tmp = getchar();
		if (tmp == EOF) {
			printfe("Error reading from input\n");
			return EIO;
		}
		data_mem[data_ptr] = tmp;
		break;
	case '.':
		tmp = putchar(data_mem[data_ptr]);
		if (tmp == EOF) {
			printfe("Error writing to output\n");
			return EIO;
		}
		break;
	case '[':
		if (data_mem[data_ptr] == 0) {
			jump();
		}
		break;
	case ']':
		if (data_mem[data_ptr] != 0) {
			jump_back();
		}
		break;
	default:
		break;
	}

	code_ptr++;
	return 0;
}


int main(int argc, char **argv)
{
	FILE *src_file;
	long size;
	int err;

	if (argc < 2) {
		printfe("File not specified\n");
		return EINVAL;
	}
	
	src_file = fopen(argv[1], "r");
	if (!src_file) {
		printfe("File not found\n");
		return ENOENT;
	}

	fseek(src_file, 0, SEEK_END);
	size = ftell(src_file);
	code_size = size;
	rewind(src_file);

	code_mem = malloc(sizeof(unsigned char) * size);
	if (code_mem == NULL) {
		printfe("Unable to allocate code memory\n");
		return ENOMEM;
	}
	err = fread(code_mem, 1, size, src_file);
	if (err != size) {
		printfe("Unable to read brainfuck file\n");
		return EIO;
	}
	fclose(src_file);
	
	memset(data_mem, 0, sizeof(data_mem));
	data_ptr = 0;
	code_ptr = 0;
	
	while (code_ptr < code_size) {
		err = exe_instr(code_mem[code_ptr]);
		if (err) {
			return err;
		}
	}

	free(code_mem);
	
	return 0;
}
