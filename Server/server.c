/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int LENGTH = 10000 + 256;
int SIZE_NAME = 256;

void dostuff(int); /* function prototype */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;

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
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) {
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     close(sockfd);
     return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
    int n;
    char buffer[LENGTH];
    bzero(buffer, LENGTH); 
    n = read(sock,buffer,LENGTH); /* read stream upload to buffer */
    

    char file_name[SIZE_NAME]; /* name file upload */
    strncpy(file_name, buffer, SIZE_NAME);

 /* Receive File from Client */
    printf("[Client] Receiveing file from Client and saving it as %s...", file_name);
    
    char* fr_name = "/home/viettd/host/"; /* dir to file */
    strcat (fr_name, file_name);

    FILE *fr = fopen(fr_name, "w");
    if(fr == NULL)
        printf("File %s Cannot be opened.\n", fr_name);
    else
    {
        if (n < 0) error("ERROR reading from socket");
        fwrite(buffer + SIZE_NAME, sizeof(char), n - SIZE_NAME, fr); /* write data to file */
        n = write(sock,"I got your file",16);
        if (n < 0) error("ERROR writing to socket");
        
        printf("Ok received from client!\n");
        fclose(fr);
    }
}