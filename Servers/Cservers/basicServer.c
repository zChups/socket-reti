#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


void error(char* msg){
    perror(msg);
    exit(1);
}

/*
   hard-coded port number portno=2525
   no inputs
*/
int main(int argc, char* argv[]){
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc!=1){
        fprintf(stderr, "ERROR, no port provided");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd<0){
        error("ERROR opening socket");
    }


    bzero((char* )&serv_addr, sizeof(serv_addr));

    portno=2525;

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port=htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");
    listen (sockfd, 5);

    clilen=sizeof(cli_addr);
    newsockfd= accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd<0){
        error("ERROR on accept");
    }


    //exercise start here ---
    return 0;
}