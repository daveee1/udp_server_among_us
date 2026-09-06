#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "headers/utils.h"
#include <sys/select.h>
#include <pthread.h>
   
#define PORT    8080
#define MAXLINE 1024
#define MAX_NUMBER_OF_CLIENTS 5
#define MAP_SIZE 25
   
typedef struct
{
    int port;
    int active;
    int position;
    int socket;
    char *ip;
    pthread_t thread_id;
}Client;
Client clients[MAX_NUMBER_OF_CLIENTS];

// map: matrix
int matrix[MAP_SIZE][MAP_SIZE];


//MUTEXES
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t roomAvb = PTHREAD_COND_INITIALIZER;


static int find_free_pos_in_clients(){
    for (int i = 0; i < MAX_NUMBER_OF_CLIENTS; i++){
        if(clients[i].active == 0){
            pthread_mutex_unlock(&clients_mutex);
            return i;
        }
    }
    return -1;
}

static int addclient(struct sockaddr_in *client_addr){
    int free_pos = find_free_pos_in_clients();
    while(free_pos == -1){
        pthread_cond_wait(&roomAvb, &clients_mutex);
        free_pos = find_free_pos_in_clients();
    }
    pthread_mutex_lock(&clients_mutex);

    Client *new = &clients[free_pos];
    new->active = 1;
    new->port = client_addr->sin_port;
    new->position = free_pos;
    new->ip = inet_ntoa(client_addr->sin_addr);
    // new->socket = c->socket;

    pthread_mutex_unlock(&clients_mutex);
    return 1;
}

static int find_client(struct sockaddr_in *client_addr){
    int port = ntohs(client_addr->sin_port);
    char *ip_str = inet_ntoa(client_addr->sin_addr);

    for(int i=0; i < MAX_NUMBER_OF_CLIENTS; i++){
        Client *curr_client;
        if(curr_client->active == 1 && curr_client->port == port && !strcmp(ip_str, curr_client->ip)){
            return 1;
        }
    }
    return -1;
}

static void handle_packet(int server_fd){
    char buffer[10];
    struct sockaddr_in cli_addr;
    socklen_t addr_len = sizeof(cli_addr);

    ssize_t bytes = recvfrom(server_fd, buffer, sizeof(buffer) - 1, 0,
                             (struct sockaddr *)&cli_addr, &addr_len);
    if (bytes <= 0) return;

    buffer[bytes] = '\0';

    // 1. Identify or register the client
    int player_idx = find_client(&cli_addr);
    if (player_idx == -1) {
        player_idx = addclient(&cli_addr);
        if (player_idx == -1) {
            printf("Server is full, dropping packet.\n");
            return;
        }
    }
    
    // manage the packet TODO


}

static void init_clients(){
    pthread_mutex_lock();
}


// Driver code
int main() {
    int server_fd;
    char buffer[MAXLINE];
    char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;
       
    // Creating socket file descriptor
    if ( (server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
       
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
       
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
       
    // Bind the socket with the server address 
    // (associate the server to the following port)
    if ( bind(server_fd, (const struct sockaddr *)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
       
    socklen_t len;
    ssize_t n;
    len = sizeof(cliaddr);  //len is value



    while(1){
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds); // to check if a client wants to connect
        
        // each 5 seconds check if a client wants to connect to us
        struct timeval timer;
        timer.tv_sec = 5; 
        timer.tv_usec = 0;


        int select_activity = select(server_fd + 1, &read_fds, NULL, NULL, &timer);
        if(select_activity < 0){
            printf("SELECT error");
            exit(1);
        }

        // a client sent a packet!
        if(FD_ISSET(server_fd, &read_fds)){
            // TODO addclient to my server
            handle_packet(server_fd);
        }

        
    }
    // MSG_WAITALL to rcv the whole response
    n = recvfrom(server_fd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
    buffer[n] = '\0';

    printf("Client : %s\n", buffer);


    // MSG_CONFIRM to state not control every time the 'mac of the ip' address: 
    // trust this and keep going dont check
    sendto(server_fd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
    printf("Hello message sent.\n"); 
       
    return 0;
}