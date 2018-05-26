#include "lectura.hpp"


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
	int aphilo = archivos.size() / nhilos;
	bool flag = false;
	if(aphilo * nhilos < archivos.size()) flag = true;
	aphilo = aphilo == 0?1:aphilo;
	for(int i = 0; i < nhilos; i++)
	{
		vector<string> archivos_hilo;
		for(int j = i *aphilo; j < (i+1) * aphilo && j < archivos.size(); j++)
		{
			archivos_hilo.push_back(archivos[j]);
		}
		if(i == nhilos -1 && flag) // archivos que faltan;
		{
			for(int j = aphilo * nhilos -1; j < archivos.size(); j++) 
				archivos_hilo.push_back(archivos[j]);
		}
		hilos.push_back( thread([=] { contar(archivos_hilo); } )); // hay que usar una lambda 
		// otra manera std::thread( [this] { test(); } )
	}
	for(int i = 0; i < hilos.size(); i++) hilos[i].join();

	int count = 0;
	for(auto it = ocurrencia.begin(); it!= ocurrencia.end() && count < 10; it++, count++)
	{
		cout<<"Posicion #"<<count+1<<": "<<it->first<<" "<<it->second<<"\n";
	}
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
}


