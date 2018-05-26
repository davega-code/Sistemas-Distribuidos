#include "gfx.h"
#include "Asteroide.hpp"
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

//Direcciones
int dx[] = {1,0,-1,-1,-1,0,1,1};
int dy[] = {1,1,1,0,-1,-1,-1,0};

int tamx = 600, tamy = 600;

bool asteroideFuera(Asteroide &ast)
{
	bool flag = true;
	for(int i = 0 ; i < ast.vertices.size(); i++)
	{
		int x = ast.vertices[i].first;
		int y = ast.vertices[i].second;
		if ( (x <= tamx && x>=0) && (y <= tamy && y>=0) ) flag = false;
	}
	return flag;
}

void moverAsteroide(Asteroide &ast)
{
	int n = ast.vertices.size();
	ast.actualizarAsteroide();
	for(int i = 0 ; i < n-1; i++)
	{
		 gfx_line(ast.vertices[i].first, ast.vertices[i].second, 
		 		   ast.vertices[i+1].first, ast.vertices[i+1].second);
	}
	gfx_line(ast.vertices[n-1].first, ast.vertices[n-1].second, 
		    ast.vertices[0].first, ast.vertices[0].second);
}

Asteroide generarAsteroide()
{
	int x0,y0;
	int lado = rand() % 8;
	switch(lado)
	{
		case 0:
		x0 = 0;
		y0 = 0;
		break;
		case 1:
		x0 = rand() % tamx;
		y0 = 0;
		break;
		case 2:
		x0 = tamx;
		y0 = 0;
		break;
		case 3:
		x0 = tamx;
		y0 = rand() % tamy;
		break;
		case 4:
		x0 = tamx;
		y0 = tamy;
		break;
		case 5:
		x0 = rand() % tamx;
		y0 = tamy;
		break;
		case 6:
		x0 = 0;
		y0 = tamy;
		break;
		case 7:
		x0 = 0;
		y0 = rand() % tamy;
		break;
	}
	int dir = rand() % 8;
	return Asteroide(rand() % 30 + 10, x0,y0,dx[dir],dy[dir], rand() % 60 );
}

int main()
{
	srand(time(NULL));
	int nAsteroides;
	cout<<"Numero de asteroides: ";
 	cin>>nAsteroides;
	gfx_open(tamy, tamx, "Asteroides");
 	gfx_color(0,200,100);
 	vector<Asteroide> poblacion;
 	for(int i = 0; i < nAsteroides; i++) poblacion.push_back(generarAsteroide());
 	while(true)
 	{
 		gfx_clear();
 		for(int i = 0; i < nAsteroides; i++)
 		{
 			if(asteroideFuera(poblacion[i]))
 				poblacion[i] = generarAsteroide();
 			else
 				moverAsteroide(poblacion[i]);
 		}
 		gfx_flush();
 		usleep(41666); //24 por segundo
 	}

}