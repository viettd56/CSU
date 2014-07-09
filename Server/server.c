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


void dostuff(int, int); /* function prototype */
void write_data(FILE *fr, int sock, char *buff, const int LENGTH);
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    const int LENGTH = FLAG_SIZE + DATA_SIZE;
    int sockfd, newsockfd, portno, pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    portno = atoi(argv[1]);


    /* server */

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
    listen(sockfd, 5);

    clilen = sizeof(cli_addr);
    while (1)
    {
        newsockfd = accept(sockfd,
                           (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");
        pid = fork();
        if (pid < 0)
            error("ERROR on fork");
        if (pid == 0)
        {
            close(sockfd);
            dostuff(newsockfd, LENGTH);
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
void dostuff (int sock, const int LENGTH)
{
    int n;
    char buffer[LENGTH];
    bzero(buffer, LENGTH);
    while (1)
    {
        bzero(buffer, LENGTH);
        n = read(sock, buffer, LENGTH); /* read stream upload to buffer */
        if (n < 0) error("ERROR reading from socket");

        char cFlag[FLAG_SIZE]; //Command
        strncpy(cFlag, buffer, FLAG_SIZE);

        //char file_name[DATA_SIZE]; /* name file upload */
        //strncpy(file_name, buffer + FLAG_SIZE + 1, n - 1 - FLAG_SIZE);

        char fr_name[256];
        bzero(fr_name, sizeof(fr_name));
        strcat (fr_name, ""); /* dir to file */
        strcat (fr_name, buffer + FLAG_SIZE + 1);

        printf("Flag: %s\n", cFlag);
        /* if upload new file */
        if (strcmp(cFlag, "STOR") == 0)
        {
            FILE *fr = fopen(fr_name, "wb");
            printf("STOR\n");
            write_data(fr, sock, buffer, LENGTH);
        }
        /* if resume upload */
        else if (strcmp(cFlag, "APPE") == 0)
        {
            FILE *fr = fopen(fr_name, "ab");
            printf("APPE\n");
            write_data(fr, sock, buffer, LENGTH);
        }
        /* check size of file */
        else if (strcmp(cFlag, "SIZE") == 0)
        {
            FILE *fr = fopen(fr_name, "rb");
            printf("file name:%s\n", fr_name);
            // If file not exist, return size 0
            if (fr == NULL)
            {
                fprintf(stderr, "File unavailable.\n");
                // char *msg = "550 Requested action not taken. File unavailable.";
                // printf("%s", msg);
                // n = write(sock, msg, strlen(msg));
                // if (n < 0) error("ERROR writing to socket");
                char *sz = "0";
                n = write(sock, sz, strlen(sz));
                if (n < 0) error("ERROR writing to socket");
                continue;
            }

            //get file's size
            fseek(fr, 0, SEEK_END);
            int sz = ftell(fr);
            char cSz[DATA_SIZE];
            snprintf(cSz, DATA_SIZE, "%d", sz);

            //Sent file's size to client

            n = write(sock, cSz, strlen(cSz));
            printf("file size: %s\n", cSz);
            if (n < 0) error("ERROR writing to socket");
        }
        // Quit
        else if (strcmp(cFlag, "QUIT") == 0)
        {
            //char *msg = "221 Service closing control connection.";
            char *msg = "221";
            n = write(sock, msg, strlen(msg));
            if (n < 0) error("ERROR writing to socket");
            return;
        }
        // not implement
        else
        {
            //char *msg = "500 Syntax error, command unrecognized.";
            char *msg = "500";
            n = write(sock, msg, strlen(msg));
            //printf("n = %d\n", n);
            if (n < 0) error("ERROR writing to socket");
            continue;
        }
    }
}

void write_data(FILE *fr, int sock, char *buffer, const int LENGTH)
{
    int n;
    if (fr == NULL)
    {
        /* cannot open file */
        fprintf(stderr, "Cannot open file.\n");
        //char *msg = "553 Requested action not taken. File name not allowed.";
        char *msg = "553";
        printf("%s", msg);
        n = write(sock, msg, strlen(msg));
        if (n < 0) error("ERROR writing to socket");
    }
    else
    {
        //char *msg125 = "125 Data connection already open; transfer starting.";
        char *msg125 = "125";
        n = write(sock, msg125, strlen(msg125));
        if (n < 0) error("ERROR writing to socket");

        /* Receive File from Client */
        printf("[Client] Receiveing file from Client and saving it\n");

        bzero(buffer, LENGTH);
        /* begin receive */
        while (n = read(sock, buffer, sizeof(buffer)))
        {
            //printf("read: %s\n", buffer);
            if (n < 0)
            {
                error("ERROR reading from socket\n");
                break;
            }
            // printf("get: %s\n", buffer);
            // //Check end of file
            // if (strstr(buffer, "\r\n") != NULL)
            // {
            //     printf("end file");
            //     break;
            // }

            /* write data */
            fflush(fr);
            fwrite(buffer, sizeof(char), n, fr); /* write data to file */
        }

        //char *msg226 = "226 Closing data connection. Requested file action successful ";
        // char *msg226 = "226";
        // n = write(sock, msg226, strlen(msg226));
        // if (n < 0) error("ERROR writing to socket");

        printf("Ok received from client!\n");
        fclose(fr);
        //close(sock);
    }
}
