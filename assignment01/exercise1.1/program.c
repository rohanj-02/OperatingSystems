#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
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
	int fptr = open("./data.csv", O_RDONLY);
	if (fptr == -1)
	{
		printf("Make sure the file is in the same directory as the program! ");
		exit(1);
	}
	char headers = '1';
	while (headers != '\n')
	{
		read(fptr, &headers, 1);
	}

	bool isEOF = false;
	while (1)
	{
		if (isEOF)
		{
			break;
		}
		char row[21];
		int count = 0;
		char x;
		bool isInput = true;
		while (1)
		{
			if (!isInput || isEOF)
			{
				break;
			}
			if (read(fptr, &x, 1) == 0)
			{
				isEOF = true;
				break;
			}
			if (x == '\n')
			{
				isInput = false;
			}
			if (count == 0)
			{
				char temp[2] = "\0";
				temp[0] = x;
				strcpy(row, temp);
				count++;
			}
			else
			{

				char temp[2] = "\0";
				temp[0] = x;
				strcat(row, temp);
			}
		}
		char *entries = strtok(row, ",");
		if (getSection(entries) == section)
		{
			struct student s = getStudentDetails(entries, section);
			printStudent(s);
		}
	}
	close(fptr);
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