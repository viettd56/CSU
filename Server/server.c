/* A simple server in the internet domain using TCP
The port number is passed as an argument
This version runs forever, forking off a separate
process for each connection
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int init_server(char *addr, const int port)
{
    const int LENGTH = FLAG_SIZE + DATA_SIZE;
    int sockfd, newsockfd, portno, pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    /* init server */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket server control");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = port;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);

    clilen = sizeof(cli_addr);

        newsockfd = accept(sockfd,
                           (struct sockaddr *) &cli_addr, &clilen);
    return newsockfd;
}

