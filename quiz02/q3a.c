#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

// Sender or Client
#define PORT_NAME "1500" // Common port for all
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
	int sock_descriptor, fptr, return_val, read_bytes;
	struct addrinfo hints, *res, *iter;

	// Setup hints to use getaddrinfo() to connect to the receiver
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = AF_UNSPEC;

	// Get LL of struct addrinfo() corresponding to LOCALHOST PORT_NAME and hints.
	if ((return_val = getaddrinfo(LOCALHOST, PORT_NAME, &hints, &res)) != 0)
	{
		fprintf(stderr, "getaddrinfo(): error: %s\n", gai_strerror(return_val));
		exit(1);
	}

	// Loop through all entries and try to make a socket
	for (iter = res; iter != NULL; iter = iter->ai_next)
	{
		if ((sock_descriptor = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol)) != -1)
		{
			break;
		}
		perror("socket(): error");
	}

	if (iter == NULL)
	{
		fprintf(stderr, "Socket not created\n");
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

	// Send individual tokens to the socket
	while (tokens != NULL)
	{
		int bytes_sent = 0;
		struct data buffer;
		buffer.sender = 1; // Sent from para 1
		memset(buffer.str, '\0', MAX_SIZE);
		strcpy(buffer.str, tokens);
		int len = strlen(tokens);

		if ((bytes_sent = sendto(sock_descriptor, &buffer, sizeof(buffer), 0, iter->ai_addr, iter->ai_addrlen)) < len)
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
	freeaddrinfo(res);		// free the LL of struct addrinfo
	close(sock_descriptor); // close the socket

	return 0;
}
