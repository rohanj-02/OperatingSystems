#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	time_t currTime = time(NULL);
	struct tm *currTimeStruct = localtime(&currTime);
	char output[150];
	strftime(output, sizeof(output), "%A %d %B %Y %T %Z(%z)\n", currTimeStruct);

	for (int i = 0; output[i] != '\0'; i++)
	{
		write(STDOUT_FILENO, &output[i], 1);
	}
	return 0;
}