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
    struct list_head **pptr = &head->next;
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

        if (strncmp(pre_val, cur_val, strlen(cur_val)) != 0 &&
            strncmp(cur_val, next_val, strlen(cur_val)) != 0) {
            *pptr = cur;
            pptr = &cur->next;
        } else {
            list_del_init(cur);
            element_t *element_node_del = list_entry(cur, element_t, list);
            q_release_element(element_node_del);
        }

        strncpy(pre_val, cur_val, strlen(cur_val));
        cur = next;
    }
    *pptr = head;

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
