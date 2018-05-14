#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

typedef struct word wordnode_t;
struct word {
    char   *w;		//word 
    int     lin;	//line number of word
    int		col;	//column location of first letter in line
    wordnode_t *next;
};

wordnode_t *new_word(char *w, int lin, int col);
wordnode_t *add_front(wordnode_t *, wordnode_t *);
wordnode_t *add_end(wordnode_t *, wordnode_t *);
wordnode_t *peek_front(wordnode_t *);
wordnode_t *remove_front(wordnode_t *);
void        free_word(wordnode_t *);

int			is_in(wordnode_t *list, char *word);
wordnode_t *get_node(wordnode_t *list, int index);
wordnode_t *add_sorted(wordnode_t *list, wordnode_t *new);
void 		*emalloc(int n);

void        apply(wordnode_t *, 
                void(*fn)(wordnode_t *, void *), void *arg);
#endif
