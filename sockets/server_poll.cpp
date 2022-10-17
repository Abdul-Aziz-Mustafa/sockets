#include <bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
#include <poll.h>

using namespace std;
#define MAX_CONNECTIONS 10
#define SERVERPORT 2002
#define BUFSIZE 4096
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;
void *handle_connection(int, char *);
int check(int exp, const char *msg);
pair<int, char *> accept_new_connection(int server_socket);
int setup_server(short port, int backlog);

long long factorial(unsigned int n)
{
    if (n == 0)
        return 1;
    int i = n;
    long long fact = 1;
    while (n / i != n)
    {
        fact = fact * i;
        i--;
    }
    return fact;
}

int main()
{
    map<int, char *> mp;

    struct sockaddr_in new_addr;
    int server_fd, ret_val, i;
    socklen_t addrlen;
    // char buf[DATA_BUFFER];
    int all_connections[MAX_CONNECTIONS];
    struct pollfd *pfds;
    int nfds = MAX_CONNECTIONS - 1, num_open_fds = nfds;
    struct pollfd pollfds[MAX_CONNECTIONS + 1];

    // get the socket server fd
    server_fd = setup_server(SERVERPORT, SERVER_BACKLOG);
    pollfds[0].fd = server_fd;
    pollfds[0].events = POLLIN;
    int useclient = 0;
    for (int i = 1; i < MAX_CONNECTIONS; i++)
    {
        pollfds[i].fd = 0;
        pollfds[i].events = POLLIN;
    }
    int buf;
    while (1)
    {
        
        ret_val = poll(pollfds, useclient + 1, 5000);

       
        if (ret_val >= 0)
        {
         
            if (pollfds[0].revents & POLLIN)
            {
                // accept the new connection
                printf("returned fd is %d(server 's fd)\n", server_fd);
                pair<int, char *> pr = accept_new_connection(server_fd);
                int new_fd = pr.first;
                char *client_socket_id = pr.second;
                // mp[new_fd] = client_socket_id;
                if (new_fd >= 0)
                {
                    printf("accepted a new connection with fd:,%d\n", new_fd);
                    for (int i = 1; i < MAX_CONNECTIONS; i++)
                    {
                        if (pollfds[i].fd == 0)
                        {
                            pollfds[i].fd = new_fd;
                            pollfds[i].events = POLLIN;
                            useclient++;
                            mp[i] = client_socket_id;
                            break;
                        }
                    }
                }
                else
                {
                    fprintf(stderr, "accepted failed [%s]\n", strerror(errno));
                }
                ret_val--;
                if (!ret_val)
                    continue;
            }

            // check if the fd with event is a non-server fd
            for (int i = 1; i < MAX_CONNECTIONS; i++)
            {
                if (pollfds[i].fd > 0 && pollfds[i].revents & POLLIN)
                {

                    // memset(buf,0,sizeof(buf));
                    // read incoming data
                    int bufsize = read(pollfds[i].fd, &buf, sizeof(buf));

                    // int bufsize=read(pollfds[i].fd,buf,DATA_BUFFER-1);
                    if (bufsize == -1)
                    {
                        pollfds[i].fd = 0;
                        pollfds[i].events = 0;
                        pollfds[i].revents = 0;
                        useclient--;
                    }
                    else if (bufsize == 0)
                    {
                        pollfds[i].fd = 0;
                        pollfds[i].events = 0;
                        pollfds[i].revents = 0;
                        useclient--;
                    }
                    else
                    {
                        FILE *fp = fopen("pollfile.txt", "a+");
                        long long send_it = factorial(buf);
                        string str = to_string(send_it);
                        char *cstr = new char[str.length() + 1];
                        strcpy(cstr, str.c_str());
                        fputs("\n", fp);
                        fputs(cstr, fp);
                        fputs(" ", fp);
                        fputs("  cliet_socket:-", fp);
                        fputs(mp[i], fp);
                        fclose(fp);
                        ret_val = write(pollfds[i].fd, &send_it, sizeof(send_it));
                    }
                }
            }
        }
        else
        {
            printf("failed to connect.\n");
        }
    }
}

int setup_server(short port, int backlog)
{
    int server_socket, client_socket, addr_size;
    SA_IN server_addr;
    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to create socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // fputs("", fp);

    check(bind(server_socket, (SA *)&server_addr, sizeof(server_addr)), "Bind Failed!");

    check(listen(server_socket, backlog), "[-]Error in Listening... :( \n");
    printf("[+]Listening.... :) \n");
    return server_socket;
}
pair<int, char *> accept_new_connection(int server_socket)
{
    int addr_size = sizeof(SA_IN);
    int client_socket;
    SA_IN client_addr;
    check(client_socket = accept(server_socket, (SA *)&client_addr, (socklen_t *)&addr_size), "accept failed");
    printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    FILE *fp = fopen("pollfile.txt", "a+");
    fputs("\n", fp);
    fputs("cliet_socket_ip:- ", fp);
    fputs(inet_ntoa(client_addr.sin_addr), fp);
    string str = to_string(ntohs(client_addr.sin_port));
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    fputs("\n", fp);
    fputs("cliet_socket:- ", fp);
    fputs(cstr, fp);
    fclose(fp);
    pair<int, char *> p = make_pair(client_socket, cstr);
    return p;
}
int check(int exp, const char *msg)
{
    if (exp == SOCKETERROR)
    {
        perror(msg);
        exit(1);
    }
    return exp;
}