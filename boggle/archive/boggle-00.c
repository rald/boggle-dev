#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define TRIE_IMPLEMENTATION_H
#include "trie.h"

int main(void) {

  Node *root=newNode();
  FILE *fp=fopen("enable1.txt","rt");
  char line[256];
  int len;

  while(fgets(line,256,fp)) {
    len=strlen(line):
    line[len-1]='\0';
    if(len>=3 && len<=16) {
      addWord(root,line);
    }
  }

  return 0;
}
