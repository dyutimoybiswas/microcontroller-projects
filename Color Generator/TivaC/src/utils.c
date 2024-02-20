#include "utils.h"
#include "io_handler.h"

uint32_t str_length(const char* str)
{
	uint32_t len = 0;
	while(str[len++] != NULL_CHAR);
	return len;
}

int char_to_int(char ch)
{
	switch(ch)
	{
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		default:
			return -1;
	}
}
