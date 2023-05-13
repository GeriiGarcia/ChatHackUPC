#include "funcions.h"

#define SERVER_PORT 8081
const int MSG_SIZE = 50000;


/*  Basicament ha d'enviar d'un client a l'altre els missatges
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


int main(int argc, char **argv)
{

    long unsigned int fd_size = 4096;	    //Quants sockets pot arribar a tenir el poll() simultaniament
    int fd_count = 0;                       //Quants sockets te el poll() simultaneament

    dadesClient *dadesClients = (dadesClient*)malloc(sizeof *dadesClients * fd_size);
    memset(dadesClients, 0, sizeof *dadesClients * fd_size);

    int numChats = 0;
    chat *chats = (chat*)malloc(sizeof *chats * fd_size/2);
    memset(chats, 0, sizeof *chats * fd_size/2);

    Type Type_demanat[fd_size];
    memset(Type_demanat, 0, sizeof *Type_demanat * fd_size);
	
	
	struct pollfd *pfds = (pollfd*)malloc(sizeof *pfds * fd_size);
	memset(pfds, 0, sizeof *pfds * fd_size);
	
	int socket_server;
	struct sockaddr_in socket_server_adress;		
	socket_server = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_server < 0){
        perror("Error socket en servidor\n");
		close(socket_server);
        return 0;
    }
	
	int flags = fcntl(socket_server, F_GETFL, 0);
	if(fcntl(socket_server, F_SETFD, flags | O_NONBLOCK) == -1){ 
		perror("Socket making non-block error");
		close(socket_server);
		return 1;
	}	
	
	socket_server_adress.sin_family = AF_INET;
    socket_server_adress.sin_addr.s_addr = INADDR_ANY;
	socket_server_adress.sin_port = htons(((u_int16_t)8080));

	if (bind(socket_server, (struct sockaddr*) &socket_server_adress, sizeof(socket_server_adress)) < 0)
	{
		perror("---------------------------Bind: que pasa con el:");
		close(socket_server);
		return 0;
	}
	if (listen(socket_server, 4096) == -1) 
	{
		perror("Que pasa con el listen bro?");
		close(socket_server);
        return 0;
    }

	
	pfds[0].fd = socket_server;
    pfds[0].events = POLLIN; // Report ready to read on incoming connection
    fd_count = 1;

    for(;;)
    {
        int poll_count = poll(pfds, (nfds_t)fd_count, -1);

        if (poll_count == -1) {
            perror("poll");
			close(socket_server);
            return 0;
        }

        for (int i = 0; i < fd_count; i++)  //// Run through the existing connections looking for data to read
		{

			if (pfds[i].revents == 0)
      		{
			  printf("  Error revents = %d\n", pfds[i].revents);
      		  continue;
      		}

            struct sockaddr_in clientAddr;
            socklen_t addrLen = sizeof(clientAddr);
            char ip[INET_ADDRSTRLEN];
            string nom = "";

            // Check if someone's ready to read
        	if (pfds[i].revents & POLLIN && pfds[i].fd == socket_server) // We got one!! 
            {

                int newfd;

                int addr_len = sizeof(socket_server_adress);
				newfd = accept(socket_server, (struct sockaddr*) &socket_server_adress, (socklen_t *__restrict__) &addr_len);

                if (newfd == -1) 
				{
        	        perror("accept");
					close(socket_server);
					return 0;
        	    }
				else 
                {
                    pfds[fd_count].fd = newfd;                 

                    if (inet_ntop(AF_INET, &(clientAddr.sin_addr), ip, INET_ADDRSTRLEN) == NULL) {
                        perror("Error al convertir la dirección IP");
                        exit(EXIT_FAILURE);
                    }

					//server nonBloking // crec que aixo pot sobrar per que el nou socket no te per que ser non blocking si no espera res
					/*flags = fcntl(pfds[fd_count].fd, F_GETFD, 0);
					if(fcntl(pfds[fd_count].fd, F_SETFD, flags | O_NONBLOCK) < 0) 
					{
						perror("-------------------------------------------Error al hacer non_blocking");
						return 1;
					}*/
					
    				pfds[fd_count].events = POLLIN; // Check ready-to-read

                    //fd_count -1 ya que fd_count incremento cuando poso el socket pare
                    dadesClients[fd_count - 1].ip = charToString(ip); 
    				
					fd_count++;
                }



            } 
            else  if (pfds[i].revents & POLLIN )// Per tots els POLLINs que no siguin el socket socket_server (listener)
			{
                char *buffer_recv;

                long int n_bytes;
                n_bytes = recv(pfds[i].fd, buffer_recv, MSG_SIZE, 0);
                
                if (n_bytes <= 0 )
				{
					if (n_bytes == -1)
					{
						printf("Error al recivir el mensaje -1\n");
						continue;
					}

					close(pfds[i].fd);
					
					for(int j = i; j < (fd_count-1); j++)
					{
						pfds[j].fd = pfds[j+1].fd;
						pfds[j].events = pfds[j+1].events;
						pfds[j].revents = pfds[j+1].revents;
					}
					
					fd_count--;

                    continue;
				}

                Type format = seeFormatText(charToString(buffer_recv));
                

                switch (format)
                {
                case Registration:
                    nom = treureNomUsuari(charToString(buffer_recv));

                    //setNomtoIP(nom, dadesClients[i].ip)
                    dadesClients[searchIP(dadesClients, ip)].nom = nom;

                    break;


                case Connect:                   

                    //enviar "connect: userDesti"
                    //iniciar chat

                    nom = treureNomUsuari(charToString(buffer_recv));
                    

                    //miro si el usuari desti esta a la meva llista de clients 
                    for(int c = 0; c < fd_size; c++)
                    {
                        if( dadesClients[c].nom == nom )
                        {
                            Type_demanat[i] = Connect;
                            pfds[i].events = POLLOUT;
                            break;                            
                        }
                    }

                    break;

                case Text:
                    //busco en que chat esta la ip i hago send a la otra ip con el formato: "origen: text", si l'origen es el servidor serà "Servidor[flag]: text"
                    Type_demanat[i] = Text;
                    pfds[i].events = POLLOUT;
                    break;


                case Command:
                    //faig coses segons el comando que m'han enviat
                    Type_demanat[i] = Command;
                    pfds[i].events = POLLOUT;
                    break;
                

                default:
                    //algun error aqui
                    break;
                }

                //he rebut un missatge d'una IP
                //he d'interpretar el missatge, 
                //si no te format de comanda suposo que es un enviament a un client
                //si te format de nou chat he de crear el nou chat 

            }
            else if(pfds[i].revents & POLLOUT)
            {
                switch (Type_demanat[i])
                {
                case Connect:
                    //enviar "connect: userDesti"
                    //iniciar chat
                    /* code */

                    char respuesta[MSG_SIZE];
                    memset(respuesta, 0, MSG_SIZE);

                    if (send(pfds[i].fd, respuesta, (long unsigned int)MSG_SIZE, MSG_NOSIGNAL) == -1) 
					{
						close(pfds[i].fd);
						
						for(int j = i; j < (fd_count-1); j++)
						{
							pfds[j].fd = pfds[j+1].fd;
							pfds[j].events = pfds[j+1].events;
							pfds[j].revents = pfds[j+1].revents;
						}
						
						fd_count--;
						continue;	
                    }

                    if(chats[numChats].ip[0] == ip)
                    {
                        //enviar a la ip[1] el connect:

                        //for c in dadesClients:
                        //      if c.nom == nom:
                        //          em quedo la i
                        //          brake
                        //          
                        //sendo to(dadesClients[i].ip)


                        //crec que he de crear un socket que envii el missatge
                    }
                    else if(chats[numChats].ip[1] == ip)
                    {
                        //enviar a la ip[0] el connect:
                    }

                    numChats++;

                    break;

                case Text:
                    //busco en que chat esta la ip i hago send a la otra ip con el formato: "origen: text", si l'origen es el servidor serà "Servidor[flag]: text"
                    /* code */
                    break;

                case Command:
                    //faig coses segons el comando que m'han enviat
                    /* code */
                    break;
                
                default:
                    //algun otro error aqui
                    break;
                }
            }

        }






    }








}