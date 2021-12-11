#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <pthread.h>        /* for POSIX threads */
#include <stdbool.h>
#include <signal.h>
#include <time.h>

#define MAXPENDING 5    /* Maximum outstanding connection requests */
#define RCVBUFSIZE 1000   /* Size of receive buffer */

/* Structure of arguments to pass to client thread */
struct ThreadArgs
{
    int clntSock;
};

int servSock;                    /* Socket descriptor for server */
int clntSock;                    /* Socket descriptor for client */
unsigned short echoServPort;     /* Server port */
pthread_t threadID;              /* Thread ID from pthread_create() */
struct ThreadArgs *threadArgs;   /* Pointer to argument structure for thread */
char jumlahthread;
int is_exit;

int AcceptTCPConnection(int servSock);
int CreateTCPServerSocket(unsigned short port);
void DieWithError(char *errorMessage);
void HandleTCPClient(int clntSocket);
void *ThreadMain(void *arg);
void sigint_handler(int sig);

#endif