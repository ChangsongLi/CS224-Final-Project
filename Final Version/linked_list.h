/*
 * A little linked list library
 * Can store a pointer to any data item passed as a pointer.
 *
 * Date last modified: March 9, 2014
 * Author: J. Hursey
 */
#ifndef __LINKED_LIST_H_
#define __LINKED_LIST_H_

#include <stdio.h>
#include <stdlib.h>

/*
 * Linked list node (internal use only)
 * This structure should not be referenced outside of
 * linked_list.c
 */
struct ll_node_t {
    // Data stored at this node
    void * data;
    // Pointer to the next node
    struct ll_node_t *next;
};
typedef struct ll_node_t ll_node_t;

struct user_t{
    int score;
    char *name;
};
typedef struct user_t user_t;

struct block{
    short *x;
    short *y;
    int *color;
};
typedef struct block block;
/*
 * Linked list type
 */
struct linked_list_t {
    // Pointer to the sentinal 'head' node
    ll_node_t *head;

    // pointer to the next list
    struct linked_list_t *next;

    // True length of the list
    int length;
};
typedef struct linked_list_t linked_list_t;

/*
 * Create the linked list
 *
 * Parameters
 *   None
 *
 * Returns
 *   NULL on error
 *   valid linked list on success
 */
linked_list_t * linked_list_create();

/*
 * Destroy the linked list
 *
 * Parameters
 *  list: A linked list previously created by linked_list_create()
 *
 * Returns
 *  nothing.
 */
void linked_list_destroy(linked_list_t *list);

/*
 * Get the size of the list
 *
 * Parameters
 *  list: A valid linked list
 *
 * Returns
 *  Length of the list
 */
int linked_list_length(linked_list_t *list);

/*
 * Append a datum item to the end of the list
 *
 * Parameters
 *  data: Data to append to the list
 *  list: A valid linked list
 *
 * Returns
 *  0 on success
 * -1 on error
 */
int linked_list_append(void *data, linked_list_t *list);

/*
 * Get the datum item at the specified index in the list
 *
 * Parameters
 *  index: Index in the list
 *  list: A valid linked list
 *
 * Returns
 *  NULL on error
 *  datum item stored at that index on success
 */
void* linked_list_get(int index, linked_list_t *list);

/*
 * Remove the datum item at the specified index in the list
 *
 * Parameters
 *  index: Index in the list
 *  list: A valid linked list
 *
 * Returns
 *  NULL on error
 *  datum item stored at that index on success
 */
void* linked_list_remove(int index, linked_list_t *list);

/*
 * Prepend a datum item to the front of the list
 *
 * Parameters
 *  data: Data to prepend to the list
 *  list: A valid linked list
 *
 * Returns
 *  0 on success
 * -1 on error
 */
int linked_list_prepend(void *data, linked_list_t *list);

/*
 * Add at the specified index
 * Shift all nodes at and after this index down.
 *
 * Parameters
 *  index: Index in the list
 *  data: Data to add to the list
 *  list: A valid linked list
 *
 * Returns
 *  0 on success
 * -1 on error
 */
int linked_list_add_at(int index, void *data, linked_list_t *list);

/*
 * Set the datum item at the specified index in the list
 *
 * Parameters
 *  index: Index in the list
 *  data: Data to add to the list
 *  list: A valid linked list
 *
 * Returns
 *  NULL on error
 *  datum item previously stored at that index on success
 */
void* linked_list_set(int index, void *data, linked_list_t *list);

/*
 * Sort the linked list in "ascending" order
 *
 * Parameters
 *  list: A valid linked list
 *  compar: A function pointer to compare two datum objects
 *
 * Returns
 *  nothing
 */
void linked_list_sort(linked_list_t *list,
                      int (*compar)(const void *, const void *));



#endif
