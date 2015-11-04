#pragma once
#include <stdlib.h>
#include <string.h>

#pragma warning ( disable : 4996) 
typedef struct  object_ {
	int x, y, w, h;
	int border, back;
	size_t index;
	char * name;
	char * tag;
	long cBorder, cBack;
}object;
typedef struct object_node_ {
	struct object_ obj;
	struct object_node_ * next;
}object_node;

#define OBJECT_FOREACH(idx, list) for(object_node * idx = list->next; idx != NULL; idx = idx->next)

object create_object(int, int, int, int, int,int, long, long, char *, char *);
object_node * create_objectNodeRAW();
object_node * create_objectNode(object);
object_node * pre_of_node(object_node * head, object_node *);
object_node * find_at_list_name(object_node *, char *);
object_node * find_at_list_index(object_node *, size_t);
void change_list(object_node *, object);
void delete_at_list(object_node *, object_node *);
void add_to_list(object_node *, object_node *, object);
void add_to_end_of_list(object_node *, object);