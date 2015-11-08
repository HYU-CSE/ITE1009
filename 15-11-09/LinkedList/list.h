#include <stdlib.h>
#include <stdio.h>

typedef struct list_{
	int value;
	struct list_ * next;
}list;

//list의 새로 만듭니다.
list * create();

//list의 길이를 구해줍니다.
int list_length(list * head);
//list에서 value가 존재하는지 여부를 알려줍니다.
int exist(list * head, int value);

//list에서 value가 몇 번째인지 인덱스를 찾아 반환합니다.
int find_index(list * head, int value);
//list에서 value를 찾아서 list*형태로 반환합니다.
list * find(list * head, int value);
//list에서 value값을 갖는 list의 앞 노드를 찾아서 list* 형태로 반환합니다.
list * find_front_of(list * head, list * node);

//list에서 node를 삭제합니다.
int remove_node(list * head, list * node);

//node 다음에 value값을 갖는 노드를 추가합니다.
void insert_next(int value, list * node);
//list의 마지막에 value값을 갖는 노드를 추가합니다.
void insert(list * head, int value);

//list를 전부 출력합니다.
void showList(list * head);