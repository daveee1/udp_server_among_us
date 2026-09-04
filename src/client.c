// Client side implementation of UDP client-server model
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>   /* Provides recv(), send(), socket(), accept() */
#include <netinet/in.h>   /* Provides struct sockaddr_in, htons(), ntohs() */
#include <arpa/inet.h>    /* Provides inet_ntop(), inet_pton() */
#include <sys/select.h>
#include <time.h>


#include "headers/utils.h"

#define PORT	8080
#define MAXLINE 1024


typedef enum {
    CLIENT_CMD_GO_FRONT,
    CLIENT_CMD_GO_BACK,
    CLIENT_CMD_GO_LEFT,
    CLIENT_CMD_GO_RIGHT,
    CLIENT_CMD_HELP,
    CLIENT_CMD_QUIT,
    CLIENT_CMD_STOP,
    CLIENT_CMD_INVALID
} ClientCmdType;

/* Parse raw user input into an enum */
static ClientCmdType parse_user_input(const char *input) {
    if ( !strcmp(input, "w")) return CLIENT_CMD_GO_FRONT;
    else if (!strcmp(input, "a")) return CLIENT_CMD_GO_LEFT;
    else if (!strcmp(input, "s")) return CLIENT_CMD_GO_BACK;
    else if (!strcmp(input, "d")) return CLIENT_CMD_GO_RIGHT;
    else if (!strcmp(input, "help")) return CLIENT_CMD_HELP;
    else if (!strcmp(input, "quit") || !strcmp(input, "exit")) return CLIENT_CMD_QUIT;
    else if (!strcmp(input, "stop")) return CLIENT_CMD_STOP;
    return CLIENT_CMD_INVALID;
}

static const char* format_server_payload(ClientCmdType cmd) {
    switch (cmd) {
        case CLIENT_CMD_GO_BACK: return "back";
        case CLIENT_CMD_GO_FRONT: return "front";
        case CLIENT_CMD_GO_RIGHT: return "right";
        case CLIENT_CMD_GO_LEFT: return "left";
        case CLIENT_CMD_QUIT:  return "quit";
        case CLIENT_CMD_STOP:  return "stop";
        default:               return "help";
    }
}



static int handle_user_input(int socket, struct sockaddr_in	 *servaddr){
    char input[10];
    if(fgets(input, sizeof(input), stdin) == NULL){
        print_client_error("FGETS returned NULL");
        return;
    }

    // Skip empty lines
    if (strlen(input) == 0) return;

    // parse the input into a server command
    ClientCmdType cmd = parse_user_input(input);
    if(cmd == CLIENT_CMD_INVALID)
        return -1;
    
    char *payload = format_server_payload(cmd);

    // must send it to the server
    sendto(socket, (const char *)payload, strlen(payload),
            MSG_CONFIRM, (const struct sockaddr *) servaddr,
                sizeof(*servaddr));
}

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
        // define a timer to check for each 'timer.tv_usec' if there is a input or response
            // from the server
        struct timeval timer;
        timer.tv_sec = 0;
        timer.tv_usec = 500000;
        int select_activity = select(max_fd + 1, &read_fds, NULL, NULL, &timer);
        if(select_activity < 0){
            print_client_error("SELECT error");
            exit(1);
        }

        // select (input || handle answer by server)
        if(FD_ISSET(client_socket, &read_fds) > 0){
            // handle server response
            if(handle_server_ans() == -1){

                return;
            }
        }

        else if(FD_ISSET(STDIN_FILENO, &read_fds)){
            // 1. i give as input a command to move ...
            handle_user_input(client_socket);

            
        }


        // 2. therefore i expect to see my movement on screen!
        //     call a callback function for it 

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
