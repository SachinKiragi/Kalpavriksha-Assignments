typedef struct Node{
    int src, dst, time;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct{
    Node* front;
    Node* rear;
} Queue;

typedef struct hashNode{
    int key;
    Node* value;
    struct hashNode* next;
} hashNode;


int mapSize = 1e6+7;
hashNode** map = NULL;

int gk(Node* value){
     int val = (value->src) % mapSize;
    val = (val * 10 + value->dst) % mapSize;
    val = (val * 100 + value->time) % mapSize;
    return val;
}

void addInMap(Node* value){
    int val = gk(value);
    hashNode* newHashNode = (hashNode*) calloc (1, sizeof(hashNode));
    newHashNode->key = val;
    newHashNode->value = value;
    if(map[val] == NULL){
        map[val] = newHashNode;
    } else {
        newHashNode->next = newHashNode;
        map[val] = newHashNode;
    }
}

void removeFromMap(Node* value){
    if(value==NULL) return;
    int val = gk(value);
    hashNode* curr = map[val];
    hashNode* prev = NULL;
    hashNode* next = NULL;
    while(curr){
        if(curr->key == val){
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    if(prev == NULL){
        map[val] = curr->next;
    } else{
        prev->next = curr->next;
    }
}

int isInMap(Node* value){
    int val = gk(value);
    hashNode* curr = map[val];
    while(curr){
        if(curr->value->src == value->src && curr->value->dst==value->dst && curr->value->time == value->time) return 1;
        curr = curr->next;
    }
    return 0;
}

Queue* q = NULL;

typedef struct {
    int capacity;
    int size;
} Router;

void addInQueue(int s, int d, int t){
    Node* newNode = (Node*) calloc (1, sizeof(Node));
    newNode->src = s;
    newNode->dst = d;
    newNode->time = t;
    if(q->front == NULL){
        q->front = newNode;
        q->rear = newNode;
    } else{
        q->rear->next = newNode;
        newNode->prev = q->rear;
        q->rear = newNode;
    }
    addInMap(newNode);
}

void removeFromQueue(){
    removeFromMap(q->front);
    q->front = q->front->next;  
}

Router* routerCreate(int memoryLimit) {
    Router* r = (Router*) calloc(1, sizeof(Router));
    q = (Queue*) calloc(1, sizeof(Queue));
    r->capacity = memoryLimit;
    map = (hashNode**) calloc (mapSize, sizeof(hashNode*));
    return r;
}

int* routerForwardPacket(Router* obj, int* retSize) {
    if(q->front == NULL){
        *retSize = 0;
        return NULL;
    }
    int *temp = (int*) malloc (3*sizeof(int));
    temp[0] = q->front->src;
    temp[1] = q->front->dst;
    temp[2] = q->front->time;
    removeFromQueue();
    obj->size--;
    *retSize = 3;
    return temp;
}


bool routerAddPacket(Router* obj, int s, int d, int t) {
    Node* newNode = (Node*) calloc (1, sizeof(Node));
    newNode->src = s;
    newNode->dst = d;
    newNode->time = t;
    if(isInMap(newNode)) return 0;
    if(obj->size == obj->capacity){
        int temp = 0;
        routerForwardPacket(obj, &temp);
    }
    obj->size++;
    addInQueue(s, d, t);
    return 1;
}


int routerGetCount(Router* obj, int d, int st, int et) {
    Node* curr = q->front;
    int cnt = 0;
    while(curr){
        if(curr->dst == d && curr->time >= st && curr->time <= et){
            cnt++;
        }
        curr = curr->next;
    }   
    return cnt;
}

void routerFree(Router* obj) {
    free(q);
    q = NULL;
}

/**
 * Your Router struct will be instantiated and called as such:
 * Router* obj = routerCreate(memoryLimit);
 * bool param_1 = routerAddPacket(obj, source, destination, timestamp);
 
 * int* param_2 = routerForwardPacket(obj, retSize);
 
 * int param_3 = routerGetCount(obj, destination, startTime, endTime);
 
 * routerFree(obj);
*/