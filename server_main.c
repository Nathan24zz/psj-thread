#include "server.h"

int main(const int argc, char *argv[])
{
    signal(SIGINT, sigint_handler);
    if (argc != 2)     /* Test for correct number of arguments */
    {
        fprintf(stderr,"Usage:  %s <SERVER PORT>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* First arg:  local port */

    /* Create socket for incoming connections */
    servSock = CreateTCPServerSocket(echoServPort);
    jumlahthread = 0;

    for (;;)
    {
	    clntSock = AcceptTCPConnection(servSock);

        /* Create separate memory for client argument */
        if ((threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) == NULL)
            DieWithError("malloc() failed");

        threadArgs -> clntSock = clntSock;
	    jumlahthread = jumlahthread +1;

        /* Create client thread */
        if (pthread_create(&threadID, NULL, ThreadMain, (void *) threadArgs) != 0)
            DieWithError("pthread_create() failed");

        printf("with thread %ld\n", (long int) threadID);
        printf("jumlah thread %d\n", jumlahthread);
        printf("port %d\n", echoServPort);
    }

}