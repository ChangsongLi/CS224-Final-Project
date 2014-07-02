/*
 * The implementation of the little linked list library.
 *
 * Date last modified: March 9, 2014
 * Author: J. Hursey
 */
#include "linked_list.h"

linked_list_t * linked_list_create() {
    linked_list_t *list = NULL;
    ll_node_t *node = NULL;

    list = (linked_list_t*)malloc(sizeof(linked_list_t) * 1);
    if( NULL == list ) {
        return NULL;
    }

    // Create the sentinal
    node = (ll_node_t*)malloc(sizeof(ll_node_t));
    if( NULL == node ) {
        return NULL;
    }

    // Initialize the node
    node->next = NULL;
    node->data = NULL;

    // Add it to the list
    list->head = node;
    list->length = 0;

    return list;
}

void linked_list_destroy(linked_list_t *list) {

    if( NULL == list ) {
        return;
    }

    // unlink all of the nodes
    while(0 < list->length) {
        linked_list_remove(0, list);
    }

    // Free the sentinal
    free(list->head);
    list->head = NULL;

    // Free the list
    free(list);
    list = NULL;
}

int linked_list_length(linked_list_t *list) {
    if( NULL == list ) {
        return -1;
    }

    return list->length;
}

int linked_list_append(void *data, linked_list_t *list) {
    ll_node_t *node = NULL;
    ll_node_t *tail = NULL;

    if( NULL == list ) {
        return -1;
    }


    // Find the end of the list
    tail = list->head;
    while(tail->next != NULL) {
        tail = tail->next;
    }
    // 'tail' is now the last node in the list

    // Create the node
    node = (ll_node_t*)malloc(sizeof(ll_node_t));
    if( NULL == node ) {
        return -1;
    }

    // Initialize the node
    node->next = NULL;
    node->data = data;

    // Add it to the list
    tail->next = node;
    
    list->length += 1;

    return 0;
}

int linked_list_prepend(void *data, linked_list_t *list) {
    ll_node_t *node = NULL;
    
    
    if ( NULL == list ) {
        return -1;
    }
    
    node = (ll_node_t*)malloc( sizeof(ll_node_t) );
    
    if( NULL == node ){
        return -1;
    }
    
    node->data = data;
    node->next = list->head->next;
    list->head->next = node;
    
    
    list->length++;
    
    return 0;
}

int linked_list_add_at(int index, void *data, linked_list_t *list) {
    ll_node_t *node = NULL;
    ll_node_t *trace = NULL;
    int i;
    
    if ( list->length < index || index < 0 || NULL == list ) {
        return -1;
    }
    
    node = (ll_node_t*)malloc( sizeof(ll_node_t) );
    
    if ( NULL == node ) {
       return -1;
    }
    
    node->data = data;
    
    if ( index == 0 ) {
        
        node->next = list->head->next;
        list->head->next = node;
        
    }
    else {
        
        trace = list->head->next;
        
        for (i = 1; i < index ; i++ ) {
            trace = trace->next;
            
        }
        
        node->next = trace->next;
        trace->next = node;
        
    }
    
    list->length += 1;
    
    return 0;
}

void* linked_list_set(int index, void *data, linked_list_t *list) {
    void * rtn = NULL;
    ll_node_t *trace = NULL;
    int i;
    
    if ( list == NULL || index > list->length - 1 || index < 0 ) {
        return NULL;
    }
    
    trace = list->head->next;
    for ( i = 0; i < index; i++ ) {
        trace = trace->next;
    }
    
    rtn = trace->data;
    trace->data = data;
    
    return rtn;
}

void* linked_list_get(int index, linked_list_t *list) {
    ll_node_t *node = NULL;
    int i;

    if( NULL == list ) {
        return NULL;
    }

    // Check the index
    if( index >= list->length ) {
        return NULL;
    }

    // Find the n'th item
    node = list->head;
    for(i = 0; i <= index; ++i ) {
        node = node->next;
    }
 
    // Return its data
    return node->data;
}

void* linked_list_remove(int index, linked_list_t *list) {
    ll_node_t *node = NULL;
    ll_node_t *prev = NULL;
    void *data = NULL;
    int i;

    if( NULL == list ) {
        return NULL;
    }

    // Check the index
    if( index >= list->length ) {
        return NULL;
    }

    // Find the node to remove
    node = list->head;
    for(i = 0; i <= index; ++i ) {
        prev = node;
        node = node->next;
    }
 
    // Save the data, so we can return it later
    data = node->data;

    // Remove this node
    prev->next = node->next;

    list->length -= 1;

    free(node);
    node = NULL;

    return data;
}

void linked_list_sort(linked_list_t *list,
                      int (*compar)(const void *, const void *)) {
    int i, k, j;
    void *tmp = NULL;
    
    if( NULL == list ) {
        return;
    }
    
    for(i = 0; i < list->length ; ++i ) {
        k = i;
        
        for(j = i + 1; j < list->length; ++j ){
            if (compar( linked_list_get(j, list), linked_list_get(k, list)) < 0 ) {
                k = j;
            }
        }
        tmp = linked_list_set(i, linked_list_get(k,list), list);
        linked_list_set(k, tmp, list);
        
    }

}
