#include <bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
using namespace std;

int main(void)
{
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[100];

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0)
    {
        printf("Unable to create socket\n");
        return -1;
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
        return -1;
    }
    printf("Connected with server successfully\n");
    // int i=2;
    for (int i = 0; i < 20; i++)
    {
        if (write(socket_desc, &i, sizeof(i)) < 0)
        {
            printf("Unable to send message\n");
            return -1;
        }
        // sleep(1);

        long long max;
        int return_status = read(socket_desc, &max, sizeof(max));
        if (return_status > 0)
        {
            
            cout<<max<<endl;
        }
        else
        {
            printf("error");
        }
    }
    // Close the socket:

    close(socket_desc);

    return 0;
}