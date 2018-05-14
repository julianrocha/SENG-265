//Julian Rocha, V00870460
#include "linkedlist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

const int OUTPUT_SIZE = 70; //Size of output line
const int CHAR_BEFORE = 20; //Max # char's to print before key
const int CHAR_AFTER = 31;  //Max # char's to print after key


//Prints a word in a wordnode, formatted by arg
void printword(wordnode_t *n, void *arg) {
    char *format = (char *)arg;
    printf(format, n->w);
}//end of printword

//Read version 2 input file from stdin and collect exclusion words/lines
void readInput(wordnode_t **ex, wordnode_t **li){
    size_t len = 0;             //store length when using getline
    char *buffer = NULL;        //string to temporarily store a line
    int counter = 0;            //counter for exlusions vs lines
    int line_num = 0;           //keep track of line numbers
    while(getline(&buffer, &len, stdin) != -1){
        buffer[strlen(buffer) - 1] = '\0';                      //strip away '\n'
        if(strcmp(buffer, "::") == 0) counter++;                //update counter
        else if(counter == 0 || buffer[0] == '\0') continue;    //skip empty lines/first line
        else if(counter == 1) *ex = add_end(*ex, new_word(buffer, 0, 0));  
                                                                // ^ append exclusion word
        else if(counter == 2){
            *li = add_end(*li, new_word(buffer, line_num, 0));  //append line
            line_num++;
        }
    }//end of input lines
}//end of readInput

//Returns a linked list of keywords extracted from lines
void grabKeywords(wordnode_t **key, wordnode_t *ex, wordnode_t *li){
    wordnode_t *curr;   //to iterate through lines
    int col_counter;    //to keep track of location in a line
    int cur_wordlen;    //to add to col_counter after every word

    for (curr = li; curr != NULL; curr = curr->next){
    
        char *word = (char*)emalloc((strlen(curr->w) + 1) * sizeof(char));
        strcpy(word, curr->w);  //make copy of line
        strtok(word, " ");      //tokenize line into words
        col_counter = 0;

        while(word != NULL){    //check every tokenized word
            
            if(is_in(ex, word) == 0){   //if word is not an exclusion
                *key = add_sorted(*key, new_word(word, curr->lin, col_counter));
            }
            cur_wordlen = strlen(word); 
            word = strtok(NULL, " ");       //move on to next word in line
            col_counter += cur_wordlen + 1; //update column location
        }
    }
}//end of grabKeywords

//Returns a linked list of formatted output lines
void constructOutput(wordnode_t **out, wordnode_t *key, wordnode_t *li){
   
    wordnode_t *curr;   //to iterate through keywords
 
    for(curr = key; curr != NULL; curr = curr->next){
        int i;          //variable for the for loops

        //**********KEYWORD DATA**********
        char *word = (char*)emalloc((strlen(curr->w)) * sizeof(char));
        strcpy(word, curr->w);      //make copy of keyword
        int word_len = strlen(word);//length of keyword
        int key_start = curr->col;  //location in line of keyword's first letter
        //**********KEYWORD DATA**********

        //**********LINE DATA**********
        char *line = (char*)emalloc((strlen(get_node(li, curr->lin)->w)) * sizeof(char));
        strcpy(line, get_node(li, curr->lin)->w); //make copy of line
        int line_len = strlen(line);              //length of line
        //**********LINE DATA**********

        char output[OUTPUT_SIZE];   //empty output line

        //**********LEFT FORMATTING**********
        int left_start = 0;             //location in line to begin formatted output
        if(key_start > CHAR_BEFORE){    //if keyword starts too late in the line
            left_start = key_start - CHAR_BEFORE;
            for(i = left_start; i < key_start; i++){
                if(*(line + i - 1) != ' ') left_start++; //shift start right if word is cut off
                else break;
            }
        }
        int left_len = key_start - left_start;  //length of left portion (text only)
        int num_spaces = CHAR_BEFORE - left_len;//number of spaces to add 
        for(i = 0; i < num_spaces; i++){
            output[i] = ' ';                    //add the spaces
        }
        output[i] = '\0';                       //add the NULL terminator for strncat
        strncat(output, line + left_start, left_len);   //add the left portion of text
        //*********LEFT FORMATTING**********

        //**********KEYWORD FORMATTING**********
        for(i = 0; i < word_len; i++){          //add capatalized keyword
            output[CHAR_BEFORE + i] = toupper(word[i]);
        }
        output[CHAR_BEFORE + i] = '\0';
        //**********KEYWORD FORMATTING**********

        //**********RIGHT FORMATTING**********
        int right_start = key_start + word_len; //location in line to begin right side format
        int right_end = line_len;               //location in line to end the output line
        if(right_end - key_start > CHAR_AFTER){ //if right portion is too long
            right_end = key_start + CHAR_AFTER;
            for(i = right_end; i > key_start + word_len; i--){
                if(*(line + i) != ' ') {        //shift end left if word is cut off
                    right_end--;
                }
                else {
                    break;
                }
            }
        }
        int right_len = right_end - right_start;//length of right portion (text only)
        strncat(output, line + right_start, right_len); //add the right portion of text
        //**********RIGHT FORMATTING**********
        
        *out = add_end(*out, new_word(output,0,0));
        // ^ add output line to linked list
    }
}//end of constructOutput


int main(int argc, char *argv[]) {
  
    wordnode_t *ex = NULL;      //list storing exlusion words
    wordnode_t *li = NULL;      //list storing lines
    wordnode_t *key = NULL;     //list storing keywords
    wordnode_t *output = NULL;  //list storing formatted output lines

    readInput(&ex, &li);
    grabKeywords(&key, ex, li);
    constructOutput(&output, key, li);
    apply(output, printword, "         %s\n");  //send output to stdout

    ex = NULL;
    li = NULL;
    key = NULL;
    output = NULL;

    //Debugging
    wordnode_t *temp;
    #ifdef DEBUG
    while (ex) {
        temp = peek_front(ex);
        ex = remove_front(ex);
        free_word(temp);
    } 
    while (li) {
        temp = peek_front(li);
        li = remove_front(li);
        free_word(temp);
    } 
    #endif
    exit(0);
}//end of main