#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define TRIE_IMPLEMENTATION_H
#include "trie.h"

TrieNode *root=NULL;

char *dice[16];
char board[4][4];
char flag[4][4];
char word[17];

char **words=NULL;
int nwords=0;

char line[256];
FILE *fp;
int len;

int Word_IndexOf(char **words,int nwords,const char *word) {
  for(int i=0;i<nwords;i++) {
    if(!strcmp(words[i],word)) return i;
  }
  return -1;
}

void Word_Add(char ***words,int *nwords,const char *word) {
  (*words)=realloc((*words),sizeof(**words)*((*nwords)+1));
  (*words)[(*nwords)++]=strdup(word);
}

void dfs(TrieNode *node,int x,int y,int d) {
  if(x<0 || x>3 || y<0 || y>3 || flag[y][x]) return;

  int ch=tolower(board[y][x]);
  int in=ch-'a';

  if(in<0 || in>25 || node->next[in]==NULL) return;

  flag[y][x]=true;

  word[d]=ch;
  word[d+1]=0;

  node=node->next[in];

  if(node->mark && Word_IndexOf(words,nwords,word)==-1) Word_Add(&words,&nwords,word);

  for(int j=-1;j<=1;j++)
    for(int i=-1;i<=1;i++)
      if(i || j)
        dfs(node,x+i,y+j,d+1);

  if(ch=='q') {
    d++;
    ch='u';
    in=ch-'a';
    if(node->next[in]!=NULL) {
      word[d]='u';
      word[d+1]=0;
      node=node->next[in];
      if(node->mark && Word_IndexOf(words,nwords,word)==-1) Word_Add(&words,&nwords,word);
      for(int j=-1;j<=1;j++)
        for(int i=-1;i<=1;i++)
          if(i || j)
            dfs(node,x+i,y+j,d+1);
    }
    d--;
  }

  flag[y][x]=false;
}

void Word_Search() {
  for(int y=0;y<4;y++) {
    for(int x=0;x<4;x++) {
      dfs(root,x,y,0);
    }
  }
}

void Word_Shuffle(char **words,int nwords) {
  for(int i=nwords-1;i>0;i--) {
    int j=rand()%(i+1);
    char *tmp=words[i];
    words[i]=words[j];
    words[j]=tmp;
  }
}

void Word_Free(char ***words,int *nwords) {
  for(int i=0;i<(*nwords);i++) {
    free((*words)[i]);
    (*words)[i]=NULL;
  }
  (*nwords)=0;
  free(*words);
  (*words)=NULL;
}

void Dice_Free(char *dice[],int ndice) {
  for(int i=0;i<ndice;i++) {
    free(dice[i]);
    dice[i]=NULL;
  }
}

void trim(char *str) {
    char *start = str;
    char *end;

    // Trim leading whitespace
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    // If the string is all spaces, return an empty string
    if (*start == '\0') {
        *str = '\0';
        return;
    }

    // Trim trailing whitespace
    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        *end-- = '\0';
    }

    // Move trimmed content to the beginning
    if (start != str) {
        memmove(str, start, end - start + 2);
    }
}

int cmp(const void *a,const void *b) {
  return strcmp(*(const char**)a,*(const char**)b);
}


int main(void) {

  srand(time(NULL));

  root=Trie_NewNode();
  fp=fopen("enable1.txt","rt");
  while(fgets(line,256,fp)) {
    char *p=strchr(line,'\n');
    if(p) *p=0;
    len=strlen(line);
    if(len>=3 && len<=16) {
      Trie_Insert(root,line);
    }
  }
  fclose(fp);

  fp=fopen("dice.txt","rt");
  for(int i=0;i<16;i++) {
    fgets(line,256,fp);
    char *p=strchr(line,'\n');
    if(p) *p=0;
    dice[i]=strdup(line);
  }
  fclose(fp);

  for(int i=15;i>0;i--) {
    int j=rand()%(i+1);
    char *tmp;
    tmp=dice[i];
    dice[i]=dice[j];
    dice[j]=tmp;
  }

  int k=0;
  for(int j=0;j<4;j++) {
    for(int i=0;i<4;i++) {
      board[j][i]=dice[k++][rand()%6];
    }
  }



  printf("board: ");
  for(int j=0;j<4;j++) {
    for(int i=0;i<4;i++) {
      putchar(board[j][i]);
    }
  }
  printf("\n");

  Word_Search();

  qsort(words,nwords,sizeof(*words),cmp);

  printf("words: ");
  for(int i=0;i<nwords;i++) {
    if(i!=0) printf(",");
    printf("%s",words[i]);
  }
  printf("\n");

  Word_Free(&words,&nwords);
  Trie_Free(&root);
  Dice_Free(dice,16);
  fclose(stdin);
  fclose(stdout);
  fclose(stderr);

  return 0;
}
