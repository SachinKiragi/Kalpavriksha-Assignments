#include<stdio.h>
#include<stdbool.h>

#define MAX_LEN 10000

const int errorCodeForValue = -1e9; // Returned As Error Indicator In Peek And Pop If Values Stack Is Empty
const char errorCodeForOperator = '.'; // Returned As Error Indicator In Peek And Pop If Operator Stack Is Empty

typedef enum{
    STACK_OVERFLOW,
    STACK_UNDERFLOW,
    DIVISION_BY_ZERO,
    INVALID_EXPRESSION,
} Error;


const char* getErrorMessage(Error err) {
    switch (err) {
        case STACK_OVERFLOW: return "Error: Stack Overflow";
        case STACK_UNDERFLOW: return "Error: Stack Underflow";
        case DIVISION_BY_ZERO: return "Error: Division by Zero";
        case INVALID_EXPRESSION: return "Error: Invalid Expression";
        default: return "UnExpected Error Occured";
    }
}

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


// Pushes Number Into Value Stack, If Stack Is Empty It Retunrs False Else Return True
bool pushValue(valueStack *values, int currValue){

    if(values->top+1 == MAX_LEN){
        printf(getErrorMessage(STACK_OVERFLOW));
        return false;
    } else{
        values->data[++values->top] = currValue;
        return true;
    }

}

// returns The Top Element From Value Stack -> If Stack Is Empty -> Returns errorCodeForValue
int peekValue(valueStack *values){
    int topValue = errorCodeForValue;

    if(values->top == -1) {
        printf("\n%s\n\n", getErrorMessage(STACK_UNDERFLOW));
    } else{
        topValue = values->data[values->top];
    }

    return topValue;
}

// Remove Top Elemenet From Value Stack And returns The Top Element Of Value Stack -> If Stack Is Empty -> Returns errorCodeForValue
int popValue(valueStack *values){
    int poppedValue = errorCodeForValue;

    if(values->top == -1){
        printf("\n%s\n\n", getErrorMessage(STACK_UNDERFLOW));
    } else{
        poppedValue = peekValue(values);
        --values->top;
    }

    return poppedValue;
}


bool pushOperator(operatorStack *operators, char currOperator){

    if(operators->top+1 == MAX_LEN){
        printf("\n%s\n\n", getErrorMessage(STACK_OVERFLOW));
        return false;
    } else{
        operators->data[++operators->top] = currOperator;
        return true;
    }

}

char peekOperator(operatorStack *operators){
    char topOperator = errorCodeForOperator;

    if(operators->top == -1) {
        printf("\n%s\n\n", getErrorMessage(STACK_UNDERFLOW));
    } else{
        topOperator = operators->data[operators->top];
    }

    return topOperator;
}

char popOperator(operatorStack *operators){
    char poppedOperator =  errorCodeForOperator;

    if(operators->top == -1){
        printf("\n%s\n\n", getErrorMessage(STACK_UNDERFLOW));
    } else{
        poppedOperator = peekOperator(operators);
        --operators->top;
    }

    return poppedOperator;
}

// Return The Size Of Stack Based On Flag -> If Flag = 'v' Then Returns valuesStack Size Else returns OperatorStack Size
int getSizeOfStack(void *stack, char flag){ 
    if(flag == 'v'){
        return ((valueStack*)(stack))->top+1;
    } else{
        return ((operatorStack*)(stack))->top+1;
    }
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

//Function To Convert a Number From String To Int, If Some Error Occurrs It Return errorCodeForValue
int getCurrentNumber(char *expression, int* i, int len){
    int currNumber = 0;

    while((*i) < len && (isDigit(expression[*i]) || isSpace(expression[*i]))){ // isSpace Handles Test Cases Like -> 12 4+1
        if(isSpace(expression[*i]) == false){ // If Current Char Is Space It Ignores
            currNumber = currNumber * 10 + (expression[*i]-'0');
        }
        (*i)++;
    }

    if((*i) < len && !isDigit(expression[*i]) && !isOperator(expression[*i]) && !isSpace(expression[*i])){
        printf("\n%s\n\n", getErrorMessage(INVALID_EXPRESSION)); // True For (eg: 2e3+4)
        currNumber = errorCodeForOperator;
    }
    return currNumber;
}


int getOperand(valueStack *values){
    int currOperand = 0;
    currOperand = peekValue(values);
    if(currOperand != errorCodeForValue){
        popValue(values);
    }
    return currOperand;
}

char getOperator(operatorStack *operators){
    char currOperator;

    currOperator = peekOperator(operators);
    if(currOperator != errorCodeForOperator) {
        popOperator(operators);
    }

    return currOperator;
}

//Computes The Operation Based On Top 2 Oprands With Respect To Top Operator, If Any Error Occurs It Return errorCodeForValue
int evaluateTopOperator(valueStack *values, operatorStack *operators){
    int right = 0;
    int left = 0;
    char op = '.';
    int result = -1;

    right = getOperand(values);

    if(right != errorCodeForValue){ // Executes Only If right Operand Is Fetched Successfully
        left = getOperand(values);
    }

    if(left != errorCodeForValue){ // Executes Only If left Operand Is Fetched Successfully
        op = getOperator(operators);
    }

    if(op == errorCodeForOperator) return op; // True If Any Error Occurs While Fetching The Opeartor OR Operands
    switch(op){
        case '/': if(right == 0){
                    printf("\n%s\n\n", getErrorMessage(DIVISION_BY_ZERO));
                    result = errorCodeForValue;
                  } else{
                    result = left / right;
                  }
            break;
        case '*': result = left * right;
            break;
        case '+': result = left + right;
            break;
        case '-': result = left - right;
            break;
    }

    return result;
}


// Function To Calculate Result Of Expession, If Any Error Occurs It Returns errorCodeForValue
int calculate(char* expression) {

    int result = 0;

    int len = length(expression);
    if(len == 0){
        printf("\n%s\n\n", getErrorMessage(INVALID_EXPRESSION));
        return errorCodeForValue;
    }

    int i=0;
    
    valueStack values = {.top = -1};
    operatorStack operators = {.top = -1};

    int isNegative = 0; // Indicator For Negative Operands (eg: -2*-2, 8/-2, -5+1)       
    
    if(isOperator(expression[i]) == true){
        if(expression[i] == '+' || expression[i] == '-'){ 
            isNegative = (expression[i]=='-');
        } else{
            printf("\n%s\n\n", getErrorMessage(INVALID_EXPRESSION));
            result = errorCodeForValue; // True For (eg: *9+1, /2+3);
        } 
        i++;
    }

    while(i < len && result != errorCodeForValue){
        if(isDigit(expression[i]) == false && isOperator(expression[i]) == false && isSpace(expression[i]) == false){
            printf("\n%s\n\n", getErrorMessage(INVALID_EXPRESSION));
            result = errorCodeForValue; // Ture For (eg: 2+a*3)
            break;
        } else if(isSpace(expression[i]) == true){ 
            i++;
            continue; // Neglecting Spaces
        } else if(isDigit(expression[i]) == true){

            int currNumber = 0;
            currNumber = getCurrentNumber(expression, &i, len);

            if(currNumber == errorCodeForValue){
                result  = errorCodeForValue;
                break;
            }

            if(isNegative){
                currNumber *= -1;
                isNegative = 0;
            }

            if(pushValue(&values, currNumber) == false){ // Exeeutes If The Stack Is Full
                result = errorCodeForOperator;
                break;
            };

        } else{

            if(i > 0 && isOperator(expression[i-1]) == true){
                if(expression[i] == '/' || expression[i] == '*'){
                    printf("\n%s\n\n", getErrorMessage(INVALID_EXPRESSION)); // True For eg: 3/*4
                    result = errorCodeForValue;
                } else if(expression[i] == '-') {
                    isNegative = 1; // True For eg: 4/-2, 1-+2
                }
                i++;
                continue;
            }

            while(getSizeOfStack(&operators, 'o') >= 1 && getSizeOfStack(&values, 'v') >= 2){
                
                char topOp;
                
                topOp = peekOperator(&operators);
                if(topOp == errorCodeForOperator) break;

                if(precedence(topOp) >= precedence(expression[i])){
                    int currResult = 0;

                    currResult = evaluateTopOperator(&values, &operators); // Evalues Top Operands And Stores The Result In currResult
                    if(currResult == errorCodeForValue) break;

                    if(pushValue(&values, currResult) == false) {
                        result = errorCodeForValue;
                        break;
                    };
                } else {
                    break;
                }
            }
            
            if(result == errorCodeForValue) break; // Break If Any  Error Occurs While Parsing Number Or While Evaluating Current Top Operands
            if(pushOperator(&operators, expression[i]) == false){
                result = errorCodeForValue;
                break;
            }
            i++; 
        }
    }

    while(getSizeOfStack(&operators, 'o')>=1 && result != errorCodeForValue){
        int currResult = 0;
        currResult = evaluateTopOperator(&values, &operators);
        if(currResult != errorCodeForValue) {
            if(pushValue(&values, currResult) == false){ // If Stack Is Full
                result = errorCodeForValue;
            }
        } else{
            result = errorCodeForValue;
            break;
        }
    }

    if(result != errorCodeForValue){
        result = peekValue(&values);
    }

   return result;

}



int main(){

    printf("Claculator App\n");
    char expression[MAX_LEN];
    int result = 0;

    while(1){
        printf("Enter mathematical expression (type 'e' to exit): ");

        fgets(expression, MAX_LEN, stdin);
        expression[length(expression)-1] = 0;
        
        if(expression[0] == 'e') break; // If e(Exit) Is Given

        result = calculate(expression); // Evaluates The Expression And Stores The Resultant Values In Result Variable Else Returns Error
        if(result != errorCodeForValue){
            printf("\n'%s = %d'\n\n", expression, result); 
        }
    }

    return 0;
}