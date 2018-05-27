// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "mensaje.hpp"
#include <bits/stdc++.h>
using namespace std;
typedef unsigned int uint;
vector< map<string,int> > casilla_votos;
map<string,bool> double_check;

//Metodos mongooso
static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
  }
}

void responder(int socket)
{
	int id = casilla_votos.size();
	casilla_votos.push_back(map<string,int>());
	while(true)
    {
    	char buffer[100] = {0};
    	read( socket , buffer, 100);
    	string s(buffer);
    	//cout<<s<<'\n';
		stringstream ss(s);
		char tipo;
		ss>>tipo;
		if(tipo == '1')
		{
			string voto;
			ss>>voto;
			casilla_votos[id][voto]++;
		}
		else
		{
			string candidato;
			int votos;
			while(ss>>candidato>>votos)
			{
				casilla_votos[id][candidato] = votos;
			}
		}
    	//cout<<s<<endl;

    }
}


void escribir_resultados()
{
	while(true)
	{
	 // Escribir resultados cada 2 segundos
	 usleep(2*41666);
	 // Hay que sumar los votos de todas las casillas
	 // Debido al numero reducido de candidatos no hay tanto problema de tiempo
	 map<string,int> votos;
	 for(uint i =0; i < casilla_votos.size(); i++)
	 {
	 	for(auto iter = casilla_votos[i].begin(); iter != casilla_votos[i].end(); iter++)
	 	{
	 		if(double_check[iter->first])
	 		votos[iter->first] += iter->second;
	 	}
	 }
	 // Se escribe el JSON para javascript
	 // Ej. [[297, "Anaya"],[254, "Bronco"],[275, "Meade"],[261, "Peje"]]
	 ofstream JSON("Candidatos.json");
	 JSON<<"[";
	 bool flag = false;
	 for(auto iter = votos.begin(); iter != votos.end(); iter++)
	 {
	 	if(flag) JSON<<",";
	 	JSON<<"["<<iter->second<<", \""<<iter->first<<"\"]";
	 	flag = true;
	 }
	 JSON<<"]";
	 JSON.close();
	}
}

void UDP_listener()
{
	//Agregar un mapa para checar dos veces en caso de error de paquete
	double_check["Anaya"] = true;
	double_check["Meade"] = true;
	double_check["Bronco"] = true;
	double_check["Peje"] = true;
	//Configuracion del socket
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int puerto = 8080;
      
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
    //Hilo para escribir resultados
    thread resultados(escribir_resultados);
    //Hilo para unir clientes
    vector <thread> clientes;
    while(true)
    {
    	//Cada que un cliente se une se asigna un hilo para escuchar
    	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
	    {
	        perror("accept");
	        exit(EXIT_FAILURE);
	    }
	    clientes.push_back(thread(responder,new_socket));
	    cout<<"Casilla "<<clientes.size()<<": conectada\n";
    }
    //Aun hay un memory leak, hay que acabar con el thread cuando ya no responda el socket del cliente
    resultados.join();
    for(uint i = 0; i < clientes.size(); i++) clientes[i].join();
    return;
}




int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  mg_mgr_init(&mgr, NULL);
  printf("Starting web server on port %s\n", s_http_port);
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    printf("Failed to create listener\n");
    return 1;
  }
  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";  // Serve current directory
  s_http_server_opts.enable_directory_listing = "yes";
  thread listen(UDP_listener);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  listen.join();
  mg_mgr_free(&mgr);

  return 0;
}
