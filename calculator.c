#include<stdio.h>
#include<stdbool.h>

#define MAX_LEN 10000

// Since We cannot use -1 to signal an empty stack because -1 is a valid operand.
// Therefore, PeekValue (and others) return an Error enum to indicate stack errors.
typedef enum{
    STACK_OVERFLOW,
    STACK_UNDERFLOW,
    DIVISION_BY_ZERO,
    INVALID_EXPRESSION,
    SUCCESS
} Error;


// Stack Structure To Store Operands
typedef struct{
    int data[MAX_LEN];
    int top;
} valueStack;

// Stack Structure To Store Operators
typedef struct{
    char data[MAX_LEN];
    int top;
} operatorStack; 


Error pushValue(valueStack *values, int currValue){
    Error err = SUCCESS;
    if(values->top+1 == MAX_LEN){
        err = STACK_OVERFLOW;
    } else{
        values->data[++values->top] = currValue;
    }
    return err;
}


Error popValue(valueStack *values){
    Error err = SUCCESS;
    if(values->top == -1){
        err =  STACK_UNDERFLOW;
    } else{
        --values->top;
    }
    return err;
}

Error peekValue(valueStack *values, int *topVal){
    Error err = SUCCESS;
    if(values->top == -1) {
        err = STACK_UNDERFLOW;
    } else{
        *topVal = values->data[values->top];
    }
    return err;
}

int getSizeOfValueStack(valueStack *values){
    return values->top+1;
}


Error pushOperator(operatorStack *operators, char currOperator){
    Error err = SUCCESS;
    if(operators->top+1 == MAX_LEN){
        err = STACK_OVERFLOW;
    } else{
        operators->data[++operators->top] = currOperator;
    }
    return err;
}


Error popOperator(operatorStack *operators){
    Error err = SUCCESS;
    if(operators->top == -1){
        err = STACK_UNDERFLOW;
    } else{
        --operators->top;
    }
    return err;
}

Error peekOperator(operatorStack *operators, char *topOperator){
    Error err = SUCCESS;
    if(operators->top == -1) {
        err = STACK_UNDERFLOW;
    } else{
        *topOperator = operators->data[operators->top];
    }
    return err;
}

int getSizeOfOperatorStack(operatorStack *operators){
    return operators->top+1;
}


int length(char *s){
    int len = 0;
    while(s[len] != '\0'){
        len++;
    }
    return len;
}


bool isSpace(char c){
    return c == ' ';
}

bool isDigit(char c){
    return (c >= '0' && c <= '9');
}

bool isOperator(char c){
    return (c == '+' || c == '-' || c == '/' || c == '*');
}

int precedence(char op){
    if(op == '/' || op == '*'){
        return 2;
    } else{
        return 1;
    } 
}

//Function To Convert a Number From String To Int
Error getCurrentNumber(char *expression, int* i, int len, int *currNumber){

    Error err = SUCCESS;

    while((*i) < len && (isDigit(expression[*i]) || isSpace(expression[*i]))){ // isSpace Handles Test Cases Like -> 12 4+1
        if(!isSpace(expression[*i])){ // If Current Char Is Space It Ignores
            *currNumber = *currNumber * 10 + (expression[*i]-'0');
        }
        (*i)++;
    }

    if((*i) < len && !isDigit(expression[*i]) && !isOperator(expression[*i]) && !isSpace(expression[*i])){
        err = INVALID_EXPRESSION; // True For (eg: 2e3+4)
    } else{
        err = SUCCESS;
    }

    return err;
}


Error getOperand(valueStack *values, int *operand){
    Error err = SUCCESS;
    err = peekValue(values, operand);
    if(err == SUCCESS){
        err = popValue(values);
    }
    return err;
}

Error getOperator(operatorStack *operators, char *op){
    Error err = SUCCESS;
    err = peekOperator(operators, op);
    if(err == SUCCESS) {
        err = popOperator(operators);
    }
    return err;
}

//Computes The Operation Based On Top 2 Oprands With Respect To Top Operator
Error evaluateTopOperator(valueStack *values, operatorStack *operators, int *result){

    int right = 0;
    int left = 0;
    char op = '.';

    Error err = SUCCESS;

    err = getOperand(values, &right);

    if(err == SUCCESS){ // Extectutes Only If getRightOperand Returns SUCCESS
        err = getOperand(values, &left);
    }

    if(err == SUCCESS){ // Extectutes Only If getLeftOperand Returns SUCCESS
        err = getOperator(operators, &op);
    }

    if(err != SUCCESS) return err;

    switch(op){
        case '/': if(right == 0){
                    err = DIVISION_BY_ZERO;
                  } else{
                    *result = left / right;
                  }
            break;
        case '*': *result = left * right;
            break;
        case '+': *result = left + right;
            break;
        case '-': *result = left - right;
            break;
    }

    return err;
}


Error calculate(char* expression, int *result) {

    Error err = SUCCESS;
    int len = length(expression);
    if(len == 0){
        err = INVALID_EXPRESSION;
        return err;
    }

    int i=0;
    
    valueStack values = {.top = -1};
    operatorStack operators = {.top = -1};

    int isNegative = 0; // Indicator For Negative Operands (eg: -2*-2, 8/-2, -5+1)       
    
    if(isOperator(expression[i])){
        if(expression[i] == '+' || expression[i] == '-'){ 
            isNegative = (expression[i]=='-');
        } else{
            err = INVALID_EXPRESSION; // True For (eg: *9+1, /2+3);
        } 
        i++;
    }

    while(i < len && err == SUCCESS){
        if(!isDigit(expression[i]) && !isOperator(expression[i]) && !isSpace(expression[i])){
            err = INVALID_EXPRESSION; // Ture For (eg: 2+a*3)
            break;
        } else if(isSpace(expression[i])){ 
            i++;
            continue; // Neglecting Spaces
        } else if(isDigit(expression[i])){

            int currNumber = 0;
            err = getCurrentNumber(expression, &i, len, &currNumber);

            if(err != SUCCESS) break;

            if(isNegative){
                currNumber *= -1;
                isNegative = 0;
            }

            err = pushValue(&values, currNumber);
            if(err != SUCCESS) break;

        } else{

            if(i > 0 && isOperator(expression[i-1])){
                if(expression[i] == '/' || expression[i] == '*') err = INVALID_EXPRESSION;// True For eg: 3/*4
                else if(expression[i] == '-') isNegative = 1; // True For eg: 4/-2, 1-+2
                i++;
                continue;
            }

            while(getSizeOfOperatorStack(&operators) >= 1 && getSizeOfValueStack(&values) >= 2){
                char topOp;
                
                err = peekOperator(&operators, &topOp);
                if(err != SUCCESS) break;

                if(precedence(topOp) >= precedence(expression[i])){
                    int currResult = 0;

                    err = evaluateTopOperator(&values, &operators, &currResult); // Evalues Top Operands And Stores The Result In currResult
                    if(err != SUCCESS) break;

                    err = pushValue(&values, currResult);
                    if(err != SUCCESS) break;

                } else {
                    break;
                }
            }

            if(err != SUCCESS) break; // Break If Any  Error Occurs While Parsing Number Or While Evaluating Current Top Operands
            err = pushOperator(&operators, expression[i]);
            i++; 
        }
    }

    while(getSizeOfOperatorStack(&operators)>=1 && err == SUCCESS){
        int currResult = 0;
        err = evaluateTopOperator(&values, &operators, &currResult);
        if(err == SUCCESS) {
            err = pushValue(&values, currResult);
        }
    }

    if(err == SUCCESS){
        err = peekValue(&values, result);
    }

   return err;

}

void printError(Error err) {
    if(err == DIVISION_BY_ZERO){
        printf("\n'Error: Division By Zero!'\n\n"); 
    } else{
        printf("\n'Error: Invalid Expression!'\n\n"); 
    }
}

int main(){

    printf("Claculator App\n");

    char expression[MAX_LEN];
    int result = 0;
    Error err = SUCCESS;

    while(1){
        printf("Enter mathematical expression (type 'e' to exit): ");

        fgets(expression, MAX_LEN, stdin);
        expression[length(expression)-1] = 0;
        
        if(expression[0] == 'e') break; // If e(Exit) Is Given

        err = calculate(expression, &result); // Evaluates The Expression And Stores The Resultant Values In Result Variable Else Returns Error
        if(err == SUCCESS){
            printf("\n'%s = %d'\n\n", expression, result); 
        } else{
            printError(err);
        }
    }

    return 0;
}