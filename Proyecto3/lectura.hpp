#include <bits/stdc++.h>
#include <sys/types.h>
#include <dirent.h>
using namespace std;

class lectura
{
	public:
	map<string,int> ocurrencia;
	vector <thread> hilos;
	string directorio;
	lectura(string);
	void contar_palabras(int);
	vector<string> get_NombreArchivos();
	void contar(vector<string>);
};