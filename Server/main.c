#include <stdio.h>
#include "server.h"

int main(int argc, char *argv[])
{
	int newsockfd;
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