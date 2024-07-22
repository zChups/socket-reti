#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char **argv){
    int sockfd, newsockfd, portno;
    char buffer[255];
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("ERROR opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 1025;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        error("ERROR on binding");
    }
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while(1){
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0){
                error("ERROR on accept");
            }
            n = read(newsockfd, buffer, 255);
            if (n < 0){error("ERROR reading from socket");}
            int length = (int)buffer[1];
            unsigned char *plain = malloc(length*sizeof(unsigned char));
            memcpy(plain,&buffer[2],length);
            unsigned char K[] = {0xDE, 0xAD, 0xBE, 0xEF};
            unsigned char *E = malloc(length*sizeof(unsigned char));
            for(int i = 0; i < length; i++){
                E[i] = plain[i] ^ K[i % sizeof(K)];
            }
            char resp[255];
            resp[0] = 'E';
            resp[1] = length;
            memcpy(&resp[2],&E[0],length);
            n = write(newsockfd, resp, length+2);
            if (n < 0) {
                error("ERROR writing to socket");
            }   
            free(plain);
            free(E);
    }
    return 0;
}