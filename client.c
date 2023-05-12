#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <poll.h>

#define SERVER_PORT 8081

int main(int argc, char **argv)
{

    // creem el socket, la adreça i ens conectem al servidor
    // create soket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		perror("[-]socket create error");
		return 1;
	}
	
    // server adress
	struct sockaddr_in addrServidor;
	addrServidor.sin_family = AF_INET;
	addrServidor.sin_addr.s_addr = INADDR_ANY; 
	addrServidor.sin_port = SERVER_PORT;
	
    // connect to server
	if(connect(sock, (struct sockaddr *) &addrServidor, sizeof(addrServidor)) == -1){
		perror("[-]Error conectant");
		close(sock);
		return 1;
	} 


    //

    //

    //




    return 0;
}