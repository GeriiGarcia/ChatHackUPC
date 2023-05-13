#include "funcions.h"


// Global variables and constants
#define SERVER_PORT 8080
bool connectionTerminated;



// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- Classes/Structs +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- // 
class UserData{
	public:
	UserData(const char* userName) { strcpy(m_userName, userName); }
	~UserData() {}

	char* getUserDataBuffer();


	private:
	char* m_userName;

};


// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- Main Function +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- // 
int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("[-] Nombre de paràmetres incorrecte\n");
		return 1;
	}
	
	// User initial information
	UserData user(argv[1]);

    // creem el socket, la adreça i ens conectem al servidor
    // create soket
	printf("...creem socket...\n");
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		perror("[-] Error al crear el Socket");
		return 1;
	}
	
    // server adress
	struct sockaddr_in addrServidor;
	addrServidor.sin_family = AF_INET;
	addrServidor.sin_addr.s_addr = inet_addr(argv[1]); 
	addrServidor.sin_port = htons(SERVER_PORT);
	
    // connect to server
	printf("...conectem al servidor...\n");
	if(connect(sock, (struct sockaddr *) &addrServidor, sizeof(addrServidor)) == -1){
		perror("[-] Error conectant");
		close(sock);
		return 1;
	} 
	connectionTerminated = false;


	// enviem la informació de l'usuari al servidor // potser cal arreglar aixó
	char *buffer_userInfoToServer;
	strcpy(buffer_userInfoToServer, user.getUserDataBuffer());

	if(send(sock, buffer_userInfoToServer, strlen(buffer_userInfoToServer) ,0) == -1){
		perror("[-] Error al enviar missatge de validació\n");
		close(sock);
		return 1;
	}



    // Creem els threads que executaràn les funcions de send i recv
    thread t1(recvProgram(sock));
    thread t2(sendProgram(sock));


	// juntem els threads
	t1.join();
    t2.join();

	delete buffer_userInfoToServer;
    return 0;
}

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- Thread functions +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- // 
int recvProgram(int sock)
{
	while(!connectionTerminated)
	{
		char *mssg;
		
		// recive block
		int r = recv(sock, mssg, sizeof(int), MSG_WAITALL < 0);
		if(r == -1){
			perror("[-]Recv error");
			connectionTerminated = true;
			close(sock);
			return 1;
		}
		else if (r == 0)
		{
			printf("[-] Conexió tancada per part del servidor");
		}
		else
		{
			// Show block
			cout << mssg << endl;

			// Gestionar casos
			if(strcmp("Server", extractSubstring(mssg, 0, 8)))
			{
				if(extractSubstring(mssg, 9, 1) == "c") // conexió tancada
				{
					
				}
			}


		}

		delete mssg;
	}

	connectionTerminated = true;
	return 0;
}

int sendProgram(int sock)
{
	while(!connectionTerminated)
	{
		char *mssg;

		// El usuari escriu el missatge que vol enviar
		cin >> mssg; // poter no funciona bé el cin amb un char*
		
		// Comprovem alguns errors que poden surgir abans d'enviar
		if(connectionTerminated)
		{
			printf("[-] Conexió acabada, no s'ha pogut enviar el missatge\n");
			break;
		} 
		else if (strlen(mssg) == 0) // sin el cin està buit no enviarem el missatge
		{
			continue;
		}
		else
		{
			if(send(sock, mssg, strlen(mssg) ,0) == -1){
				perror("[-] Error al enviar missatge de validació\n");
				connectionTerminated = true;
				close(sock);
				return 1;
			}
		}


		delete mssg;
	}

	connectionTerminated = true;
	return 0;
}



// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- Other functions +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- // 

char* UserData::getUserDataBuffer() 
{ 
	char* returnBuffer;
	
	concatenateStrings("username:" , m_userName);
	concatenateStrings(returnBuffer , m_userName);
	
	return returnBuffer; 
}

