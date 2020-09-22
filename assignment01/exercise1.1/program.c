#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct student
{
	int id;
	char section;
	float average;
};

struct student getStudentDetails(char *entry, char section)
{
	struct student s;
	int count = 0;
	s.average = 0;
	s.section = section;
	while (entry != NULL)
	{
		if (count == 0)
		{
			s.id = atoi(entry);
		}
		else if (count < 6)
		{
			s.average += atoi(entry);
		}
		count++;
		entry = strtok(NULL, ",");
	}
	s.average /= 4;
	return s;
}

void printStudent(struct student s)
{
	printf("ID: %d, Section: %c, Average: %f\n", s.id, s.section, s.average);
}

char getSection(char *entry)
{
	// int count = 0;
	int count = 0;
	while (entry != NULL)
	{
		if (count == 1)
		{
			return entry[0];
		}
		count++;
		entry = strtok(NULL, ",");
	}
	return 'Z';
}

void parseCsv(char section)
{
	FILE *fptr;
	fptr = fopen("./data.csv", "r");
	if (fptr == NULL)
	{
		printf("Make sure the file is in the same directory as the program! ");
		exit(1);
	}
	char headers[200];
	fscanf(fptr, "%s", headers);
	char row[21];
	int count = 0;
	bool end = false;
	while (!end)
	{
		if (feof(fptr))
		{
			end = feof(fptr);
			break;
		}
		fscanf(fptr, "%s", row);
		char *entries = strtok(row, ",");
		if (getSection(entries) == section)
		{
			struct student s = getStudentDetails(entries, section);
			printStudent(s);
		}
		count++;
		end = feof(fptr);
	}
	fclose(fptr);
}

int main(int argc, char *argv[])
{
	pid_t process;
	int status;
	process = fork();
	if (process == -1)
	{
		printf("Could not spawn child process");
		exit(1);
	}
	if (process == 0)
	{
		printf("Child process running! PID is: %ld\n", (long)getpid());
		parseCsv('A');
		printf("Terminating Child Process! \n");
		exit(0);
	}
	else
	{
		waitpid(-1, NULL, 0);
		printf("Parent Process running! PID is: %ld\n", (long)getpid());
		parseCsv('B');
		printf("Terminating Parent Process! \n");
		exit(0);
	}
	return 0;
}