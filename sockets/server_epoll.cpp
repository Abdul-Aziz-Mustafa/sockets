
#include <bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
#include <poll.h>
#include <sys/epoll.h>

using namespace std;
#define MAX_CONNECTIONS 2
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

int main()
{
    int buf;
    int server_add = setup_server(SERVERPORT, SERVER_BACKLOG);
    struct epoll_event events[5];
    int epfd = epoll_create(10);
    map<int,char*> mp; 
    for (int i = 0; i < 10; i++)
    {
        static struct epoll_event ev;
        pair<int,char*> el = accept_new_connection(server_add);
        ev.data.fd=el.first;
        mp[ev.data.fd]=el.second;
        ev.events = EPOLLIN;
        epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
    }
    while (1)
    {
        puts("round again");
        int nfds = epoll_wait(epfd, events, 5, 10000);

        for (int i = 0; i < nfds; i++)
        {
            FILE *fp = fopen("epoll_file.txt", "a+");
            int bufsize = read(events[i].data.fd, &buf, sizeof(buf));
            long long send_it = factorial(buf);
            string str = to_string(send_it);
            char *cstr = new char[str.length() + 1];
            strcpy(cstr, str.c_str());
            fputs("\n", fp);
            fputs(cstr, fp);
            fputs(" ", fp);
            fputs("  cliet_socket:-", fp);
            cout<<send_it<<endl;
            fputs(mp[events[i].data.fd], fp);
            fclose(fp);

            write(events[i].data.fd, &send_it, sizeof(send_it));
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
    FILE *fp = fopen("epoll_file.txt", "a+");
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