#include "lectura.hpp"
using namespace std;
int main()
{
	string dir;
	int nhilos = 0;
	cout<<"Directorio: ";
	cin>>dir;
	cout<<"Numero de hilos: ";
	cin>>nhilos;
	lectura lct(dir);
	vector<string> res = lct.get_NombreArchivos();
	for(int i = 0; i < res.size(); i++)
		cout<<res[i]<<"\n";
	lct.contar_palabras(nhilos);
}