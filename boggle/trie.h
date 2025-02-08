#ifndef TRIE_H
#define TRIE_H


#include <stdbool.h>
#include <string.h>


#define NUM_LETTER 26

typedef struct TrieNode TrieNode;

struct TrieNode {
  bool mark;
  TrieNode *next[NUM_LETTER];
};


TrieNode *Trie_NewNode();

void Trie_Insert(TrieNode *root,const char *word);

void Trie_Free(TrieNode **root);



#ifdef TRIE_IMPLEMENTATION_H


TrieNode *Trie_NewNode() {
  TrieNode *node=malloc(sizeof(*node));
  node->mark=false;
  for(int i=0;i<NUM_LETTER;i++) {
    node->next[i]=NULL;
  }
  return node;
}

void Trie_Insert(TrieNode *root,const char *word) {
  TrieNode *curr=root;
  for(int i=0;i<strlen(word);i++) {
    int index=word[i]-'a';
    if (curr->next[index]==NULL) {
      curr->next[index]=Trie_NewNode();
    }
    curr=curr->next[index];
  }
  curr->mark=true;
}

void Trie_Free(TrieNode **root) {
  if (*root == NULL) return;
  for (int i = 0; i < NUM_LETTER; i++) {
    if ((*root)->next[i] != NULL) {
      Trie_Free(&((*root)->next[i]));
    }
  }
  free(*root);
    *root = NULL;
}


#endif // TRIE_IMPLEMENTATION_H

#endif // TRIE_H


