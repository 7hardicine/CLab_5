#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#define MAXSIZE 80

struct Teachers
{
	char FIO[MAXSIZE];
	char Chair[MAXSIZE];
	int Workload;
	int Salary;
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char word[4][20];
	struct Teachers* teacher = NULL;
	FILE* fPtr;
	if ((fPtr = fopen("D:\Teachers.txt", "r")) == NULL)
		perror("Файл не может быть открыт");
	else
	{
		char* teacher = (char*)malloc(sizeof(struct Teachers) + 1);
		int teacherCount = 0;
		while (!feof(fPtr))
		{
			if (fgets(teacher, MAXSIZE, fPtr) != NULL)
			{
				teacherCount++;
				teacher = (struct Teachers*)realloc(teacher, teacherCount * sizeof(struct Teachers));
				teacher[strlen(teacher) - 1] = '\0';
				char* wrd = strtok(teacher, ";");
				for (int i = 0; i < 4; i++)
				{
					strcpy(word[i], wrd);
					wrd = strtok('\0', ";");
				}
				strcpy(teacher[teacherCount - 1].FIO, FIO);
			}
		}
	}

	system("pause");
	return 0;
}