#include<stdio.h>
#include<stdlib.h>
#include "student.h"

int main(){

    int noOfStudents;

    do{
        noOfStudents = getValidIntegerInput(); 
        if(!isNoOfStudentsValid(noOfStudents)){ // True When User Enters Any char apart From Int or If the range is not in [1-100] (eg: 102, -3, w, 3e4, xyz ...)
            printf("Invalid No Of Students Range Should Be [1-100]: \n");
        }
    } while(!isNoOfStudentsValid(noOfStudents));

    studentInfo *students = (studentInfo*)malloc(noOfStudents * sizeof(studentInfo));

    if(students == NULL){
        printf("\nError Occured While Alloacating Memory For Students Data\n\n");
        return 0;
    }

    analyzeStudentPerformance(students, noOfStudents);

    free(students);
    students = NULL;

    return 0;

}
