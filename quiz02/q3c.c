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

// Receiver or Server
#define PORT_NAME 1500 // Common port for all
#define LOCALHOST "127.0.0.1"
#define MAX_SIZE 501

struct data
{
	long sender;
	char str[MAX_SIZE];
};

int main(void)
{
	int sock_descriptor, flag = 1;
	struct sockaddr_in server_address;
	struct in_addr localhost;

	memset(&server_address, 0, sizeof(server_address));
	memset(&localhost, 0, sizeof(localhost));

	if ((sock_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket(): error");
		exit(1);
	}

	if (inet_pton(AF_INET, LOCALHOST, &localhost) != 1)
	{
		perror("inet_pton(): error");
		exit(1);
	}
	server_address.sin_addr = localhost;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT_NAME);

	if (bind(sock_descriptor, (const struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		perror("bind(): error");
		exit(1);
	}

	// Receive messages till \n\n is not received
	while (flag)
	{
		int bytes_received;
		struct data buffer;
		memset(buffer.str, '\0', MAX_SIZE);

		// No use of this because we don't have to send response
		struct sockaddr_storage client_address;
		socklen_t len_client = sizeof(client_address);

		if ((bytes_received = recvfrom(sock_descriptor, &buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &len_client)) == -1)
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
