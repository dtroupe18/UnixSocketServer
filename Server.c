#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

char* stringRotate (int offset, size_t size, const char *inStr);

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    // call the socket function
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // initialize socket structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5001;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // now bind the host address
    if (bind(sockfd, (struct sockaddr *) & serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error on binding");
        exit(1);
    }

    // now we can start listening for clients
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // accept connections
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("Error on accept");
        exit(1);
    }

    // if connection is established then start communicating
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);

    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    }

    printf("Here is the message: %s\n", buffer);



    // write a response to the client
    n = write(newsockfd,"I got your message",18);

    if (n < 0) {
        perror("Error writing to socket");
        exit(1);
    }

    return 0;
}

char* stringRotate (int offset, size_t size, const char *inStr) {
    char *result = (char *)malloc(size * sizeof(char));

    int trueOffset = size - (offset % size);

    int inIndex = trueOffset;
    int outIndex = 0;

    for (inIndex = trueOffset; inIndex < (int)size; inIndex++, outIndex++) {
        result[outIndex] = inStr[inIndex];
    }
    for (inIndex = 0; inIndex < trueOffset; inIndex++, outIndex++) {
        result[outIndex] = inStr[inIndex];
    }

    result[(int)size] = '\0';

    return result;
}