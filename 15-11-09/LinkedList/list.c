#include "list.h"

int list_length(list * head)
{
	int result = 0;
	for (list * iter = head->next; iter != NULL; iter = iter->next, result++);
	return result;
}
list * create()
{
	//새 list* 을 만듭니다.
	list * result = (list*)malloc(sizeof(list));
	result->next = NULL;
	return result;
}
int exist(list * head, int value)
{
	//head로 주어진 list를 순회하면서 value값을 갖는 노드를 찾습니다. 없다면 0 있다면 1을 반환
	for (list * iter = head->next; iter != NULL; iter = iter->next)
		if (iter->value == value)
			return 1;
	return 0;
}
int find_index(list * head, int value)
{
	//head로 주어진 list를 순회하면서 value값을 갖는 노드를 찾아 그 index를 반환합니다. 없다면 0반환
	int result = 0;
	for (list * iter = head->next; iter != NULL; iter = iter->next, result++)
		if (iter->value == value)
			return result;
	return 0;
}
list * find(list * head, int value)
{
	//head로 주어진 list를 순회하면서 value값을 갖는 노드를 찾아 반환합니다. 없다면 NULL반환
	for (list * iter = head->next; iter != NULL; iter = iter->next)
		if (iter->value == value)
			return iter;
	return NULL;
}
list * find_front_of(list * head, list * node)
{
	//result는 iter의 전 노드를 담고있습니다.
	//iter는 head로 주어진 list를 순회하면서 node를 찾고, result를 반환하면
	//result->next = node를 만족하는 result가 반환됩니다.
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
	//node가 NULL, 빈 노드면 삭제할 수 없고 0을 반환합니다.
	if (node == NULL)
		return 0;

	//node의 이전 노드를 찾습니다.
	list * prev = find_front_of(head, node);

	//prev가 없으면 삭제할 수 없고 0을 반환합니다. <- head라는 소리
	if (prev == NULL)
		return 0;


	//이 작업은 prev -> node -> next 의 형태에서 node가 삭제되면서
	//node의 뒷부분을 다시 연결해주는 작업입니다.
	prev->next = node->next;

	//node를 할당 해제해줍니다.
	free(node);
	return 1;
}
void insert_next(int value, list * node)
{
	//value값을 갖는 새로운 노드를 만듭니다.
	list * newNode = create();
	newNode->value = value;

	//node다음에 새 노드를 위치시킵니다.
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