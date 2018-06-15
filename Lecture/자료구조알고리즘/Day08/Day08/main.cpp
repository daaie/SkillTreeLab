#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* Infix = "2 - 10 / 5 * 6 + 4";

int main()
{
	char* token = strtok(Infix, " ");
	while (token != NULL)
	{
		printf("%s\n", token);

		char* token = strtok(token, " ");
	}

	return 0;
}