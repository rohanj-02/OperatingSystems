/* Name: Rohan Jain
   Roll_Number: 2019095 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	char user_filename[256];
	pid_t pid;
	long int sys_return;

	printf("Enter pid(-1 to send pid of this process): ");
	scanf("%d", &pid);
	printf("Enter filename: ");
	scanf("%s", user_filename);
	
	if(pid == -1){
		sys_return = syscall(440, getpid(), user_filename);
	}
	else{
		sys_return = syscall(440, pid, user_filename);		
	}
	if(sys_return != 0){
		perror("Error: sh_task_info");
		return -1;
	}
	printf("Run dmesg or see the file in your current directory to check the output.\n");
	return 0;
}
