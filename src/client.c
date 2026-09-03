// Client side implementation of UDP client-server model
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>   /* Provides recv(), send(), socket(), accept() */
#include <netinet/in.h>   /* Provides struct sockaddr_in, htons(), ntohs() */
#include <arpa/inet.h>    /* Provides inet_ntop(), inet_pton() */
#include <sys/select.h>

#include "headers/utils.h"

#define PORT	8080
#define MAXLINE 1024


// Driver code
int main() {
	int client_socket;
	char buffer[MAXLINE];
	char *hello = "Hello from client";
	struct sockaddr_in	 servaddr;

	// Creating socket file descriptor
	if ( (client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
	int n, len;
	
    while(1){
        fd_set read_fds;
        FD_ZERO(&read_fds);

        FD_SET(STDIN_FILENO, &read_fds); // to check input
        FD_SET(client_socket, &read_fds); // to check server's responses

        // w.r.t. the theory we need considerate highest sokect value
        int max_fd = (client_socket > STDIN_FILENO) ? client_socket : STDIN_FILENO;
        int select_activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if(select_activity < 0){
            print_client_error("SELECT error");
            exit(1);
        }

        // select (input || handle answer by server)
        if(FD_ISSET(client_socket, &read_fds)){
            
        }
            // 1. i give as input a command to move therefore...


            // 2. i expect to see my movement on screen!

        sendto(client_socket, (const char *)hello, strlen(hello),
            MSG_CONFIRM, (const struct sockaddr *) &servaddr,
                sizeof(servaddr));
        printf("Command sent.\n");
            



        n = recvfrom(client_socket, (char *)buffer, MAXLINE,
                    MSG_WAITALL, (struct sockaddr *) &servaddr,
                    &len);
        buffer[n] = '\0';
        printf("Server : %s\n", buffer);
    }
	close(client_socket);
	return 0;
}
