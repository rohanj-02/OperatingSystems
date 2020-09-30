// Written by Rohan Jain 2019095

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

void writeToStdout(char *str, int length)
{
	if (write(STDOUT_FILENO, str, length) == -1)
	{
		perror("write: error");
		exit(1);
	}
	return;
}

void printStudent(struct student s)
{
	char id[] = "ID: ";
	char section[] = "\t\tSection: ";
	char average[] = "\tAverage: ";
	char student_id[4];
	int id_length;
	if ((int)(s.id / 1000) == 0)
		id_length = 3;
	if ((int)(s.id / 100) == 0)
		id_length = 2;
	if ((int)(s.id / 10) == 0)
		id_length = 1;
	char student_average[8];
	sprintf(student_id, "%d", s.id);
	sprintf(student_average, "%.3f\n", s.average);
	writeToStdout(id, sizeof(id));
	writeToStdout(student_id, id_length);
	writeToStdout(section, sizeof(section));
	writeToStdout(&s.section, sizeof(s.section));
	writeToStdout(average, sizeof(average));
	writeToStdout(student_average, sizeof(student_average));
	// printf("ID: %d, Section: %c, Average: %f\n", s.id, s.section, s.average);
}

char getSection(char *entry)
{
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
		perror("open: Error in opening data.csv");
		exit(1);
	}
	//Discard headers from csv file
	char headers = '1';
	while (headers != '\n')
	{
		read(fptr, &headers, 1);
	}

	bool isEOF = false;
	//External loop looping over each record
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
		//Loop to get one row
		while (1)
		{
			if (!isInput || isEOF)
			{
				break;
			}
			int status = read(fptr, &x, 1);
			if (status == 0)
			{
				isEOF = true;
				break;
			}
			if (status == -1)
			{
				perror("read: error");
				exit(1);
			}
			if (x == '\n')
			{
				isInput = false;
			}
			//If first byte then strcpy else strcat
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
		// If section matches then construct student struct and print student details
		char *entries = strtok(row, ",");
		if (getSection(entries) == section)
		{
			struct student s = getStudentDetails(entries, section);
			printStudent(s);
		}
	}
	if (close(fptr) == -1)
	{
		perror("close: error");
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	pid_t process;
	process = fork();
	if (process == -1)
	{
		perror("fork: error");
		// printf("Could not spawn child process");
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
		if (waitpid(-1, NULL, 0) == -1)
		{
			perror("wait: error");
			exit(1);
		}
		printf("Parent Process running! PID is: %ld\n", (long)getpid());
		parseCsv('B');
		printf("Terminating Parent Process! \n");
		exit(0);
	}
	return 0;
}