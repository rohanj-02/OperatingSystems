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

int main(void)
{
	int sock_descriptor, return_val;
	struct addrinfo hints, *res, *iter;

	// Setup hints to use getaddrinfo()
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // Generic code for IPv4 and IPv6
	hints.ai_socktype = SOCK_DGRAM;

	// Get LL of struct addrinfo() corresponding to LOCALHOST PORT_NAME and hints.
	if ((return_val = getaddrinfo(LOCALHOST, PORT_NAME, &hints, &res)) != 0)
	{
		fprintf(stderr, "getaddrinfo(): error: %s\n", gai_strerror(return_val));
		exit(1);
	}

	// Dont assume first entry in LL is correct, loop through all and try to make a socket
	for (iter = res; iter != NULL; iter = iter->ai_next)
	{
		if ((sock_descriptor = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol)) != -1)
		{
			if (bind(sock_descriptor, iter->ai_addr, iter->ai_addrlen) != -1)
			{
				break;
			}
			close(sock_descriptor);
			perror("listener: bind");
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

	printf("listener: waiting to recvfrom...\n");

	while (1)
	{
		int bytes_received;
		char str[MAX_SIZE];
		struct sockaddr_storage server_address;
		socklen_t addr_len = sizeof(server_address);

		if ((bytes_received = recvfrom(sock_descriptor, str, MAX_SIZE - 1, 0, (struct sockaddr *)&server_address, &addr_len)) == -1)
		{
			perror("recvfrom(): error");
			exit(1);
		}
		str[bytes_received] = '\0';
		printf("%s ", str);
	}

	// Cleanup: Close Socket
	close(sock_descriptor);

	return 0;
}
