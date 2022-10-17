#include <bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
using namespace std;

#define SERVERPORT 2002
#define BUFSIZE 4096
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;
// prototypes
void *handle_connection(void *);
int check(int exp, const char *msg);
pair<int, char *> accept_new_connection(int server_socket);
int setup_server(short port, int backlog);

// #define MAX 500
typedef struct sargs
{
    int client_socket;
    char *id;
} args;

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

    int server_socket = setup_server(SERVERPORT, SERVER_BACKLOG);
    while (true)
    {
        // if ((childpid = fork()) == 0)
        // {
        printf("Waiting for connections ...\n");
        // wait for, and eventually accept an incoming connection

        pair<int, char *> pr = accept_new_connection(server_socket);
        int client_socket = pr.first;
        char *client_socket_id = pr.second;
        args a1;
        a1.client_socket = client_socket;
        a1.id = client_socket_id;
        // fclose(fp);
        pthread_t t;
        int *pclient = new int;
        *pclient = client_socket;
        pthread_create(&t, NULL, handle_connection, &a1);

        // pthread_join(t, NULL);
        // handle_connection(client_socket, fp, client_socket_id);
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
    FILE *fp = fopen("threadfile.txt", "a+");
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
void *handle_connection(void *arg)
{
    // char buffer[BUFSIZE];
    // args*)input)->age
    int client_socket = ((args *)arg)->client_socket;
    char *client_socket_id = ((args *)arg)->id;
    cout << client_socket_id << "LOL" << endl;
    // free(arg);
    int buffer;
    size_t bytes_read;
    int msgsize = 0;

    int k = 20;
    while (k >= 0)
    {
        bytes_read = read(client_socket, &buffer, sizeof(buffer));
        check(bytes_read, "recv error");
        cout << buffer << endl;
        FILE *fp = fopen("threadfile.txt", "a+");
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
    }

    close(client_socket);
    // fclose(fp);
    printf("closing connection\n");
    // return NULL;
    return NULL;
}