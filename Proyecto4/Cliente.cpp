#include "spider.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
pair<int,int> getNumber(char *buffer)
{
	int a = -1, b = -1;
	bool flag = true;
	for(int i = 0; i < 1024; i++)
	{
		if(buffer[i] == '#') break;
		if(buffer[i] ==' ')
		{
			flag = false;
			continue;
		}
		if(flag)
		{
			if(a == -1) a = 0;
			a*=10;
			a+= buffer[i]-'0';
		}
		else
		{
			if(b == -1) b = 0;
			b*=10;
			b+= buffer[i]-'0';
		}
	}
	return make_pair(a,b);
}
  
int main(int argc, char const *argv[])
{
	int Velocidad;
	cout<<"Velocidad: ";
	cin>>Velocidad;

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

    // Leer L y definir posicion inicial
    //send(sock , hello , strlen(hello) , 0 );
    valread = read( sock , buffer, 40);
    pair<int,int> L = getNumber(buffer);
    fill(buffer,buffer+40,0);
    valread = read(sock, buffer,40);
    pair<int,int> id = getNumber(buffer);
    cout<<L.first<<' '<<id.first<<'\n';
    double x,y;
    switch(id.first)
    {
    	case 0:
    	x = 0;
    	y = 0;
    	break;
    	case 1:
    	x = L.first;
    	y = 0;
    	break;
    	case 2:
    	x = L.first;
    	y = L.first;
    	break;
    	case 3:
    	x = 0;
    	y = L.first;
    	break;
    }
    //Setear la spider del cliente
    spider current(x,y,Velocidad,id.first);
    string s = to_string((int) x) + " " + to_string((int)y)+"#";
    send(sock , s.c_str(), s.size() , 0 );
    //Enviar posicion inicial al servidor
    double res = 0;
    pair<int,int> op;
    while(true)
    {
    	fill(buffer,buffer+40,0);
    	valread = read(sock, buffer,40);
   		pair<int,int> aux = getNumber(buffer);
        if(aux.first <= L.first && aux.first>=0)
        {
            op.first = aux.first;
            op.second = aux.second;
        }

        //cout<<"Seguir: "<<op.first<<' '<<op.second<<'\n';
   		if(op.second == 9999) // Choco
   		{
   			break;
   		}
   		//updateamos posicion a seguir
   		current.setSeguir(op);
   		res += current.updatePos();
   		//regresamos posicion actual
   		string s = to_string((int)current.pos.first)+ " " + to_string((int)current.pos.second) + "#";
        //cout<<"Posicion: "<<s<<'\n';
   		send(sock , s.c_str(), s.size() , 0 );
    }
    cout<<"Distancia recorrida: "<<res<<'\n';
    close(sock);
    return 0;
}