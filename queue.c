#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));

    if (!q) /* return NULL if could not allocate space */
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */

    if (!q)
        return;

    list_ele_t *tmp;
    tmp = q->head;
    while (tmp) {
        tmp = tmp->next;
        q->head->next = NULL;
        free(q->head->value);
        free(q->head);
        q->head = tmp;
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *new;

    /* return false if the q is NULL */
    if (!q)
        return false;

    new = malloc(sizeof(list_ele_t));
    if (!new) {
        free(new);
        return false;
    }

    /* allocate space for the string */
    new->value = malloc(sizeof(char) * (strlen(s) + 1));

    /* free the space if malloc returns NULL? */
    if (!new->value) {
        free(new);
        return false;
    }

    /* copy the string to the allocated space */
    memset(new->value, '\0', strlen(s) + 1);
    strncpy(new->value, s, strlen(s));

    if (!q->head) {
        q->head = q->tail = new;
        new->next = NULL;
    } else {
        new->next = q->head;
        q->head = new;
    }

    q->size += 1;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* return false if q is NULL */
    if (!q)
        return false;

    list_ele_t *new;
    /* allocate space for the new element */
    new = malloc(sizeof(list_ele_t));

    /* return false if allocate error */
    if (!new)
        return false;

    new->value = malloc(sizeof(char) * (strlen(s) + 1));

    /* free the space and return false if error when allocating space for the
     * value */
    if (!new->value) {
        free(new);
        return false;
    }

    memset(new->value, '\0', strlen(s) + 1);
    strncpy(new->value, s, strlen(s));

    /* q->tail point to the new element and new->next point to NULL */
    new->next = NULL;
    if (!q->tail) {
        q->tail = q->head = new;
    } else {
        q->tail->next = new;
        q->tail = new;
    }

    /* increase the size of queue */
    q->size += 1;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* TODO: You need to fix up this code. */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q || !q->size)
        return false;

    if (sp) {
        size_t size = (bufsize - 1 > strlen(q->head->value))
                          ? strlen(q->head->value) + 1
                          : bufsize;

        memset(sp, '\0', size);
        strncpy(sp, q->head->value, size - 1);
    }

    list_ele_t *tmp = q->head;

    q->head = q->head->next;
    tmp->next = NULL;
    free(tmp->value);
    free(tmp);

    q->size -= 1;

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    return (!q) ? 0 : q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* test if q is NULL */
    if (!q || q->size < 2)
        return;

    list_ele_t *tmp = q->head->next, *store_head = q->head;
    q->tail->next = q->head;
    while (tmp != q->tail) {
        q->tail->next = tmp;
        tmp = tmp->next;
        q->tail->next->next = q->head;
        q->head = q->tail->next;
    }
    q->head = q->tail;
    q->tail = store_head;
    q->tail->next = NULL;

    return;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* return if q have less than 2 elements */
    if (!q || q->size < 2)
        return;

    merge_sort(&q->head);

    while (q->tail->next)
        q->tail = q->tail->next;

    return;
}

void merge_sort(list_ele_t **head)
{
    if (!(*head) || !((*head)->next))
        return;

    list_ele_t *tmp1, *tmp2;
    tmp1 = (*head)->next;
    tmp2 = *head;
    while (tmp1 && tmp1->next) {
        tmp1 = tmp1->next->next;
        tmp2 = tmp2->next;
    }
    tmp1 = tmp2->next;
    tmp2->next = NULL;
    tmp2 = *head;

    merge_sort(&tmp1);
    merge_sort(&tmp2);

    *head = NULL;
    list_ele_t **tmp = head;
    while (tmp1 && tmp2) {
        if (strcmp(tmp1->value, tmp2->value) < 0) {
            *tmp = tmp1;
            tmp1 = tmp1->next;
        } else {
            *tmp = tmp2;
            tmp2 = tmp2->next;
        }
        tmp = &((*tmp)->next);
    }

    *tmp = tmp1 ? tmp1 : tmp2;

    // printf("head: %s\n", (*head)->value);

    return;
}
