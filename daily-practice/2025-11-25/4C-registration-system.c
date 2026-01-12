#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_STRING_LEN 33

typedef struct node{
    char s[MAX_STRING_LEN];
    int cnt;
} node;

typedef struct hashNode{
    char key[MAX_STRING_LEN];
    node* val;
    struct hashNode* next;
} hashNode;

int size = 1e6+7;
hashNode** map = NULL;

int getIndex(char *s){
    int n = strlen(s);
    int index = 0;
    int currPow = 2;
    int prevPow = 1;
    int i = 0;
    int tempHash = 0;
    while(i < n){
        currPow = (prevPow * 2) / 2;
        tempHash = (s[i]-'a') * currPow;
        index += tempHash;
        index %= size;
        prevPow *= 2;
        prevPow %= size;
        i++;
    }
    return index;
}

node* find(int index, char* s){
    hashNode* curr = map[index];
    while(curr){
        if(strcmp(curr->key, s) == 0){
            curr->val->cnt++;
            return curr->val;
        }
        curr = curr->next;
    }
    return NULL;
}

void insert(int index, char *s){
    node* temp = (node*) calloc (1, sizeof(node));
    strcpy(temp->s, s);
    temp->cnt = 0;
    hashNode* tempHashNode = (hashNode*) calloc(1, sizeof(hashNode));
    strcpy(tempHashNode->key, s);
    tempHashNode->val = temp;
    tempHashNode->next = map[index];
    map[index] = tempHashNode;
}

void solve(char *s){
    int n = strlen(s);
    int index = getIndex(s);
    node* temp = find(index, s);
    if(temp){
        printf("%s%d\n", s, temp->cnt);
    } else{
        insert(index, s);
        printf("OK\n");
    }
}

int main(){
    map = (hashNode**) calloc (size, sizeof(hashNode*));
    char temp[MAX_STRING_LEN];
    int n;
    scanf("%d", &n);
    while(n--){
        scanf("%s", temp);
        temp[strlen(temp)] = 0;
        solve(temp);
    }
    
    return 0;
}