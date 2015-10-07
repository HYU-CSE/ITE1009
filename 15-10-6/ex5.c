#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
	int index;
	for(index = 0; index < argc; index++)
		printf("arg[%d]: %s\n",index, argv[index]);
	return 0;
}