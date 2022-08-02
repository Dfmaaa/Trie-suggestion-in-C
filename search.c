//NOTE: The expression sizeof(array)/sizeof(node) must always evaluate to 26. Not more; not less, for all the node arrays.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *chars="abcdefghijklmnopqrstuvwxyz";
struct node{
    char ch;
    struct node *next[26];
};
char *my_strcat(char *cp1, char *cp2){
    int len1=strlen(cp1);
    int len2=strlen(cp2);
    char *new_str=(char *)malloc(sizeof(char)*(len1+len2+1));
    int globcount, ocount=0;
    while(cp1[ocount]!='\0'){
        new_str[globcount++]=cp1[ocount++];
    }
    ocount=0;
    while(cp2[ocount]!='\0'){
        new_str[globcount++]=cp2[ocount++];
    }
    new_str[globcount]='\0';
    return new_str;
}
void init_w_null(struct node **n, int len){
    register int counter=0;
    while(counter<len){
       *(n+counter)=NULL;
       counter++;
    }
}
int index_of_char(char ch){
    register int counter=0;
    while(*(chars+counter)!='\0'){
        if(*(chars+counter)==ch){
            return counter;
        }
        counter++;
    }
    return -1;
}
void insert(struct node **root, char *key){
    if(*root==NULL){
        *root=(struct node*)malloc(sizeof(struct node));
        if(*root==NULL){
            perror("[malloc]");
            exit(EXIT_FAILURE);
        }
        init_w_null((**root).next,26);
        struct node *selected=*root;
        (**root).ch=key[0];
        register int counter=1;
        while(counter<strlen(key)){
            int ind=index_of_char(key[counter]);
            (*selected).next[ind]=(struct node *)malloc(sizeof(struct node));
            if(selected==NULL){
                perror("[malloc]");
                exit(EXIT_FAILURE);
            }
            (*(*selected).next[ind]).ch=key[counter];
            selected=(*selected).next[ind];
            counter++;
        }
        return;
    }
    register int counter=1;
    struct node *selected=*root;
    while(counter<=strlen(key)){
        int ind=index_of_char(key[counter]);
        if((*selected).next[ind]!=NULL){
            selected=(*selected).next[ind];
            counter++;
            continue;
        }
        (*selected).next[ind]=(struct node*)malloc(sizeof(struct node));
        if((*selected).next[ind]==NULL){
            perror("[malloc]");
            exit(EXIT_FAILURE);
        }
        (*(*selected).next[ind]).ch=key[counter];
        selected=(*selected).next[ind];
        init_w_null((*selected).next,26);
        counter++;
    }
}
void find(struct node *root, char *key){
    register int counter=1;
    struct node *selected=root;
    int ind=0;
    int key_len=strlen(key);
    while(counter<=key_len){
        //if key param ends, and tree doesn't
        if(key[counter]=='\0'){
            printf("List of possible keys:\n");
            construct_str(selected,key);
            return;
        }
        ind=index_of_char(key[counter]);
        //a character of key not found.
        if((*selected).next[ind]==NULL){
            puts("Similar keys not found.");
            return;
        }
        selected=(*selected).next[ind];
        counter++;
    }
    puts("Key found.");
}
void construct_str(struct node *n, char *str){
    //end of recursion
    if(all_children_null(n)&&n!=NULL){
        printf("%s\n",str);
        return;
    }
    register int counter=0;
    while(counter<26){
        if((*n).next[counter]!=NULL){
            char nstr[2];
            nstr[0]=(*(*n).next[counter]).ch;
            nstr[1]='\0';
            char *recur_call_cp=my_strcat(str,nstr);
            construct_str((*n).next[counter],recur_call_cp);
            free(recur_call_cp);
        }
        counter++;
    }
}
void heighti(struct node *n,int f, int *m){
    if(n!=NULL){
    if(f>*m){
        *m=f;
    }
    register int counter=0;
    for(;counter<26;counter++){
        heighti((*n).next[counter],f+1,m);
    }
    }
}
int height(struct node *n){
    int h=0;
    heighti(n,1,&h);
    return h;
}
int all_children_null(struct node *n){
    register int counter=0;
    while(counter<26){
        if((*n).next[counter]!=NULL){
            return 0;
        }
        counter++;
    }
    return 1;
}
void insert_full(struct node **arr, char *key){
    int first=index_of_char(key[0]);
    insert(&arr[first],key);
}
//a debugging function to see whether insertion is successful.
/*void raw_print(struct node *n){
    //puts("[raw_print]");
    if(n!=NULL){
        putchar((*n).ch);
        register int counter=0;
        for(;counter<26;counter++){
            raw_print((*n).next[counter]);
        }
        if(all_children_null(n)){
            printf("\nAll children of %c are NULL.\n",(*n).ch);
        }
    }
}*/
int main(int argc, char **argv){
    struct node *nds[26];
    init_w_null(nds,26);
    register int counter=1;
    for(;counter<argc-1;counter++){
        insert_full(nds,argv[counter]);
    }
    char *keyword=argv[argc-1];
    struct node *keytree=nds[index_of_char(keyword[0])];
    int h=height(keytree);
    if(strlen(keyword)>h){
        printf("\nSearch key longer than every word in the trie for character '%c'.\n",keyword[0]);
        exit(EXIT_FAILURE);
    }
    find(keytree,argv[argc-1]);
    return 0;
}