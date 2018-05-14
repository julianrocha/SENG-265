/*
 * linkedlist.c
 *
 * Based on the implementation approach described in "The Practice 
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"
#include <ctype.h>

//Returns the node with the passed index
wordnode_t *get_node(wordnode_t *list, int index){
    assert(index >= 0);
    wordnode_t *curr;
    for (curr = list; curr != NULL; curr = curr->next){
        if(curr->lin == index) return curr;
    }
    return NULL;
}//end of get_node

//Returns 1 if passed word is in list, 0 otherwise
int is_in(wordnode_t *list, char *word){
    assert(word != NULL);
    wordnode_t *curr;
    for (curr = list; curr != NULL; curr = curr->next){
       if(strcasecmp(curr->w, word) == 0) return 1;
    }
    return 0;
}//end of is_in

//Insert node to a list, maintain alphabetic order
wordnode_t *add_sorted(wordnode_t *list, wordnode_t *new){
    wordnode_t *curr;
    if (list == NULL) {
        new->next = NULL;
        return new;
    }
    if(strcasecmp(new->w, list->w) < 0){
        new->next = list;
        return new;
    }
    for (curr = list; curr->next != NULL; curr = curr->next){
        if(strcasecmp(new->w, curr->next->w) < 0){
            new->next = curr->next;
            curr->next = new;
            return list;
        }
    }
    curr->next = new;
    new->next = NULL;
    return list;
}//end of add_sorted


//Validated malloc for null allocation
void *emalloc(int n) {
    void *p;
    p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "malloc of %d bytes failed", n);
        exit(1);
    }
    return p;
}//end of emalloc

//Create a new word_node
wordnode_t *new_word(char *w, int lin, int col) {
    assert( w != NULL);
    wordnode_t *temp = (wordnode_t *)emalloc(sizeof(wordnode_t));
    temp->w = strdup(w);
    temp->lin = lin;
    temp->col = col;
    temp->next = NULL;
    return temp;
}//end of new_word

//Add node to front of list
wordnode_t *add_front(wordnode_t *list, wordnode_t *new) {
    new->next = list;
    return new;
}//end of add_front

//Add node to end of list
wordnode_t *add_end(wordnode_t *list, wordnode_t *new) {
    wordnode_t *curr;
    if (list == NULL) {
        new->next = NULL;
        return new;
    }
    for (curr = list; curr->next != NULL; curr = curr->next);
    curr->next = new;
    new->next = NULL;
    return list;
}//end of add_end

//Return first node
wordnode_t *peek_front(wordnode_t *list) {
    return list;
}//end of peek_front

//Return and remove first node
wordnode_t *remove_front(wordnode_t *list) {
    if (list == NULL) {
        return NULL;
    }
    return list->next;
}//end of remove_front

//Free the memory allocated to word node
void free_word(wordnode_t *word) {
    assert(word != NULL);
    if (word->w != NULL) {
        free(word->w);
    }
    free(word);
}//end of free_word

//Applies a function to every node of linked list
void apply(wordnode_t *list,
           void (*fn)(wordnode_t *list, void *),
           void *arg)
{
    for ( ; list != NULL; list = list->next) {
        (*fn)(list, arg);
    }
}//end of apply