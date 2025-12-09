// adjSizes[i] stores the number of neighbors node i has in the adjacency list.

typedef struct Node{
    int data;
    struct Node* next;
} Node;
Node* head= NULL;
Node* tail = NULL;

void push(int data){
    Node* n = (Node*) calloc (1, sizeof(Node));
    n->data = data;
    if(head == NULL){
        head = n;
        tail = n;
    } else{
        tail->next = n;
        tail  = n;
    }
}
void pop(){
    head = head->next;
}

Node* empty(){
    return head;
}

int front(){
    return head->data;
}

int* bfs(int** adj, int* adjSizes, int V) {
    // Code here
    int *ans = (int*) calloc (V, sizeof(int));
    int *vis = (int*) calloc (V, sizeof(int));

    int k= 0;
    push(0);
    vis[0] = 1;
    while(head){
        int curr = front();
        pop();
        ans[k++] = curr;
        for(int i = 0; i < adjSizes[curr]; i++){
            int ch = adj[curr][i];
            if(!vis[ch]){
                push(ch);
                vis[ch] = 1;
            }
        }
    }
    return ans;
}
