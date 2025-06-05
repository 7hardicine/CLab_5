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

void printMenu() {
    puts("Меню:");
    puts("1. Загрузить данные из CSV файла");
    puts("2. Вывести данные в виде таблицы");
    puts("3. Записать преподавателей заданной кафедры в текстовый файл (по убыванию нагрузки)");
    puts("4. Найти среднюю зарплату преподавателей с нагрузкой меньше заданной");
    puts("5. Записать данные в бинарный файл");
    puts("6. Вставить запись в бинарный файл по номеру");
    puts("7. Добавить запись в конец бинарного файла");
    puts("8. Удалить запись с указанным номером из бинарного файла");
    puts("9. Поиск в бинарном файле (преподаватели с нагрузкой меньше заданной)");
    puts("10. Вывести содержимое бинарного файла");
    puts("0. Выход");
}

void swapTeachers(Teacher* a, Teacher* b) {
    Teacher temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Teacher* teachers, int low, int high) {
    int pivot = teachers[high].hours;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (teachers[j].hours >= pivot) {
            i++;
            swapTeachers(&teachers[i], &teachers[j]);
        }
    }
    swapTeachers(&teachers[i + 1], &teachers[high]);
    return (i + 1);
}

void quickSort(Teacher* teachers, int low, int high) {
    if (low < high) {
        int pi = partition(teachers, low, high);
        quickSort(teachers, low, pi - 1);
        quickSort(teachers, pi + 1, high);
    }
}

int readTeachers(Teacher** teachers, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка открытия файла %s\n", filename);
        return 0;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        count++;
    }

    if (count == 0) {
        fclose(file);
        return 0;
    }

    *teachers = (Teacher*)malloc(count * sizeof(Teacher));
    if (!*teachers) {
        fclose(file);
        printf("Ошибка выделения памяти\n");
        return 0;
    }

    rewind(file);
    int i = 0;
    while (fgets(line, sizeof(line), file) && i < count) {
        line[strcspn(line, "\r\n")] = '\0';

        char* token = strtok(line, ";");
        if (token) strncpy((*teachers)[i].name, token, MAX_NAME);

        token = strtok(NULL, ";");
        if (token) strncpy((*teachers)[i].department, token, MAX_DEPARTMENT);

        token = strtok(NULL, ";");
        if (token) (*teachers)[i].hours = atoi(token);

        token = strtok(NULL, ";");
        if (token) (*teachers)[i].salary = atof(token);

        i++;
    }
    fclose(file);
    return count;
}

void printTeachers(Teacher* teachers, int count) {
    printf("\n%-20s %-15s %-10s %-10s\n", "ФИО", "Кафедра", "Часы", "Зарплата");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-20s %-15s %-10d %-10.2f\n",
            teachers[i].name,
            teachers[i].department,
            teachers[i].hours,
            teachers[i].salary);
    }
}

void writeTeachersToTextFile(Teacher* teachers, int count, const char* filename, const char* department) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Ошибка открытия файла %s для записи\n", filename);
        return;
    }

    fprintf(file, "%-20s %-15s %-10s %-10s\n", "ФИО", "Кафедра", "Часы", "Зарплата");
    fprintf(file, "--------------------------------------------------\n");

    quickSort(teachers, 0, count - 1);

    for (int i = 0; i < count; i++) {
        if (strcmp(teachers[i].department, department) == 0) {
            fprintf(file, "%-20s %-15s %-10d %-10.2f\n",
                teachers[i].name,
                teachers[i].department,
                teachers[i].hours,
                teachers[i].salary);
        }
    }
    fclose(file);
}

double averageSalaryForLowHours(Teacher* teachers, int count, int maxHours) {
    double sum = 0;
    int n = 0;

    for (int i = 0; i < count; i++) {
        if (teachers[i].hours < maxHours) {
            sum += teachers[i].salary;
            n++;
        }
    }
    return n > 0 ? sum / n : 0;
}

void writeTeachersToBinary(Teacher* teachers, int count, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Ошибка открытия бинарного файла %s для записи\n", filename);
        return;
    }

    fwrite(&count, sizeof(int), 1, file);
    fwrite(teachers, sizeof(Teacher), count, file);
    fclose(file);
}

int readTeachersFromBinary(Teacher** teachers, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Ошибка открытия бинарного файла %s для чтения\n", filename);
        return 0;
    }

    int count;
    fread(&count, sizeof(int), 1, file);

    *teachers = (Teacher*)malloc(count * sizeof(Teacher));
    if (!*teachers) {
        fclose(file);
        printf("Ошибка выделения памяти\n");
        return 0;
    }

    fread(*teachers, sizeof(Teacher), count, file);
    fclose(file);
    return count;
}

void insertTeacherToBinary(Teacher* teacher, int position, const char* filename) {
    Teacher* teachers = NULL;
    int count = readTeachersFromBinary(&teachers, filename);

    teachers = (Teacher*)realloc(teachers, (count + 1) * sizeof(Teacher));
    for (int i = count; i > position; i--) {
        teachers[i] = teachers[i - 1];
    }
    teachers[position] = *teacher;

    writeTeachersToBinary(teachers, count + 1, filename);
    free(teachers);
}

void appendTeacherToBinary(Teacher* teacher, const char* filename) {
    Teacher* teachers = NULL;
    int count = readTeachersFromBinary(&teachers, filename);

    teachers = (Teacher*)realloc(teachers, (count + 1) * sizeof(Teacher));
    teachers[count] = *teacher;

    writeTeachersToBinary(teachers, count + 1, filename);
    free(teachers);
}

void deleteTeacherFromBinary(int position, const char* filename) {
    Teacher* teachers = NULL;
    int count = readTeachersFromBinary(&teachers, filename);

    if (position < 0 || position >= count) {
        printf("Некорректная позиция для удаления\n");
        free(teachers);
        return;
    }

    for (int i = position; i < count - 1; i++) {
        teachers[i] = teachers[i + 1];
    }

    writeTeachersToBinary(teachers, count - 1, filename);
    free(teachers);
}

void searchInBinaryFile(const char* filename, int maxHours) {
    Teacher* teachers = NULL;
    int count = readTeachersFromBinary(&teachers, filename);

    printf("\nРезультаты поиска (нагрузка < %d часов):\n", maxHours);
    printf("%-20s %-15s %-10s %-10s\n", "ФИО", "Кафедра", "Часы", "Зарплата");
    printf("--------------------------------------------------\n");

    double sum = 0;
    int n = 0;
    for (int i = 0; i < count; i++) {
        if (teachers[i].hours < maxHours) {
            printf("%-20s %-15s %-10d %-10.2f\n",
                teachers[i].name,
                teachers[i].department,
                teachers[i].hours,
                teachers[i].salary);
            sum += teachers[i].salary;
            n++;
        }
    }

    if (n > 0) {
        printf("\nСредняя зарплата: %.2f\n", sum / n);
    }
    else {
        printf("Преподавателей с нагрузкой меньше %d часов не найдено\n", maxHours);
    }
    free(teachers);
}

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