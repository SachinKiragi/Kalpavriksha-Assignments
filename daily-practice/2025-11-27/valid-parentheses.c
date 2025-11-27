typedef struct Stack Stack;


typedef struct Stack{
    char arr[10000+2];
    int top;
    void (*push)(char, Stack*);
    bool (*size)(Stack*);
    void (*pop)(Stack*);
    char (*peek)(Stack*);
} Stack;

void push(char c, Stack* s){
    s->arr[s->top] = c;
    s->top++;
}

bool size(Stack* s){
    return s->top;
}

void pop(Stack* s){
    s->arr[s->top-1];
    s->top--;
    return;
}

char peek(Stack *s){
    return s->arr[s->top-1];
}

bool isMatching(char a, char b){
    if(a == ')') return b == '(';
    else if(a == ']') return b == '[';
    else if(a == '}') return b == '{';
    else return 0;
}

bool isValid(char* s) {
    Stack* st = (Stack*) calloc (1, sizeof(Stack));
    st->push = &push;
    st->pop = &pop;
    st->peek = &peek;
    st->size = &size;
    int n = strlen(s);
    int i = 0;

    while(i<n){
        if(st->size(st) && isMatching(s[i], st->peek(st))){
            st->pop(st);
        } else{
            st->push(s[i], st);
        }
        i++;
    }

    return st->size(st) == 0;
}