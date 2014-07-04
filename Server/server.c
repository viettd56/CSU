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

const int DATA_SIZE = 256;
const int FLAG_SIZE = 4;
const int LENGTH = FLAG_SIZE + DATA_SIZE;
const int PORT_STOR = 20;


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
	
	/* server control */
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	error("ERROR opening socket server control");
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
	while(1)
	{
		n = read(sock,buffer,LENGTH); /* read stream upload to buffer */
		if (n < 0) error("ERROR reading from socket");
		
		char cFlag[FLAG_SIZE];
		strncpy(cFlag, buffer, FLAG_SIZE);
		
		char file_name[DATA_SIZE]; /* name file upload */
		strncpy(file_name, buffer + FLAG_SIZE, DATA_SIZE);

		
		char* fr_name = ""; /* dir to file */
		strcat (fr_name, file_name);
		
		/* if upload new file */
		if (strcmp(cFlag, "STOR")){
			FILE *fr = fopen(fr_name, "wb");
			writeData(fr, sock);
		} 
		/* if resume upload */
		else if (strcmp(cFlag, "APPE")){
			FILE *fr = fopen(fr_name, "ab");
			writeData(fr, sock);
		}
		/* check size of file */
		else if (strcmp(cFlag, "SIZE")){
			FILE *fr = fopen(fr_name, "ab");
			
			int sz = ftell(fr);
			char cSz[DATA_SIZE];
			itoa(cSz, sz, 10);
			char *msg = "";
			strncpy(msg, cSz, sizeof(cSz));
			
			n = write(sock,msg,sizeof(msg));
			if (n < 0) error("ERROR writing to socket");
		}
		else if (strcmp(cFlag, "QUIT")){
			char *msg = "221 Service closing control connection.";
			n = write(sock,msg,sizeof(msg));
			if (n < 0) error("ERROR writing to socket");
			return;
		} 	
		else {
			char *msg = "500 Syntax error, command unrecognized.";
			n = write(sock,msg,sizeof(msg));
			if (n < 0) error("ERROR writing to socket");
			continue;
		}
	}
}

void writeData(FILE fr, int sock){
	if(fr == NULL){
		/* cannot open file */
		printf("File %s Cannot be opened.\n", fr_name);
		char *msg = "553 Requested action not taken. File name not allowed.";
		n = write(sock,msg,sizeof(msg));
		if (n < 0) error("ERROR writing to socket");
	}
	else
	{
		char *msg = "125 Data connection already open; transfer starting.";
		n = write(sock,msg,sizeof(msg));
		if (n < 0) error("ERROR writing to socket");
		
		/* Receive File from Client */
		printf("[Client] Receiveing file from Client and saving it as %s...", file_name);
		
		
		/* begin receive */
		while (n = read(sock,buffer,LENGTH)){
			/* write data */
			fflush(fr);
			fwrite(buffer, sizeof(char), n, fr); /* write data to file */
		}
		
		char *msg = "250 Requested file action okay, completed.";
		n = write(sock,msg,sizeof(msg));
		if (n < 0) error("ERROR writing to socket");
		
		printf("Ok received from client!\n");
		fclose(fr);
	}
}