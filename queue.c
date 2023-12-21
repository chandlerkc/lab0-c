#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (head)
        INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;

    struct list_head *list_node;
    struct list_head *safe;
    list_for_each_safe (list_node, safe, l) {
        element_t *element_node = list_entry(list_node, element_t, list);
        q_release_element(element_node);
    }

    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *element_node = malloc(sizeof(element_t));
    if (element_node == NULL)
        return false;
    element_node->value = strdup(s);
    if (!element_node->value) {
        free(element_node);
        return false;
    }
    struct list_head *list_node = &element_node->list;
    list_add(list_node, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *element_node = malloc(sizeof(element_t));
    if (element_node == NULL)
        return false;
    element_node->value = strdup(s);
    if (!element_node->value) {
        free(element_node);
        return false;
    }
    struct list_head *list_node = &element_node->list;
    list_add_tail(list_node, head);
    return true;
}
/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head) || !sp)
        return NULL;

    struct list_head *list_node = head->next;
    element_t *element_node = list_entry(list_node, element_t, list);
    strncpy(sp, element_node->value, bufsize);
    list_del(list_node);
    sp[bufsize - 1] = '\0';
    return element_node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *list_node = head->prev;
    element_t *element_node = list_entry(list_node, element_t, list);
    strncpy(sp, element_node->value, bufsize);
    sp[bufsize - 1] = '\0';
    list_del(list_node);
    return element_node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    size_t length = 0;
    struct list_head *node;
    list_for_each (node, head) {
        length++;
    }
    return length;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;
    struct list_head *slow = head->next;
    struct list_head *fast = head->next;
    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    list_del_init(slow);
    element_t *element_node = list_entry(slow, element_t, list);
    q_release_element(element_node);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    struct list_head *cur = head->next;
    struct list_head *next = NULL;
    char pre_val[1024] = {0};
    char cur_val[1024] = {0};
    char next_val[1024] = {0};

    while (cur != head) {
        next = cur->next;
        element_t *element_node_cur = list_entry(cur, element_t, list);
        strncpy(cur_val, element_node_cur->value,
                strlen(element_node_cur->value));

        if (next != head) {
            element_t *element_node_next = list_entry(next, element_t, list);
            strncpy(next_val, element_node_next->value,
                    strlen(element_node_next->value));
        } else {
            next_val[0] = '\0';
        }

        if (strncmp(pre_val, cur_val, strlen(cur_val)) == 0 ||
            strncmp(cur_val, next_val, strlen(cur_val)) == 0) {
            list_del_init(cur);
            element_t *element_node_del = list_entry(cur, element_t, list);
            q_release_element(element_node_del);
        }

        strncpy(pre_val, cur_val, strlen(cur_val));
        cur = next;
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *pre = head;
    struct list_head *cur = head->next;
    struct list_head *next = cur->next;

    while (cur != head && next != head) {
        list_del_init(next);
        list_add(next, pre);

        pre = cur;
        cur = cur->next;
        next = cur->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *cur = head->next;
    struct list_head *next = NULL;
    while (cur != head) {
        next = cur->next;
        list_del_init(cur);
        list_add(cur, head);
        cur = next;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head))
        return;
    struct list_head *cur = head->next;
    struct list_head *next = NULL;
    int count = 0;
    while (cur != head && count < k) {
        next = cur->next;
        list_del_init(cur);
        list_add(cur, head);
        cur = next;
        count++;
    }
}

/* Sort elements of queue in ascending/descending order */
/*need to break list1 and list2 from circular list to oneway list first*/
struct list_head *mergeTwoLists(struct list_head *list1,
                                struct list_head *list2,
                                struct list_head **tail,
                                bool descend)
{
    struct list_head *ret = NULL;
    struct list_head *prev = NULL;
    struct list_head **ptr = &ret;
    for (; list1 != NULL && list2 != NULL; ptr = &(*ptr)->next) {
        element_t *element_node1 = list_entry(list1, element_t, list);
        element_t *element_node2 = list_entry(list2, element_t, list);

        int compare_result = strcmp(element_node1->value, element_node2->value);
        if (descend)
            compare_result *= -1;

        if (compare_result < 0) {
            *ptr = list1;
            list1->prev = prev;
            prev = list1;
            list1 = list1->next;
        } else {
            *ptr = list2;
            list2->prev = prev;
            prev = list2;
            list2 = list2->next;
        }
    }

    if (list1 == NULL) {
        *ptr = list2;
        list2->prev = prev;
        *tail = list2;
    } else {
        *ptr = list1;
        list1->prev = prev;
        *tail = list1;
    }
    while ((*tail)->next != NULL)
        *tail = (*tail)->next;
    return ret;
}

struct list_head *sortList(struct list_head *node,
                           struct list_head **tail,
                           bool descend)
{
    if (node == NULL || node->next == NULL)
        return node;
    struct list_head *slow = node;
    struct list_head *fast = node->next;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    struct list_head *list1 = sortList(slow->next, tail, descend);
    slow->next = NULL;
    struct list_head *list2 = sortList(node, tail, descend);

    return mergeTwoLists(list1, list2, tail, descend);
}
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    head->prev->next = NULL;  // break the circular list at first

    struct list_head *tail = NULL;
    struct list_head *ret = sortList(head->next, &tail, descend);
    head->next = ret;
    ret->prev = head;
    head->prev = tail;
    tail->next = head;
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return q_size(head);
    char max_string[1024] = {0};
    struct list_head *cur = head->next;
    struct list_head *next = NULL;

    while (cur != head) {
        next = cur->next;
        element_t *element_node_cur = list_entry(cur, element_t, list);
        if (strcmp(max_string, element_node_cur->value) <=
            0) /*max_string is less or equal than cur string*/
        {
            strncpy(max_string, element_node_cur->value,
                    strlen(element_node_cur->value));
        } else /*max_string is greater then the cur string*/
        {
            list_del_init(cur);
            q_release_element(element_node_cur);
        }
        cur = next;
    }


    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return q_size(head);
    char max_string[1024] = {0};
    struct list_head *cur = head->prev;
    struct list_head *next = NULL;

    while (cur != head) {
        next = cur->prev;
        element_t *element_node_cur = list_entry(cur, element_t, list);
        if (strcmp(max_string, element_node_cur->value) <=
            0) /*max_string is less or equal than cur string*/
        {
            strncpy(max_string, element_node_cur->value,
                    strlen(element_node_cur->value));
        } else /*max_string is greater then the cur string*/
        {
            list_del_init(cur);
            q_release_element(element_node_cur);
        }
        cur = next;
    }


    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    if (!head || list_empty(head))
        return 0;

    return 0;
    /** struct list_head *forward = head->next; */
    /** struct list_head *backward = head->prev; */
    /** while(head->next != head->prev) */
    /** { */
    /**     while(forward!=backward && forward!=head && backward!=head) */
    /**     { */
    /**         queue_contex_t *forward_head = list_entry(forward,
     * queue_contex_t, chain); */
    /**         queue_contex_t *backward_head = list_entry(backward,
     * queue_contex_t, chain); */
    /**         struct list_head *head1 = forward_head->q; */
    /**         struct list_head *head2 = backward_head->q; */
    /**         struct list_head *list1 = head1->next; */
    /**         struct list_head *list2 = head2->next; */
    /**         struct list_head *merge = mergeTwoLists(list1, list2, head1,
     * head2, descend);  */
    /**         forward_head->q->next = merge; */
    /**         merge->prev = forward_head->q; */
    /**  */
    /**         struct list_head *del = backward; */
    /**  */
    /**         backward = backward->prev; */
    /**         forward = forward->next; */
    /**  */
    /**         list_del_init(del); */
    /**     } */
    /**  */
    /** } */
    /**  */
    /** struct list_head *first = head->next; */
    /** queue_contex_t *first_head = list_entry(first, queue_contex_t, chain);
     */
    /**  */
    /** return q_size(first_head->q); */
}
