#include "lectura.hpp"

struct comparador
{
	bool operator()(const pair<string, int> &elem1 ,const pair<string, int> &elem2)
	{
		return elem1.second > elem2.second;
	}
};
lectura::lectura(string dir)
{
		directorio = dir;
}
vector<string> lectura::get_NombreArchivos()
{
	vector<string> res;
    DIR* dirp = opendir(directorio.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
    	bool flag1 = false;
    	bool flag2 = false;
    	string s = dp->d_name;
    	for(int i = s.size() - 1; i >= 0 ; i--)
    	{
    		if(flag2) break;
    		if(s[i] != '.') flag1 = true;
    		if(s[i] == '.') flag2 = true;
    	}
    	if(flag1 && flag2)
       	res.push_back(dp->d_name);
    }
    closedir(dirp);
    return res;
}
void lectura::contar_palabras(int nhilos)
{
	vector<string> archivos = get_NombreArchivos();
	vector< vector<string> > archivos_hilo(nhilos);
	bool flag = false;
	for(int i = 0; i < archivos.size(); i++)
	{
		archivos_hilo[i % nhilos].push_back(archivos[i]);
	}
	for(int i = 0; i < nhilos; i++) 
		hilos.push_back( thread([=] { contar(archivos_hilo[i]); } )); 
	// hay que usar una lambda 
	// otra manera std::thread( [this] { test(); } )
	for(int i = 0; i < hilos.size(); i++) hilos[i].join();
	//Ya que terminaron todos los hilos, ordenamos las ocurrencias
	cout<<ocurrencia.size()<<endl;
	vector< pair<string,int> > sorted;
	for(auto it = ocurrencia.begin(); it!= ocurrencia.end(); it++)
	{
		sorted.push_back(make_pair(it->first,it->second));
	}
	sort(sorted.begin(),sorted.end(),comparador());
	for(int i = 0; i < 500 && i < sorted.size(); i++)
		cout<<"Posicion #"<<i+1<<": "<<sorted[i].first<<' '<<sorted[i].second<<'\n';
	return;
}
void lectura::contar(vector<string> narchivo)
{
	for(int i = 0; i < narchivo.size(); i++)
	{
		ifstream infile;
		infile.open(directorio + narchivo[i]);
		string s;
		while(infile >> s) ocurrencia[s]++;
		infile.close();
	}
	return;
}