/*	Name: Rohan Jain
	Roll No.: 2019095 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

// Sender or Client
#define PORT_NAME 1500 // Common port for all
#define LOCALHOST "127.0.0.1"
#define MAX_SIZE 501
#define FILENAME "./para1.txt"

struct data
{
	long sender;
	char str[MAX_SIZE];
};

int main(void)
{
	char file_input[MAX_SIZE];
	int sock_descriptor, fptr, read_bytes;
	struct sockaddr_in server;
	struct in_addr localhost;

	memset(&server, 0, sizeof(server));
	memset(&file_input, '\0', MAX_SIZE);
	memset(&localhost, 0, sizeof(localhost));

	if ((sock_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) //IPv4 only
	{
		perror("socket(): error");
		exit(1);
	}

	// Socket succesfully created
	// Open file
	fptr = open(FILENAME, O_RDONLY);
	if (fptr == -1)
	{
		perror("open(): error");
		exit(1);
	}

	// Read the file and tokenize it
	if ((read_bytes = read(fptr, &file_input, MAX_SIZE)) == -1)
	{
		perror("read(): error");
		exit(1);
	}

	file_input[read_bytes] = '\0';
	char *tokens = strtok(file_input, " ");

	if (inet_pton(AF_INET, LOCALHOST, &localhost) != 1)
	{
		perror("inet_pton(): error");
		exit(1);
	}
	server.sin_addr = localhost;
	server.sin_port = htons(PORT_NAME);
	server.sin_family = AF_INET;

	// Send individual tokens to the socket
	while (tokens != NULL)
	{
		int bytes_sent = 0;
		struct data buffer;
		buffer.sender = 1; // Sent from para 1
		memset(buffer.str, '\0', MAX_SIZE);
		strcpy(buffer.str, tokens);
		int len = strlen(tokens);

		if ((bytes_sent = sendto(sock_descriptor, &buffer, sizeof(buffer), 0, (const struct sockaddr *)&server, sizeof(server))) < len)
		{
			if (bytes_sent == -1)
			{
				perror("sendto(): error");
				exit(1);
			}
			else
			{
				fprintf(stderr, "Did not send all the data to the socket!\n");
			}
		}

		tokens = strtok(NULL, " ");
	}

	// Cleanup
	close(sock_descriptor); // close the socket

	return 0;
}
