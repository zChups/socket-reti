#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// personalised error function
void error(char* msg){
    perror(msg);
    exit(0);
}

// MAIN
/*
   hard-coded port number portno=2525
   dinamic servername argv[1] - first argument from command line
*/
int main(int argc, char* argv[]){
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    if (argc != 2){
        fprintf(stderr, "usage %s hostname\n", argv[0]);
        exit(0);
    }

    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd<0) error ("ERROR opening socket");

    portno= 2525;
    server = gethostbyname(argv[1]);



    if (server==NULL){
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char* )&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port=htons(portno);

    if (connect(sockfd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR connecting");

    //exercise start here ---



    return 0;
}