#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>

#define SOCK_PATH "socket_q3"
#define MAX_SIZE 501

int main(void)
{
	int sock_descriptor, sock_client_desc, len;
	struct sockaddr_in local, remote;

	// Get socket descriptor
	if ((sock_descriptor = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket(): error");
		exit(1);
	}

	// Standard
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, SOCK_PATH);
	// Delete the socket file if it exists
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);

	// Bind the socket descriptor to SOCK_PATH
	if (bind(sock_descriptor, (struct sockaddr *)&local, len) == -1)
	{
		perror("bind(): error");
		exit(1);
	}

	// Listen to connections, if more than 2 try to connect they receive ECONNREFUSED
	if (listen(sock_descriptor, 5) == -1)
	{
		perror("listen(): error");
		exit(1);
	}

	while (true)
	{
		int bytes_received;
		bool done = false;
		char str[MAX_SIZE];
		int temp_size = sizeof(remote);

		printf("Waiting for a connection...\n");

		// Get socket descriptor connected to client
		if ((sock_client_desc = accept(sock_descriptor, (struct sockaddr *)&remote, &temp_size)) == -1)
		{
			perror("accept(): error");
			exit(1);
		}
		printf("Connected.\n");

		done = false;

		do
		{
			// Receive data from client
			bytes_received = recv(sock_client_desc, str, MAX_SIZE, 0);
			if (bytes_received <= 0)
			{
				if (bytes_received < 0)
				{
					perror("recv(): error");
				}
				//Error so no response as no read
				done = true;
			}

			// Send ok response on receiving data
			if (!done)
			{
				char response[10] = "200";
				if (send(sock_client_desc, response, sizeof(response), 0) < 0)
				{
					perror("send(): error");
					done = true;
				}
			}
		} while (!done);

		close(sock_client_desc);
	}
	return 0;
}