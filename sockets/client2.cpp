#include <bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
using namespace std;
void *handle_connection(void *arg);
int main()
{
    int socket_desc , new_socket , c , *new_sock, i;
    
    pthread_t sniffer_thread;
    for (i=1; i<=10; i++) {
        if( pthread_create( &sniffer_thread , NULL ,  &handle_connection , &i) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        // sleep(2);
    }
    pthread_exit(NULL);
    return 0;
}

void *handle_connection(void *arg)
{
    int threadnum =  *((int *)arg);
    int socket_desc;
    struct sockaddr_in server_addr;

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0)
    {
        printf("Unable to create socket\n");
        return NULL;
    }

    printf("Socket created successfully\n");

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2002);
    // server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // Send connection request to server:
    if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Unable to connect\n");
        return NULL;
    }
    printf("Connected with server successfully\n");
    // int i=2;
    for (int i = 0; i < 20; i++)
    {
        printf("For thread : %d\n", threadnum);
        if (write(socket_desc, &i, sizeof(i)) < 0)
        {
            printf("Unable to send message\n");
            return NULL;
        }
        // sleep(1);

        long long max;
        int return_status = read(socket_desc, &max, sizeof(max));
        if (return_status > 0)
        {

            cout << max << endl;
        }
        else
        {
            printf("error");
        }
    }
    // Close the socket:

    close(socket_desc);
    return NULL;
}