#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "student.h"


bool isValidRollNumber(studentInfo *students, int currentStudentIndex){

    bool validRollNo = true;

    if(doesRollNumberExist(students, currentStudentIndex)){
        printf("\nRoll Number Exists Already!\n");
        validRollNo = false;
    } else if(students[currentStudentIndex].rollNumber <= 0){
        printf("\nRoll Number Should Be Positive Number\n");
        validRollNo = false;
    }
    return validRollNo;   

}

// Ensures Each Roll Number Is Unique By Comparing Current Student Roll Number With All It Previous Students Roll Number 
bool doesRollNumberExist(studentInfo *students, int currentStudentIndex){

    for(int i = 0; i < currentStudentIndex; i++){
        if(students[i].rollNumber == students[currentStudentIndex].rollNumber){
            return true;
        }
    }
    return false;

}

bool isMarkValid(int marks){
    return marks >= 0 && marks <= 100;
}

bool isStudentDetailsValid(studentInfo *students, int currentStudentIndex){

    if(!isValidRollNumber(students, currentStudentIndex)){
        return false;
    } 

    for(int i = 0; i < NUMBER_OF_SUBJECTS; i++){
        if(!isMarkValid(students[currentStudentIndex].marks[i])){
            printf("\nInvalid Marks Entered For Subject %d Should Be In The Range [1-100]\n", i+1);
            return false;
        }
    }

    return true;
}

bool isNoOfStudentsValid(int noOfStudents){
    return (noOfStudents > 0 && noOfStudents <= 100);
}

bool isDigit(char ch){
    return ch >= '0' && ch <= '9';
}

int getLength(char *str){
    int len = 0;
    while(str[len] != '\0'){
        len++;
    }
    return len;
}

int stringToInteger(char valueAsString[], int len){
    int i = 0;
    int valueAsInt = 0;
    while(i < len) {
        valueAsInt = valueAsInt * 10 + (valueAsString[i] - '0');
        i++;
    }
    return valueAsInt;
}


// Function To Ensure NUmber Of Students Is Valid Integer
int getValidIntegerInput(){

    char inputValueAsString[MAX_LINE_LENGTH];
    fgets(inputValueAsString, MAX_LINE_LENGTH, stdin);
    int length = getLength(inputValueAsString);
    inputValueAsString[length-1] = 0;

    int index = 0;
    while(inputValueAsString[index] != '\0'){
        if(!isDigit(inputValueAsString[index])){
            return -1;
        }
        index++;
    }
    
    int inputValueAsInt = stringToInteger(inputValueAsString, length-1);
    return inputValueAsInt;

}


// Function To Take Information For A Single Student
void getCurrentStudentInfo(studentInfo *students, int currentStudentIndex){

    bool validInfo = false;
    while(!validInfo){
        char line[MAX_LINE_LENGTH];
        fgets(line, sizeof(line), stdin);

        int count = sscanf(line, "%d %s %d %d %d", 
                           &students[currentStudentIndex].rollNumber, 
                           students[currentStudentIndex].name,
                           &students[currentStudentIndex].marks[0],
                           &students[currentStudentIndex].marks[1],
                           &students[currentStudentIndex].marks[2]);

        if(count == 5 && isStudentDetailsValid(students, currentStudentIndex)){
            validInfo = true;
        } else {
            printf("\nInvalid Details! Please enter details again.\n\n"); // True If User Enters Inavlid Number Inputs (e.g: [2 Jack 43], [3 Adam] ...)
        }
    }
}


int getTotalMarks(int marks[NUMBER_OF_SUBJECTS]){

    int totalMarks = 0;
    for(int i = 0; i < NUMBER_OF_SUBJECTS; i++){
        totalMarks += marks[i];
    }
    return totalMarks;
}


float getAverageMarks(int marks[NUMBER_OF_SUBJECTS]){
    int totalMarks = getTotalMarks(marks);
    float avgMarks = (float)(totalMarks) / NUMBER_OF_SUBJECTS;
    return avgMarks;
}


Grade getGrade(float averageMarks){

    if(averageMarks >= GRADE_A_MIN){
        return A;
    } else if(averageMarks >= GRADE_B_MIN){
        return B;
    } else if(averageMarks >= GRADE_C_MIN){
        return C;
    } else if(averageMarks >= GRADE_D_MIN){
        return D;
    } else{
        return F;
    }

}


void displayPerformancePattern(Grade grade){

    printf("Performance: ");
    switch (grade) {
        case A: printf("*****");
            break;
        case B: printf("****");
            break;
        case C: printf("***");
            break;
        case D: printf("**");
            break;
        default:
            break;
    }
    printf("\n");

}


void displayStudentsInfo(studentInfo *students, int noOfStudents){

    printf("\n");

    for(int i = 0; i < noOfStudents; i++){
        
        int totalMarks = getTotalMarks(students[i].marks);
        float averageMarks = getAverageMarks(students[i].marks);
        Grade grade = getGrade(averageMarks);
        
        printf("Roll: %d\n", students[i].rollNumber);
        printf("Name: %s\n", students[i].name);
        printf("Total: %d\n", totalMarks);
        printf("Average: %.2f\n", averageMarks);
        printf("Grade: %c\n", grade);

        if(averageMarks < 35){
            printf("\n");
            continue;
        }

        displayPerformancePattern(grade); 
        printf("\n");
    }
}


// Function To Print Roll Number Of Students Via Recursion
void displayCurrentStudentRollNumber(int index, studentInfo* students, int noOfStudents){
    if(index >= noOfStudents){
        return;
    }
    printf("%d ", students[index].rollNumber);
    displayCurrentStudentRollNumber(index+1, students, noOfStudents);
}


void displayRollNumbers(studentInfo* students, int noOfStudents){
    printf("List of Roll Numbers (via recursion): ");
    displayCurrentStudentRollNumber(0, students, noOfStudents);
    printf("\n");
}


void analyzeStudentPerformance(studentInfo* students, int noOfStudents){
    
    for(int i = 0; i < noOfStudents; i++){
        getCurrentStudentInfo(students, i);
    }
    
    displayStudentsInfo(students, noOfStudents);
    displayRollNumbers(students, noOfStudents);
    printf("\n");
}

