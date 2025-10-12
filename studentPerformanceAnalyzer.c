#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define NUMBER_OF_SUBJECTS 3
#define MAX_NAME_LENGTH 100

const float GRADE_A = 85.0;
const float GRADE_B = 70.0;
const float GRADE_C = 50.0;
const float GRADE_D = 35.0;

typedef struct {
    int rollNumber;
    char name[MAX_NAME_LENGTH];
    int marks[NUMBER_OF_SUBJECTS];
} studentInfo;


// Function To Take Information For A Single Student
void getCurrentStudentInfo(studentInfo *currentStudent){
    scanf("%d", &currentStudent->rollNumber);
    scanf("%s", currentStudent->name);
    scanf("%d %d %d", &currentStudent->marks[0], &currentStudent->marks[1], &currentStudent->marks[2]);
}


int getTotalMarks(int marks[NUMBER_OF_SUBJECTS]){
    int totalMarks = 0;

    for(int i=0; i<NUMBER_OF_SUBJECTS; i++){
        totalMarks += marks[i];
    }

    return totalMarks;
}


float getAverageMarks(int marks[NUMBER_OF_SUBJECTS]){
    int totalMarks = getTotalMarks(marks);
    float avgMarks = (float)(totalMarks) / NUMBER_OF_SUBJECTS;
    return avgMarks;
}


char getGrade(float averageMarks){
    if(averageMarks >= GRADE_A){
        return 'A';
    } else if(averageMarks >= GRADE_B){
        return 'B';
    } else if(averageMarks >= GRADE_C){
        return 'C';
    } else if(averageMarks >= GRADE_D){
        return 'D';
    } else{
        return 'F';
    }
}


void displayPerformancePattern(char grade){

    int stars = 0;
    switch (grade)
    {
        case 'A': stars = 5;
            break;
        case 'B': stars = 4;
            break;
        case 'C': stars = 3;
            break;
        case 'D': stars = 2;
            break;
        default:
            break;
    }

    printf("Performance: ");
    for(int i=1; i<=stars; i++){
        printf("*");
    }
    printf("\n");

}


void displayStudentsInfo(studentInfo *students, int noOfStudents){
    printf("\n");

    for(int i=0; i<noOfStudents; i++){
        
        int totalMarks = getTotalMarks(students[i].marks);
        float averageMarks = getAverageMarks(students[i].marks);
        char grade = getGrade(averageMarks);
        
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
    
    for(int i=0; i<noOfStudents; i++){
        getCurrentStudentInfo(&students[i]);
    }

    displayStudentsInfo(students, noOfStudents);
    displayRollNumbers(students, noOfStudents);
}


bool isNoOfStudentsValid(int noOfStudents){
    return (noOfStudents > 0 && noOfStudents <= 100);
}


int main(){

    int noOfStudents;

    do{
        scanf("%d", &noOfStudents);
        if(!isNoOfStudentsValid(noOfStudents)){
            printf("Invalid No Of Students Range Should Be [1-100]: \n");
        }
    } while(!isNoOfStudentsValid(noOfStudents));

    studentInfo *students = (studentInfo*)malloc(noOfStudents * sizeof(studentInfo));

    analyzeStudentPerformance(students, noOfStudents);

    free(students);

    students = NULL;

    return 0;

}
