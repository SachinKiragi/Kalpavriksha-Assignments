#ifndef STUDENT_H
#define STUDENT_H

#include<stdbool.h>

#define NUMBER_OF_SUBJECTS 3
#define MAX_NAME_LENGTH 100
#define MAX_LINE_LENGTH 500 // Used To Get Each Student Data In A Single Row

#define GRADE_A_MIN 85.0
#define GRADE_B_MIN 70.0
#define GRADE_C_MIN 50.0
#define GRADE_D_MIN 35.0

typedef enum {
    A = 'A',
    B = 'B', 
    C = 'C',
    D = 'D',
    F = 'F'
} Grade;


typedef struct {
    int rollNumber;
    char name[MAX_NAME_LENGTH];
    int marks[NUMBER_OF_SUBJECTS];
} studentInfo;


// VALIDATION FUNCTIONS
bool isValidRollNumber(studentInfo*, int);
bool doesRollNumberExist(studentInfo*, int);
bool isMarkValid(int);
bool isStudentDetailsValid(studentInfo*, int);
bool isNoOfStudentsValid(int);
bool isDigit(char);

// HELPER FUNCTIONS
int getLength(char*);
int stringToInteger(char [], int);
int getValidIntegerInput();

// INPUT FUNCTIONS
void getCurrentStudentInfo(studentInfo*, int);

// CALCULATE FUNCTIONS
int getTotalMarks(int [NUMBER_OF_SUBJECTS]);
float getAverageMarks(int [NUMBER_OF_SUBJECTS]);
Grade getGrade(float);

// DISPLAY FUNCTION
void displayPerformancePattern(Grade);
void displayStudentsInfo(studentInfo*, int);
void displayCurrentStudentRollNumber(int, studentInfo*, int);
void displayRollNumbers(studentInfo*, int);

void analyzeStudentPerformance(studentInfo*, int);

#endif