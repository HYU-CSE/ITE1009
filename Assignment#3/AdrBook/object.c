#include "object.h"

object create_object(int x, int y, int w, int h, int border, long cBorder, long cBack, char * name, char * number)
{
	object result;
	result.x = x;
	result.y = y;
	result.w = w;
	result.h = h;
	result.border = border;
	result.cBorder = cBorder;
	result.cBack = cBack;
	result.name = (char*)malloc(sizeof(strlen(name)) + sizeof(char));
	strcpy(result.name, name);
	result.number = (char*)malloc(sizeof(strlen(number)) + sizeof(char));
	strcpy(result.number, number);
	return result;
}
object_node * create_objectNode()
{
	object_node * result = (object_node*)malloc(sizeof(object_node));
	result->next = NULL;
	return result;
}
object_node * create_objectNode(object obj)
{
	object_node * result = create_objectNode();
	result->obj = obj;
	return result;
}
object_node * pre_of_node(object_node * head, object_node * node)
{
	object_node * result = head;
	while ((result = result->next) != node);
	return result;
}
object_node * find_at_list(object_node * head, char * name)
{
	object_node * result = head;
	while (strcmp((result = result->next)->obj.name, name));
	return result;
}
object_node * find_at_list(object_node * head, size_t index)
{
	object_node * result = head;
	size_t cnt = 0;
	while (cnt++ < index)
		result = result->next;
	return result;
}
void delete_at_list(object_node * head, object_node * obj)
{
	if (obj == NULL)
		return;
	object_node * front = pre_of_node(head, obj);
	front->next = obj->next;
	free(obj);
}
void add_to_list(object_node * objList, object_node * pointer, object obj)
{
	object_node * front = pointer;
	object_node * node = create_objectNode(obj);
	node->next = front->next;
	front->next = node;
}
void add_to_list(object_node * objList, object obj)
{
	object_node * tail = pre_of_node(objList, NULL);
	tail->next = create_objectNode(obj);
}