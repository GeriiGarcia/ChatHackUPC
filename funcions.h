#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>
#include <thread>

#include <iostream>
using namespace std;


//del que hi ha a la posicio startIndex avança length i et retorn allo
char* extractSubstring(const char* str, size_t startIndex, size_t length) {
    size_t strLength = strlen(str);

    // Adjust the start index and length if they exceed the string boundaries
    startIndex = std::min(startIndex, strLength);
    length = std::min(length, strLength - startIndex);

    // Allocate memory for the substring
    char* substring = new char[length + 1];

    // Copy the characters from the original string to the substring
    strncpy(substring, str + startIndex, length);
    substring[length] = '\0';  // Null-terminate the substring

    return substring;
}


char* concatenateStrings(const char* str1, const char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t newLen = len1 + len2;

    // Allocate memory for the new concatenated string
    char* concatenated = new char[newLen + 1];

    // Copy the first string into the new string
    strcpy(concatenated, str1);

    // Append the second string to the new string
    strcat(concatenated, str2);

    return concatenated;
}




//------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------




string treureNomUsuari(string nom)
{
    return " ";
}

string charToString(char *arr)
{
    int arrSize = sizeof(arr) / sizeof(arr[0]); // Obtener el tamaño del array
    string str(arr, arr + arrSize);
    return str;
}

/**
* @param Error return si hi ha error
* @param Registration si el format es: "username: name"
* @param Text no te format especial
* @param Command si el format es: "::command"
*/
enum Type {
    Error,
    Registration,
    Text,
    Command,
    Connect
};

//retornara un int depend del format que sigui
Type seeFormatText(string msg)
{

    return Error;
}

struct dadesClient
{
    string nom;
    string ip;
};

int searchIP(dadesClient dades[], string ip)
{

    return 1;
}

struct chat
{
    //ha de tenir les 2 ips del chat
    //pot tenir una variable que sigui actiu, aixi no perdo chats pero de moment no
    //cada vegada que algun client tanqui un chat s'esborra de la llista de chats actius
    string ip[2];
};



