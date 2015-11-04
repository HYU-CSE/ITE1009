#include "object.h"

object create_object(int x, int y, int w, int h, int border, int back, long cBorder, long cBack, char * name, char * tag)
{
	object result;
	result.x = x;
	result.y = y;
	result.w = w;
	result.h = h;
	result.border = border;
	result.back = back;
	result.cBorder = cBorder;
	result.cBack = cBack;
	result.name = (char*)malloc(sizeof(strlen(name)) + sizeof(char));
	if(name!=NULL)
		strcpy(result.name, name);
	result.tag = (char*)malloc(sizeof(strlen(tag)) + sizeof(char));
	if(tag != NULL)
	strcpy(result.tag, tag);
	return result;
}
object_node * create_objectNodeRAW()
{
	object_node * result = (object_node*)malloc(sizeof(object_node));
	result->next = NULL;
	result->obj = create_object(0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL);
	return result;
}
object_node * create_objectNode(object obj)
{
	object_node * result = create_objectNodeRAW();
	result->obj = obj;
	return result;
}
object_node * pre_of_node(object_node * head, object_node * node)
{
	object_node * result = NULL ,* ptr = head;
	while ((ptr) != node)
	{
		result = ptr;
		ptr = ptr->next;
	}
	return result;
}
object_node * find_at_list_name(object_node * head, char * name)
{
	object_node * result = head;
	while (strcmp((result = result->next)->obj.name, name));
	return result;
}
object_node * find_at_list_index(object_node * head, size_t index)
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
void add_to_end_of_list(object_node * objList, object obj)
{
	object_node * tail = pre_of_node(objList, NULL);
	tail->next = create_objectNode(obj);
}
void change_list(object_node * node, object obj)
{
	node->obj = obj;
}
