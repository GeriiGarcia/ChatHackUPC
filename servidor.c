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


/*  Basicametn ha d'enviar d'un client a l'altre els missatges
    Els client li enviaran la ip a la que volen conectar-se al servidor i el servidor gestionará.

    Array dinàmic que guardi Clients (nom, ip, (contra))
    Si un usuari es conecta amb una altre ip, la ip del usuari canvia.

    El servidor ha de interpretar comandes que li passi el client amb el format:    ::function


    Serveis del servidor als clients:
        - Mostrar menú usuaris disponibles
        - conectar a un usuari
        - Enviar missatges entre usuaris
        - Acabar conexió amb usuari
        - Desconectarse del servidor
        - 
    
    Funcionament del Servidor*/


#define SERVER_PORT 8081


int main(int argc, char **argv)
{





}