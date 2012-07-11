/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 4242

extern int axis[4];

void error(const char *msg)
{
    perror(msg);
    exit(1);
    
}

int socket_server_start()
{
     int sockfd, newsockfd;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     
     // Socket erstellen
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     
     // Fehler?
     if (sockfd < 0) 
        error("ERROR opening socket");
     
     
     bzero((char *) &serv_addr, sizeof(serv_addr));
     
     // Servereinstellungen
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(PORT);
     
     
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
     
     // Listenen
     listen(sockfd, 5);
     
     clilen = sizeof(cli_addr);
     
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     
     if (newsockfd < 0) 
          error("ERROR on accept");
     
     bzero(buffer,256);
     
     while(n = read(newsockfd,buffer,255)) {
     
        if (n < 0) 
            error("ERROR reading from socket");

        for(int i = 0; i < 4; i++) {

            axis[i] = buffer[i];

        }
     
     }
     
     printf("Blub\n");
     fflush(stdout);
     
     close(newsockfd);
     
     close(sockfd);
     
     return 0;
     
}