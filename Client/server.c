/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>

using namespace std;


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[259];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
    while(true){
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, 
                    (struct sockaddr *) &cli_addr,     
                     &clilen);
        if (newsockfd < 0) 
              error("ERROR on accept");
        int rev=0;
        while(0 == rev){
            bzero(buffer,259);
            n = read(newsockfd,buffer,258);
            if (n < 0) error("ERROR reading from socket");
            printf("Here is the message: %s\n",buffer);
            cout <<"Size of buffer : " << sizeof(buffer)<< endl;
            if(strlen(buffer) ==0){
                rev=1;
            }
        }
        cout << "Da nhan xong !" << endl;
        n = write(newsockfd,"I got your message",18);
         if (n < 0) error("ERROR writing to socket");
    }
     close(newsockfd);
     close(sockfd);
     return 0; 
}
