#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#define MAX_NAME 100
#define MAX_DEPARTMENT 50
#define FILENAME "C:\\teachers.csv"
#define OUTPUT_TEXT "C:\\output.txt"
#define BINARY_FILE "C:\\teachers.dat"

typedef struct {
    char name[MAX_NAME];
    char department[MAX_DEPARTMENT];
    int hours;
    double salary;
} Teacher;

void printMenu();
void swapTeachers(Teacher* a, Teacher* b);
void shakerSort(Teacher* teachers, int count);
int readTeachers(Teacher** teachers, const char* filename);
void printTeachers(Teacher* teachers, int count);
void writeTeachersToTextFile(Teacher* teachers, int count, const char* filename, const char* department);
double averageSalaryForLowHours(Teacher* teachers, int count, int maxHours);
void writeTeachersToBinary(Teacher* teachers, int count, const char* filename);
int readTeachersFromBinary(Teacher** teachers, const char* filename);
void insertTeacherToBinary(Teacher* teacher, int position, const char* filename);
void appendTeacherToBinary(Teacher* teacher, const char* filename);
void deleteTeacherFromBinary(int position, const char* filename);
void searchInBinaryFile(const char* filename, int maxHours);
