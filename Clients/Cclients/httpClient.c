#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// personalised error function
void error(char *msg){
    perror(msg);
    exit(0);
}

// http sender
void send_http_request(int sockfd, char* pathname, char * hostname){
    char reqbuffer[256];
    int n;

    bzero (reqbuffer, 256);
    sprintf(reqbuffer, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", pathname, hostname);
    n=write(sockfd, reqbuffer, strlen(reqbuffer));
    if (n<0) error("ERROR writing to socket");
}

// http reciever
void read_http_response(int sockfd){
    char respbuffer[256];
    bzero(respbuffer, 256);

    read (sockfd, respbuffer, 255);
    printf("%s\n", respbuffer);

}

// MAIN
/*
   hard-coded port number portno=8080
   dinamic http request/reply pathname=argv[1] - first argument from command line
*/
int main(int argc, char* argv[]){
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char *hostname = "localhost";
    char * pathname;

    if (argc < 2){
        fprintf(stderr, "usage %s URL\n", argv[0]);
        exit(0);
    }
    pathname=argv[1];
    portno=8080;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {error("ERROR opening socket");}
    server = gethostbyname(hostname);
    if (server==NULL){
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family =AF_INET;
    bcopy((char*) server->h_addr,
            (char*) &serv_addr.sin_addr.s_addr,
            server->h_length);

    //from host-byte to network-byte communication
    serv_addr.sin_port=htons(portno);
    if (connect(sockfd,(const struct sockaddr* ) & serv_addr, sizeof(serv_addr)) < 0) error ("ERROR connecting");

    send_http_request(sockfd,pathname, hostname);
    read_http_response(sockfd);
    return 0;
}