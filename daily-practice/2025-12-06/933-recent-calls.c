typedef struct Node{
    int t;
    struct Node* next;
} Node;

Node* head = NULL;
Node* tail = NULL;
int size = 0;

void push(int t){
    Node* n = (Node*) calloc (1, sizeof(Node));
    n->t = t;
    if(head == NULL){
        head = n;
        tail = n;
    } else{
        tail->next = n;
        tail = n;
    }
    size++;
}

void pop(){
    head = head->next;
    if(head == NULL){
        tail = NULL;
    }
    size--;
}

int front(){
    if(head == NULL){
        return 1e9;
    }
    return head->t;
}

typedef struct {
    
} RecentCounter;


RecentCounter* recentCounterCreate() {
    size = 0;
    head = NULL;
    tail = NULL;
    return NULL;
}

int recentCounterPing(RecentCounter* obj, int t) {
    while(size && front() < t - 3000){
        pop();
    }
    push(t);
    return size;
}

void recentCounterFree(RecentCounter* obj) {
    
}

/**
 * Your RecentCounter struct will be instantiated and called as such:
 * RecentCounter* obj = recentCounterCreate();
 * int param_1 = recentCounterPing(obj, t);
 
 * recentCounterFree(obj);
*/