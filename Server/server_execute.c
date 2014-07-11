#include <stdio.h>
#include <string.h>
#include "server_execute.h"


const int DATA_SIZE = 256;
const int FLAG_SIZE = 4;

/******** EXECUTE() *********************
There is a separate instance of this function
for each connection.  It handles all communication
once a connnection has been established.
*****************************************/
void execute(int sock)
{
	const int LENGTH = FLAG_SIZE + DATA_SIZE;
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

void write_data(FILE *fr, int sock, char *buffer, const int length)
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

        bzero(buffer, length);
        /* begin receive */
        while (n = read(sock, buffer, sizeof(buffer)))
        {
            //printf("read: %s\n", buffer);
            if (n < 0)
            {
                error("ERROR reading from socket\n");
                break;
            }
  
            /* write data */
            fflush(fr);
            fwrite(buffer, sizeof(char), n, fr); /* write data to file */
        }

        printf("Ok received from client!\n");
        fclose(fr);
        //close(sock);
    }
}
