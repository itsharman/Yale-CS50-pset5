/**
 * dictionary.c 
 *
 * Harmanpreet Singh
 * CPSC 100
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

//global variables
#define HASH_SIZE 143093 //size of hash table (lines in large dictionary)              
int dict_size = 0; //size of dictionary
int hash_val = 0; //value of hash function
int alpha_apos = 0; //will be used in hash function

//prototype of hash function
int hash_fxn (const char* word);    

//create linked list w/ nodes
typedef struct node {               
    char word [LENGTH + 1];
    struct node *next;
} node;

//create a hash table of size HASH_SIZE (words in large dictionary)
node *hash_tab [HASH_SIZE];  

//returns true if word is in dictionary else false. Tested w/ Alice.txt
bool check(const char* word) {
    //create 'key_word' array to store current word
    char key_word [LENGTH + 1];             
    //position of word
    int pos = hash_fxn(key_word);       
    //create cursor
    node *cursor = hash_tab [pos];  
    //lower-case all letters in word
    for (int i = 0, n = strlen (word); i < n; i++) key_word[i] = tolower (word[i]); 
    //set last character of key_word to '\0'
    key_word [strlen (word)] = '\0';
    //return false if hash table empty at position
    if (hash_tab [pos] == NULL) return false;
    //compare words if not empty at position
    while (cursor != NULL) {           
        printf ("comparing %s against %s\n", key_word, cursor -> word);
        if (strcmp (key_word, cursor -> word) == 0) return true;
        cursor = cursor -> next;
    } 
    //return false if no word found
    return false;                    
}

//loads dictionary into memory. Returns true if successful else false. Tested w/ large.
bool load(const char* dictionary) {
    //initializes all values in hash table to NULL
    for (int i = 0; i < HASH_SIZE; i++) hash_tab[i] = NULL;
    //create 'key_word' array to store current word
    char key_word [LENGTH + 1]; 
    //opens the dictionary file in read-mode
    FILE *fp = fopen(dictionary, "r");
    //return false if dictionary file does not exist
    if (fp == NULL) return false;  
    //load every word into hash table
    while (fscanf(fp, "%s\n", key_word) != EOF) {   
        //add 1 to size of dictionary
        dict_size++;                
        //allocate memory for next word 
        node *next_word = malloc(sizeof(node)); 
        //copy word into new node
        strcpy (next_word -> word, key_word);
        //positions word
        int pos = hash_fxn(key_word);
        //insert new word if current position is empty
        if (hash_tab[pos] == NULL) {
            //put next word into empty position
            hash_tab[pos] = next_word;
            //point to new position
            next_word -> next = NULL;
        }
        //link if current position is not empty
        else {                            
            //link nodes together
            next_word -> next = hash_tab[pos];
            hash_tab[pos] = next_word;
        }      
    } 
    //close dictionary file
    fclose(fp);                              
    return true;
}

//create hash function for hash table
int hash_fxn (const char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        //check if character is from alphabet
        if (isalpha(word[i])) alpha_apos = word[i] + 1 - 'a';
        //check if apostrophe
        else alpha_apos = 27;
        hash_val = (alpha_apos + (hash_val << 3)) % HASH_SIZE;
    } return hash_val;    
}

//returns number of words in dictionary if loaded else 0 if not yet loaded.
unsigned int size(void) {
    return dict_size;
}

//unloads dictionary from memory. Returns true if successful else false.
bool unload(void) {
    int pos = 0;
    //iterate through every character in hash table
    while (pos < HASH_SIZE) {
        //if position is empty, move on to next
        if (hash_tab[pos] == NULL) pos++;
        //iterate through nodes if not empty
        else while (hash_tab[pos] != NULL) {
                node *cursor = hash_tab[pos];
                hash_tab[pos] = cursor -> next;
                //free every node
                free(cursor);
            } pos++;
    } return true;
}

////Extra useful code from Zamayla
// node* node1 = malloc(sizeof(node));
    // node* node2 = malloc(sizeof(node));
    // node1->word = "Hello";
    // node2->word = "World";
    // //links/points node1 to node2 
    // node1->next = node2;
    // //preserves all of the links
    // new_node->next = head;
    // head = new_node;
    // //traversing linked lists
    // node* cursor = head;
    // while (cursor != NULL) {
    //     //do something
    //     cursor = cursor->next;
    // }
    // //reads word-for-word in dictionary file
    // fscanf(file, "%s", new_node->word);