#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

// Receiver or Server
#define PORT_NAME "1500" // Common port for all
#define LOCALHOST "127.0.0.1"
#define MAX_SIZE 501

struct data
{
	long sender;
	char str[MAX_SIZE];
};

unsigned int get_port(struct sockaddr *server)
{
	if (server->sa_family == AF_INET6)
	{
		return ((struct sockaddr_in6 *)server)->sin6_port;
	}
	return ((struct sockaddr_in *)server)->sin_port;
}

int main(void)
{
	int sock_descriptor, return_val, flag = 1;
	struct addrinfo hints, *res, *iter;

	// Setup hints to use getaddrinfo()
	memset(&hints, 0, sizeof hints);
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
			if (bind(sock_descriptor, iter->ai_addr, iter->ai_addrlen) != -1)
			{
				break;
			}
			close(sock_descriptor);
			perror("bind(): error");
		}
		perror("socket(): error");
	}

	if (iter == NULL)
	{
		fprintf(stderr, "Socket not created\n");
		exit(1);
	}

	// Socket succesfully created
	// Cleanup: Free the LL of struct addrinfo because process going in inf loop
	freeaddrinfo(res);

	// Receive messages till \n\n is not received
	while (flag)
	{
		int bytes_received;
		struct data buffer;
		memset(buffer.str, '\0', MAX_SIZE);

		// No use of this because we don't have to send response
		struct sockaddr_storage server_address;
		socklen_t len_server = sizeof(server_address);

		if ((bytes_received = recvfrom(sock_descriptor, &buffer, sizeof(buffer), 0, (struct sockaddr *)&server_address, &len_server)) == -1)
		{
			perror("recvfrom(): error");
			exit(1);
		}

		ulong len = strlen(buffer.str);
		buffer.str[len] = '\0';

		// Stop receving if \n\n is received
		if (len >= 2)
		{
			if (buffer.str[len - 1] == '\n' && buffer.str[len - 2] == '\n')
			{
				flag = 0;
				if (len != 2)
				{
					buffer.str[len - 2] = '\0';
					printf("%s from para%ld\n", buffer.str, buffer.sender);
				}
			}
		}
		if (flag)
		{
			printf("%s from para%ld\n", buffer.str, buffer.sender);
		}
	}

	// Cleanup: Close Socket
	close(sock_descriptor);

	return 0;
}
