#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "mensaje.hpp"
#define PORT 8080
using namespace std;
vector<string> candidatos = {"Anaya","Meade","Peje","Bronco"};


int enviar_votos()
{
	struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[40] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  	// conectarse al servidor e iniciar 
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    //Contar votos enviados y votos locales
    int current = 0;
    map<string,int> localDB;
    while(true)
    {
    	//Se envian 60 votos y luego se envia la base de datos local 
    	//Ej. 1 'candidato'
    	//Ej. 2 'candidato1' 'votos' 'candidato2' 'votos' .....
    	current++;
    	usleep(41666);
    	if(current<60)
    	{
	    	int voto = rand() % candidatos.size();
	    	localDB[candidatos[voto]]++;
	    	string s = "1 " + candidatos[voto];
	    	//cout<<s<<endl;		
	    	send(sock,s.c_str(),s.size(),0);
    	}
    	else
    	{
    	    current = 0;
    	    string s = "2";
    	    for(auto it = localDB.begin(); it != localDB.end(); it++)
    	    {
    	    	s+=" ";
    	    	s+=it->first + " " +to_string(it->second);
    	    }
    	    //cout<<s<<endl;		
	    	send(sock,s.c_str(),s.size(),0);    	
    	}
    }
    close(sock);
    return 0;
}


int main()
{
	srand (time(NULL));
    cout<<"Numero de casillas: ";
    int hilos;
    cin>>hilos;
    vector<thread> casillas;
    for(int i = 0; i < hilos; i++) 
    {
    	usleep(41666); 
    	casillas.push_back(thread(enviar_votos));
    }
    while(true)
    {
    	usleep(41666);
    }
    return 0;
}
