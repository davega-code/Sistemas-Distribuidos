#include <bits/stdc++.h>
#include "gfx.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
using namespace std;


int iniciar = 0;
bool parar = false;
const double ERROR = 1e-3;
bool Igual(double a, double b) 
{ 
    return fabs(a - b) < ERROR;
}
void responder(int socket)
{
	
}


void listener(int puerto)
{
	// Server side C/C++ program to demonstrate Socket programming
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
      
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }  
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( puerto );
      
    // Forcefully attaching socket to the puerto 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }


    // aceptar clientes
    vector<thread> clientes;
    for(int i = 0; i < 4 ; i++)
    {
	    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
	                       (socklen_t*)&addrlen))<0)
	    {
	        perror("accept");
	        exit(EXIT_FAILURE);
	    }
	    clientes.push_back(new thread(responder,new_socket));
	}
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
}

int main()
{
	
}