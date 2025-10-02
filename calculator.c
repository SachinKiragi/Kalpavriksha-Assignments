#include<stdio.h>

#define MAX_LEN 10000

//errorFlag=0 -> No Error,
//errorFlag=1 ->Invalid expression,
//errorFlag=2 ->Division By Zero
int errorFlag = 0;

int length(char *s){
    int len = 0;
    while(s[len]!='\0'){
        len++;
    }
    return len;
}

void removeSpaces(char *s){
    int j=0, i=0;
    while(s[j]!='\0'){
        if(s[j]!=' '){
            s[i] = s[j];
            i++;
        }
        j++;
    }
    s[i] = '\0';
}

int isSpace(char c){
    return c==' ';
}

int isDigit(char c){
    return (c >= '0' && c <= '9');
}

int isOperator(char c){
    return (c=='+' || c=='-' || c=='/' || c=='*');
}

int precedence(char op){
    if(op=='/' || op == '*') return 2;
    else return 1;
}

//Function To Convert a Number From String To Int
int getCurrentNumber(char *expression, int* i, int len){

    int currNum = 0;

    while((*i) < len && isDigit(expression[*i])){
        currNum = currNum * 10 + (expression[*i]-'0');
        (*i)++;
    }

    return currNum;
}

//Computes The Operation Based On Top 2 Oprands With Respect To Top Operator
int evaluateTopOperator(int *values, char *ops, int *valTop, int *opsTop){

    int right = values[(*valTop)--];
    int left = values[(*valTop)--];

    char op = ops[(*opsTop)--];

    int result = 0;

    switch(op){
        case '/': if(right==0){
                    errorFlag = 2;
                    break;
                }
                result = left / right;
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

//Checks Validity Of Expression (If Expression Is Invalid -> Modifies The Error Flag)
void isValidExpression(char *expression, int len){

    if(len==0){ // True When We Just Press Enter While Giving Input Without Any Actual Expression
        errorFlag = 1;
        return;
    }
    
    for(int i=0; i<len; i++){

        if(!isDigit(expression[i]) && !isOperator(expression[i])){ // (eg: 2+a-4 or 2/[4)
            errorFlag = 1;
            return;
        }
    }
}

void calculate(char* expression) {

    removeSpaces(expression);

    int len = length(expression);
    
    isValidExpression(expression, len);

    int i=0;
    int values[MAX_LEN] = {0};
    int valTop = -1;
    char ops[MAX_LEN] = {'.'};
    int opsTop = -1;

    int isNegative = 0; // Indicator For Negative Operands (eg: -2*-2, 8/-2, -5+1)       
    
    if(isOperator(expression[i])){
        if(expression[i]=='+' || expression[i]=='-') isNegative = (expression[i]=='-');
        else errorFlag = 1;
        i++;
    }

    while(i<len && errorFlag==0){
        
        if(isDigit(expression[i])){

            int currNum = getCurrentNumber(expression, &i, len);
            if(isNegative){
                currNum *= -1;
                isNegative = 0;
            }
            values[++valTop] = currNum;
        } else{

            if(i>0 && isOperator(expression[i-1])){
                if(expression[i]=='/' || expression[i]=='*') errorFlag = 1; // True For eg: 3/*4
                else if(expression[i]=='-') isNegative = 1; // True For eg: 4/-2, 1-+2
                i++;
                continue;
            }

            while(opsTop >= 0 && valTop>=1 && precedence(ops[opsTop]) >= precedence(expression[i])){
                int result = evaluateTopOperator(values, ops, &valTop, &opsTop);
                values[++valTop] = result;
            }

            
            ops[++opsTop] = expression[i];
            
            i++; 
        }

    }

    while(opsTop>=0){
        int result = evaluateTopOperator(values, ops, &valTop, &opsTop);
        values[++valTop] = result;
    }

    if(!errorFlag)
        printf("\n%s = %d\n\n", expression, values[valTop]);

}


int main(){

    printf("Claculator App\n");

    char expression[MAX_LEN];

    while(1){

        printf("Enter mathematical expression (type 'e' to exit): ");

        fgets(expression, MAX_LEN, stdin);
        expression[length(expression)-1] = 0;
        
        if(expression[0]=='e') break; // If e(Exit) Is Given

        calculate(expression);

        if(errorFlag){
            if(errorFlag==1) printf("\nError: Invalid Expression! (%s) \n\n", expression);
            else if(errorFlag==2) printf("\nError: Division By Zero(0)! (%s)\n\n", expression);
            errorFlag = 0;
        }
    }

    return 0;
}