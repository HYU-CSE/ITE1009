#ifndef lcthw_List_h
#define lcthw_List_h
#include <list>
#include <stdlib.h>

struct ListNode;

class ListNode{
public:
	ListNode();
	ListNode(List* next, List* prev);
	ListNode(List* next, List* prev, void* value);
	~ListNode();

	ListNode* next();
	ListNode* prev();
	void* value();

	ListNode* setNext(ListNode* next);
	ListNode* setPrev(ListNode* prev);
private:
	struct ListNode *next_;
	struct ListNode *prev_;
	void *value_;
};

class List{
public:
	List()
	~List()

	void List_destroy(List *list);
	void List_clear(List *list);
	void List_clear_destroy(List *list);

	void List_push(List *list, void *value);
	void *List_pop(List *list);

	void List_unshift(List *list, void *value);
	void *List_shift(List *list);
	void *List_remove(List *list, ListNode *node);

	size_t count();
	ListNode * first();
	ListNode * last();
private:
	size_t count_;
	ListNode *first_;
	ListNode *last_;
};

#define LIST_FOREACH(L, B, C) ListNode* b = NULL; ListNode* c = NULL; for(c = L->frist(); c != L->last(); b=c, c = c->next();)

#define LIST_BACKEACH(L, B, C) ListNode* b = NULL; ListNode* c = NULL; for(c = L->last(); c != L->first(); b=c, c = c->prev();)

#endif