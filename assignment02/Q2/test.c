#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SYSCALL_NUMBER 440
#define PATH_LIMIT 256

int main(int argc, char **argv)
{
	char user_filename[PATH_LIMIT];
	pid_t pid;
	long int sys_return;

	printf("Enter pid(-1 to send pid of this process): ");
	scanf("%d", &pid);
	printf("Enter filename: ");
	fgets(user_filename, PATH_LIMIT, stdin);
	fgets(user_filename, PATH_LIMIT, stdin);
	for (int i = 0; user_filename[i] != '\0'; i++)
	{
		if (user_filename[i + 1] == '\0')
		{
			user_filename[i] = '\0';
		}
	}

	if (pid == -1)
	{
		sys_return = syscall(SYSCALL_NUMBER, getpid(), user_filename);
	}
	else
	{
		sys_return = syscall(SYSCALL_NUMBER, pid, user_filename);
	}
	if (sys_return != 0)
	{
		perror("Error: sh_task_info");
		return -1;
	}
	printf("Run dmesg or see the file in your current directory to check the output.\n");
	return 0;
}
