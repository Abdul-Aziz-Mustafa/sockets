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
void *handle_connection(int , char*);
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
    pid_t childpid;

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
      
        handle_connection(client_socket, client_socket_id);
          int pid=fork();
        if(pid==0){
            cout<<"main"<<endl;
        }
   
        // if ((childpid = fork()) == 0)
        // {
            // return 0;
        // }
        
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
    FILE *fp = fopen("testfork.txt", "a+");
    check(client_socket = accept(server_socket, (SA *)&client_addr, (socklen_t *)&addr_size), "accept failed");
    printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    fputs("\n", fp);
    
    fputs("ip Adder:- ", fp);
    fputs(inet_ntoa(client_addr.sin_addr), fp);
    string str = to_string(ntohs(client_addr.sin_port));
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    fputs("\n", fp);
    fputs("clientscoket:- ", fp);
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
    while (k >= 0)
    {
        bytes_read = read(client_socket, &buffer, sizeof(buffer));
        check(bytes_read, "recv error");
        cout << buffer << endl;
        FILE *fp = fopen("testfork.txt", "a+");
        long long send_it = factorial(buffer);
        string str = to_string(send_it);
        char *cstr = new char[str.length() + 1];
        strcpy(cstr, str.c_str());
        fputs("\n", fp);
        fputs(cstr, fp);
        fputs(" ", fp);
        fputs("clientscoket:- ", fp);
        fputs(client_socket_id, fp);
        fclose(fp);
        write(client_socket, &send_it, sizeof(send_it));
        k--;
    }
    close(client_socket);
    // fclose(fp);
    printf("closing connection\n");
    // return NULL;
    return NULL;
}