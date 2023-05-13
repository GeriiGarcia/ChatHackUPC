# ChatHackUPC
Will be a program to chat with other terminal in other network


## Server:
    
    Basicametn ha d'enviar d'un client a l'altre els missatges
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
    
    Funcionament del Servidor
        


## Client:

    El proposit del client es conectarse al servidor i a partir de les comandes d'aquest enviar missatges a altres clients 

    El primer que fa el client es conectar-se al servidor amb un usuari.
    El client podrà parlar amb usuaris registrats online 
    Per via de les comandes del servidor el client podrà realitzar accions com conectarse a altres clients, desconectar-se del servidor, ..
    Per parlar amb el servidor s'hauràn de fer servir comandes 

    Menu principal:
        - Amb qui vols interactuar?


## Possibles errors
    - Ports (aixó pot sortir molt malament)
    - Al client al tindre dos processos potser generen colisió al enviar dades per el mateix socket (possible solució fent un altre socket amb diferent port)
    - Al conectarnos podem fer una sequencua syn ack on el servidor demana les nostres dades indefinidament fins que les tingui
    - Errors al intentar conectar a una ip quan guardem la seva ip i el nom de l'usuari
    - Els flags d'alguns recv i send poden estar malament
    - Que estigui sempre rebent el client (fer O_NONBLOCK)



## Coses que podem fer si ens motivem

    - paswords als usuaris
    - guardar missatges d'un usuari a un altre quan un estigui desconectat
    - Array dinàmic al servidor amb missatges pendents a cada client.
    - Guardar informació del servidor


## Formats
    - Format per cridar el client " ./client userName"
    - Format de Registre: "username: name"
    - Format de comandes: "::comanda"
    - Format de servidor a client: "origen: text", si l'origen es el servidor serà "Servidor[flag]: text"
    - Format iniciar conversa: "connect: userDesti"