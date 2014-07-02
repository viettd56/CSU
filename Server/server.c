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

const int NAME_SIZE = 256;
const int FLAG_SIZE = 3;
const int LENGTH = FLAG_SIZE + NAME_SIZE;


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
    if (n < 0) error("ERROR reading from socket");
	
	char cFLAG[FLAG_SIZE];
	strncpy(cFLAG, buffer, FLAG_SIZE);
	const int flag = atoi(cFLAG);
	
    char file_name[NAME_SIZE]; /* name file upload */
    strncpy(file_name, buffer + 3, NAME_SIZE);

 /* Receive File from Client */
    printf("[Client] Receiveing file from Client and saving it as %s...", file_name);
    
    char* fr_name = "/home/viettd/host/"; /* dir to file */
    strcat (fr_name, file_name);
	
	if (flag == 200) FILE *fr = fopen(fr_name, "wb");
	else if (flag == 201){
		FILE *fr = fopen(fr_name, "ab");
		int sz = ftell(fr); /* file's size */
		
		cFLAG = "202";
		char msg[LENGTH];
		strncpy(msg, cFLAG, FLAG_SIZE);
		
		char cSz[NAME_SIZE];
		itoa(cSz, sz, 10);
		strncpy(msg, cSz, sizeof(cSz));
		
		n = write(sock,msg,sizeof(msg));
		if (n < 0) error("ERROR writing to socket");
        
	} 
	
    if(fr == NULL)
        printf("File %s Cannot be opened.\n", fr_name);
    else
    {
		/* first buffer */
		n = read(sock,buffer,LENGTH);
		if (n < 0) error("ERROR reading from socket");
		
		/* get flag */
		char cFLAG[FLAG_SIZE];
		strncpy(cFLAG, buffer, FLAG_SIZE);
		const int flag = atoi(cFLAG);
        
		if (flag != 400){
			error("Error first buffer");
		}
		
		/* write first data */
		fflush(fr);
        fwrite(buffer + FLAG_SIZE, sizeof(char), n - FLAG_SIZE, fr); /* write data to file */
        
		while (n = read(sock,buffer,LENGTH)){
			/* write next data */
			fflush(fr);
			fwrite(buffer, sizeof(char), n, fr); /* write data to file */
		}
		
		n = write(sock,"I got your file",16);
        if (n < 0) error("ERROR writing to socket");
        
        printf("Ok received from client!\n");
        fclose(fr);
    }
}