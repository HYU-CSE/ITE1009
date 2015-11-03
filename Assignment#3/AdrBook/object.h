#pragma once
#include <stdlib.h>
#include <string.h>

typedef struct object_ {
	int x, y, w, h;
	int border;
	size_t index;
	char * name;
	char * number;
	long cBorder, cBack;
} object;
typedef struct object_node_ {
	object obj;
	struct object_node * next;
} object_node;

object create_object(int, int, int, int, int, long, long, char *, char *);
object_node * create_objectNode();
object_node * create_objectNode(object);
object_node * pre_of_node(object_node * head, object_node *);
object_node * find_at_list(object_node *, char *);
object_node * find_at_list(object_node *, size_t);
void delete_at_list(object_node *, object_node *);
void add_to_list(object_node *, object_node *, object);
void add_to_list(object_node *, object);