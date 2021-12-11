#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <unistd.h> // write(), read(), close()
#define SIZE 20

void send_data(char *buf, int sockfd){
  send(sockfd, buf, SIZE, 0);
}

int main(int argc, char *argv[]){
  char * client_name;
  const char * serial_port_name = "/dev/ttyUSB0";
  char buf[SIZE]= {0};
  int serial_port, data; 
  char *ip = "127.0.0.1";
  int port;
  int e;

   if (argc != 3)     /* Test for correct number of arguments */
    {
        fprintf(stderr,"Usage:  %s <SERVER PORT> <client_name>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);  /* First arg:  local port */
    client_name = argv[2];

  int sockfd;
  struct sockaddr_in server_addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("Error in socket");
    exit(1);
  }
  printf("Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e == -1) {
    perror("Error in socket");
    exit(1);
  }
    printf("Connected to Server.\n");

  serial_port = open(serial_port_name, O_RDWR);
  send_data(client_name, sockfd);

  while (serial_port > 0) 
  {
      data = read(serial_port, buf, SIZE);
      if (data > 3)
      {
          buf[data] = '\0';
          send_data(buf, sockfd);
          printf("%s send: %s\n", client_name, buf);
      }
  }
  printf("Closing the connection.\n");
  close(sockfd);

  return 0;
}