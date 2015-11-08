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
				printf("��� ������ �����Ұ��ΰ�?: ");
				scanf("%d %d", &arg, &input);

				if (!list_length(List))
					insert(List, input);
				else if (!exist(List, input))
					insert_next(input, find(List, arg));
				else
					printf("�̹� ����\n");
				getchar();
				break;
			case 'd':
				printf("������ ������ΰ�?: ");
				scanf("%d", &input);

				if (!remove_node(List, find(List, input)))
					printf("�׷��ž��µ�?\n");

				getchar();
				break;
			case 'f':
				printf("������ ã�����ΰ�?: ");
				scanf("%d", &input);

				printf("%d�� %d��°�� ����\n", input, 1 + find_index(List, input));
				getchar();
				break;
			case 'q':
				printf("����\n");
				return 0;
				break;
			default:
				break;
		}
		showList(List);
		printf("������ �� ���ΰ�?: ");
	} while (scanf("%c", &query));
	return 0;
}