#include <stdio.h>
#include "list.h"
#ifdef WIN32
#pragma warning (disable : 4996)
#endif

int main(int argc, char * argv[])
{
	char query = 0;

	list * List = create();

	do{
		int arg, input;
		switch (query)
		{
			case 'i':
				printf("어디에 무엇을 삽입할것인가?: ");
				scanf("%d %d", &arg, &input);

				if (!list_length(List))
					insert(List, input);
				else if (!exist(List, input))
					insert_next(input, find(List, arg));
				else
					printf("이미 있음\n");
				getchar();
				break;
			case 'd':
				printf("무엇을 지울것인가?: ");
				scanf("%d", &input);

				if (!remove_node(List, find(List, input)))
					printf("그런거없는뎅?\n");

				getchar();
				break;
			case 'f':
				printf("무엇을 찾을것인가?: ");
				scanf("%d", &input);

				printf("%d는 %d번째에 있음\n", input, 1 + find_index(List, input));
				getchar();
				break;
			case 'q':
				printf("ㅂㅂ\n");
				return 0;
				break;
			default:
				break;
		}
		showList(List);
		printf("무엇을 할 것인가?: ");
	} while (scanf("%c", &query));
	return 0;
}