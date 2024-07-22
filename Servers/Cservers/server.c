#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void error(char* str) {
    perror(str);
    exit(1);
}

unsigned int ip2uint(char *ip){
    unsigned int ip3, ip2, ip1, ip0;
    sscanf(ip, "%d.%d.%d.%d", &ip3, &ip2, &ip1, &ip0);
    return (ip3<<24) + (ip2<<16) + (ip1<<8) + ip0;
}

void handle_request(int cli_socket, struct sockaddr_in client_addr) {
    char buff[255] = {0};
    int size = read(cli_socket, buff, 255);
    if (size < 0) {
        error("ERROR on read");
        return;
    }

    int ip_size = buff[1];

    unsigned int query_ip = ip2uint(buff + 2);
    
    char resp[6] = { 0 };
    resp[0] = 'R';
    resp[1] = 4;

    query_ip = htonl(query_ip);
    memcpy(resp + 2, &query_ip, 4);

    if(write(cli_socket, resp, 6) < 0) {
        error("ERROR on write");
        return;
    }
}

int main() {
    int port = 1025;

    int s = socket(AF_INET, SOCK_STREAM, 0);    //tcp
    if (s < 0) {
        error("ERROR while opening socket");
    }
    
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(s, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        error("ERROR on binding");
    }

    listen(s, 5);

    while (1) {
        int cli_len = sizeof(client_addr);
        int cli_socket = accept(s, (struct sockaddr*)&client_addr, &cli_len);
        if (cli_socket < 0) {
            error("ERROR on accept");
        }
        handle_request(cli_socket, client_addr);    //single process
    }
}