#include <stdlib.h>
#include <stdio.h>

typedef struct list_{
	int value;
	struct list_ * next;
}list;

//list�� ���� ����ϴ�.
list * create();

//list�� ���̸� �����ݴϴ�.
int list_length(list * head);
//list���� value�� �����ϴ��� ���θ� �˷��ݴϴ�.
int exist(list * head, int value);

//list���� value�� �� ��°���� �ε����� ã�� ��ȯ�մϴ�.
int find_index(list * head, int value);
//list���� value�� ã�Ƽ� list*���·� ��ȯ�մϴ�.
list * find(list * head, int value);
//list���� value���� ���� list�� �� ��带 ã�Ƽ� list* ���·� ��ȯ�մϴ�.
list * find_front_of(list * head, list * node);

//list���� node�� �����մϴ�.
int remove_node(list * head, list * node);

//node ������ value���� ���� ��带 �߰��մϴ�.
void insert_next(int value, list * node);
//list�� �������� value���� ���� ��带 �߰��մϴ�.
void insert(list * head, int value);

//list�� ���� ����մϴ�.
void showList(list * head);