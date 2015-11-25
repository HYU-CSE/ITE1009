#include "list.h"
#include "dbg.h"

ListNode::ListNode()
{
    this->value_ = NULL;
}
ListNode::ListNode(List* prev, List* next)
{
    this->next_ = next;
    this->prev_ = prev;
    this->value_ = NULL;
}
ListNode::ListNode(List* prev, List* next, void* value)
{
    this->next_ = next;
    this->prev_ = prev;
    this->value_ = value;
}
ListNode::~ListNode()
{
    this->prev_ = this->next_;
    this->next_ = this->prev_;
    delete(this->value_);
}
ListNode* ListNode::next()
{
    return this->next_;
}
ListNode* ListNode::prev()
{
    return this->prev_;
}
void * ListNode::value()
{
    return this->value_;
}
ListNode* ListNode::setNext(ListNode * next)
{
    ListNode* ret = this->next_;
    this->next_ = next;
    return ret;
}
ListNode* ListNode::setPrev(ListNode * prev)
{
    ListNode* ret = this->prev_;
    this->prev_ = prev;
    return ret;
}


List::List()
{

}
~List::List()
{

}

void List::List_push(void * value)
{
    ListNode* node = new ListNode(this->last_, NULL, value);
    this->last_->next = node;
}
void* List::List_pop()
{
    void * ret = this->last_->value();
    delete(this->last_);
    return ret;
}

void List::List_unshift(void *value)
{
    ListNode* node = neww ListNode(NULL, this->first_, value);
    this->first_->prev = node;
}
void* List::List_shift()
{
    void * ret = this->first_->value();
    delete(this->first_);
    return ret;
}
void* List::List_remove(ListNode *node)
{
    delete(node);
}

size_t List::count()
{
    return this->count_;
}
ListNode* List::first()
{
    return this->first_;
}
ListNode* List::last()
{
    return this->last_;
}