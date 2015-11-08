#include "list.h"

int list_length(list * head)
{
	int result = 0;
	for (list * iter = head->next; iter != NULL; iter = iter->next, result++);
	return result;
}
list * create()
{
	//�� list* �� ����ϴ�.
	list * result = (list*)malloc(sizeof(list));
	result->next = NULL;
	return result;
}
int exist(list * head, int value)
{
	//head�� �־��� list�� ��ȸ�ϸ鼭 value���� ���� ��带 ã���ϴ�. ���ٸ� 0 �ִٸ� 1�� ��ȯ
	for (list * iter = head->next; iter != NULL; iter = iter->next)
		if (iter->value == value)
			return 1;
	return 0;
}
int find_index(list * head, int value)
{
	//head�� �־��� list�� ��ȸ�ϸ鼭 value���� ���� ��带 ã�� �� index�� ��ȯ�մϴ�. ���ٸ� 0��ȯ
	int result = 0;
	for (list * iter = head->next; iter != NULL; iter = iter->next, result++)
		if (iter->value == value)
			return result;
	return 0;
}
list * find(list * head, int value)
{
	//head�� �־��� list�� ��ȸ�ϸ鼭 value���� ���� ��带 ã�� ��ȯ�մϴ�. ���ٸ� NULL��ȯ
	for (list * iter = head->next; iter != NULL; iter = iter->next)
		if (iter->value == value)
			return iter;
	return NULL;
}
list * find_front_of(list * head, list * node)
{
	//result�� iter�� �� ��带 ����ֽ��ϴ�.
	//iter�� head�� �־��� list�� ��ȸ�ϸ鼭 node�� ã��, result�� ��ȯ�ϸ�
	//result->next = node�� �����ϴ� result�� ��ȯ�˴ϴ�.
	list * result = head, *iter = head->next;
	while (iter != node)
	{
		result = iter;
		iter = iter->next;
	}
	return result;
}
int remove_node(list * head, list * node)
{
	//node�� NULL, �� ���� ������ �� ���� 0�� ��ȯ�մϴ�.
	if (node == NULL)
		return 0;

	//node�� ���� ��带 ã���ϴ�.
	list * prev = find_front_of(head, node);

	//prev�� ������ ������ �� ���� 0�� ��ȯ�մϴ�. <- head��� �Ҹ�
	if (prev == NULL)
		return 0;


	//�� �۾��� prev -> node -> next �� ���¿��� node�� �����Ǹ鼭
	//node�� �޺κ��� �ٽ� �������ִ� �۾��Դϴ�.
	prev->next = node->next;

	//node�� �Ҵ� �������ݴϴ�.
	free(node);
	return 1;
}
void insert_next(int value, list * node)
{
	//value���� ���� ���ο� ��带 ����ϴ�.
	list * newNode = create();
	newNode->value = value;

	//node������ �� ��带 ��ġ��ŵ�ϴ�.
	newNode->next = node->next;
	node->next = newNode;
}
void insert(list * head, int value)
{
	insert_next(value, find_front_of(head, NULL));
}
void showList(list * head)
{
	for (list * iter = head->next; iter != NULL; iter = iter->next)
		printf("%d -> ", iter->value);
	printf("\n");
}