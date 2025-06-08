#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "Funcs.h"
#define MAX_NAME 100
#define MAX_DEPARTMENT 50
#define FILENAME "C:\\teachers.csv"
#define OUTPUT_TEXT "C:\\output.txt"
#define BINARY_FILE "C:\\teachers.dat"

// 10. Преподаватель
// Поля : фамилия И.О., кафедра, нагрузка в часах, зарплата	
// а) вывести информацию о преподавателях заданной кафедры в новый текстовый файл 
// в порядке убывания нагрузки
// б) определить среднюю зарплату преподавателей, нагрузка которых меньше заданной


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Teacher* teachers = NULL;
    int count = 0;
    int choice;
    bool dataLoaded = false;
    bool binaryDataLoaded = false;

    do {
        printMenu();
        printf("\nВыберите действие: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
        case 1: {
            count = readTeachers(&teachers, FILENAME);
            if (count > 0) {
                dataLoaded = true;
                printf("Данные успешно загружены из %s\n", FILENAME);
            }
            else {
                printf("Ошибка при чтении файла или файл пуст\n");
            }
            break;
        }
        case 2: {
            if (dataLoaded) {
                printTeachers(teachers, count);
            }
            else {
                printf("Данные не загружены. Сначала загрузите данные (пункт 1).\n");
            }
            break;
        }
        case 3: {
            if (dataLoaded) {
                char department[MAX_DEPARTMENT];
                printf("Введите название кафедры: ");
                fgets(department, MAX_DEPARTMENT, stdin);
                department[strcspn(department, "\n")] = '\0';

                writeTeachersToTextFile(teachers, count, OUTPUT_TEXT, department);
                printf("Результат записан в файл %s\n", OUTPUT_TEXT);
            }
            else {
                printf("Данные не загружены. Сначала загрузите данные (пункт 1).\n");
            }
            break;
        }
        case 4: {
            if (dataLoaded) {
                int maxHours;
                printf("Введите максимальную нагрузку в часах: ");
                scanf("%d", &maxHours);
                while (getchar() != '\n');

                double avg = averageSalaryForLowHours(teachers, count, maxHours);
                printf("Средняя зарплата преподавателей с нагрузкой меньше %d часов: %.2f\n", maxHours, avg);
            }
            else {
                printf("Данные не загружены. Сначала загрузите данные (пункт 1).\n");
            }
            break;
        }
        case 5: {
            if (dataLoaded) {
                writeTeachersToBinary(teachers, count, BINARY_FILE);
                printf("Данные записаны в бинарный файл %s\n", BINARY_FILE);
                binaryDataLoaded = true;
            }
            else {
                printf("Данные не загружены. Сначала загрузите данные (пункт 1).\n");
            }
            break;
        }
        case 6: {
            Teacher newTeacher;
            int position;

            printf("Введите ФИО преподавателя: ");
            fgets(newTeacher.name, MAX_NAME, stdin);
            newTeacher.name[strcspn(newTeacher.name, "\n")] = '\0';

            printf("Введите кафедру: ");
            fgets(newTeacher.department, MAX_DEPARTMENT, stdin);
            newTeacher.department[strcspn(newTeacher.department, "\n")] = '\0';

            printf("Введите нагрузку в часах: ");
            scanf("%d", &newTeacher.hours);
            while (getchar() != '\n');

            printf("Введите зарплату: ");
            scanf("%lf", &newTeacher.salary);
            while (getchar() != '\n');

            printf("Введите позицию для вставки: ");
            scanf("%d", &position);
            while (getchar() != '\n');

            insertTeacherToBinary(&newTeacher, position, BINARY_FILE);
            printf("Преподаватель вставлен в бинарный файл на позицию %d\n", position);
            binaryDataLoaded = true;
            break;
        }
        case 7: {
            Teacher newTeacher;

            printf("Введите ФИО преподавателя: ");
            fgets(newTeacher.name, MAX_NAME, stdin);
            newTeacher.name[strcspn(newTeacher.name, "\n")] = '\0';

            printf("Введите кафедру: ");
            fgets(newTeacher.department, MAX_DEPARTMENT, stdin);
            newTeacher.department[strcspn(newTeacher.department, "\n")] = '\0';

            printf("Введите нагрузку в часах: ");
            scanf("%d", &newTeacher.hours);
            while (getchar() != '\n');

            printf("Введите зарплату: ");
            scanf("%lf", &newTeacher.salary);
            while (getchar() != '\n');

            appendTeacherToBinary(&newTeacher, BINARY_FILE);
            printf("Преподаватель добавлен в конец бинарного файла\n");
            binaryDataLoaded = true;
            break;
        }
        case 8: {
            int position;
            printf("Введите номер записи для удаления: ");
            scanf("%d", &position);
            while (getchar() != '\n');

            deleteTeacherFromBinary(position, BINARY_FILE);
            printf("Запись с номером %d удалена из бинарного файла\n", position);
            binaryDataLoaded = true;
            break;
        }
        case 9: {
            int maxHours;
            printf("Введите максимальную нагрузку в часах для поиска: ");
            scanf("%d", &maxHours);
            while (getchar() != '\n');

            searchInBinaryFile(BINARY_FILE, maxHours);
            break;
        }
        case 10: {
            if (binaryDataLoaded) {
                Teacher* binaryTeachers = NULL;
                int binaryCount = readTeachersFromBinary(&binaryTeachers, BINARY_FILE);
                if (binaryCount > 0) {
                    printTeachers(binaryTeachers, binaryCount);
                    free(binaryTeachers);
                }
                else {
                    printf("Ошибка при чтении бинарного файла или файл пуст\n");
                }
            }
            else {
                printf("Бинарные данные не загружены. Сначала выполните запись в бинарный файл (пункт 5).\n");
            }
            break;
        }
        case 0: {
            break;
        }
        default: {
            printf("Неверный выбор. Попробуйте снова.\n");
            break;
        }
        }
    } while (choice != 0);

    if (dataLoaded) {
        free(teachers);
    }

    return 0;
}