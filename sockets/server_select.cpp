#include <bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
using namespace std;
#define MAX_CONNECTIONS 10
#define SERVERPORT 2002
#define BUFSIZE 4096
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;
// prototypes
void *handle_connection(int, char *);
int check(int exp, const char *msg);
pair<int, char *> accept_new_connection(int server_socket);
int setup_server(short port, int backlog);

// #define MAX 500

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

int main(int argc, char **argv)
{
    map<int, char *> mp;
    
    int server_socket = setup_server(SERVERPORT, SERVER_BACKLOG);
    int all_connections[MAX_CONNECTIONS];
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        all_connections[i] = -1;
        mp[i]="-1";
    }
    int buffer;
    all_connections[0] = server_socket;
    fd_set current_sockets, ready_sockets, read_fd_set;
    // FD_ZERO(&current_sockets);               // initializing with 0
    // FD_SET(server_socket, &current_sockets); // adding serversock to current_scoks list
    while (true)
    {

        FD_ZERO(&read_fd_set);
        for (int i = 0; i < MAX_CONNECTIONS; i++)
        {
            if (all_connections[i] >= 0)
            {
                FD_SET(all_connections[i], &read_fd_set);
            }
        }
        // ready_sockets = current_sockets
        int ret_val = select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL);
        if (ret_val < 0)
        {
            perror("select error");
            exit(EXIT_FAILURE);
        }

        // for (int i = 0; i < FD_SETSIZE; i++)
        // {
        if (FD_ISSET(server_socket, &read_fd_set))
        {

            // this is new connection
            // int client_socket = accept_new_connection(server_socket);
            pair<int, char *> pr = accept_new_connection(server_socket);
            int new_fd = pr.first;
            char *client_socket_id = pr.second;
            // mp[new_fd] = client_socket_id;
            // cout<<mp[new_fd]<<"lol"<<typeid(new_fd).name()<<endl;

            // FD_SET(new_fd, &current_sockets);
            if (new_fd >= 0)
            {
                printf("Accepted a new connection with fd :%d\n", new_fd);
                for (int i = 0; i < MAX_CONNECTIONS; i++)
                {
                    if (all_connections[i] < 0)
                    {
                        all_connections[i] = new_fd;
                        mp[i] = client_socket_id;
                        break;
                    }
                }
            }
            else
            {
                fprintf(stderr, "accpet failed[%s]\n", strerror(errno));
            }
            ret_val--;
            if (!ret_val)
                continue;
        }
        for (int i = 1; i < MAX_CONNECTIONS; i++)
        {
            // cout<<mp[i]<<"kk"<<endl;
            if ((all_connections[i] > 0) && (FD_ISSET(all_connections[i], &read_fd_set)))
            {
                
                
                
                
                ret_val= read(all_connections[i], &buffer, sizeof(buffer));
                check(ret_val, "recv error");

                if (ret_val == 0)
                {
                    printf("closing connection for fd%d\n", all_connections[i]);
                    close(all_connections[i]);
                    all_connections[i] = -1;
                    // connection is now closed
                }
                if (ret_val > 0)
                {
                    FILE *fp = fopen("select.txt", "a+");
                    long long send_it = factorial(buffer);
                    string str = to_string(send_it);
                    char *cstr = new char[str.length() + 1];
                    strcpy(cstr, str.c_str());
                    fputs("\n", fp);
                    fputs(cstr, fp);
                    fputs(" ", fp);
                    fputs("  cliet_socket:-", fp);
                    // cout<<i<<mp[i]<<"kkkk"<<endl;
                    fputs(mp[i], fp);
                    fclose(fp);
                    ret_val=write(all_connections[i], &send_it, sizeof(send_it));
                    
                    // printf("Received data(len %d bytes fd: %d):%s\n", ret_val, all_connections[i], buf);
                }
                if (ret_val == -1)
                {
                    printf("recv() failed for fd: %d[%s]\n", all_connections[i], strerror(errno));
                    break;
                }
            }
            ret_val--;
            if (!ret_val)
                continue;
        }

        // }
        // wait for, and eventually accept an incoming connection
    }
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (all_connections[i] > 0)
        {
            close(all_connections[i]);
        }
    }
    return 0;
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
    FILE *fp = fopen("select.txt", "a+");
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
void *handle_connection(int client_socket, char *client_socket_id)
{
    // char buffer[BUFSIZE];
    int buffer;
    size_t bytes_read;
    int msgsize = 0;
    // char actualpath[PATH_MAX + 1];
    // read the client's message the name of the file to read
    int k = 20;
    // while (k >= 0)
    // {

    bytes_read = read(client_socket, &buffer, sizeof(buffer));
    check(bytes_read, "recv error");
    cout << buffer << endl;
    FILE *fp = fopen("select.txt", "a+");
    long long send_it = factorial(buffer);
    string str = to_string(send_it);
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    fputs("\n", fp);
    fputs(cstr, fp);
    fputs(" ", fp);
    fputs("  cliet_socket:-", fp);
    fputs(client_socket_id, fp);
    write(client_socket, &send_it, sizeof(send_it));
    fclose(fp);
    k--;
    // }
    close(client_socket);
    // fclose(fp);
    printf("closing connection\n");
    // return NULL;
    return NULL;
}