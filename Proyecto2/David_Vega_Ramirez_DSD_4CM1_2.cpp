#include "gfx.h"
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

class Asteroide
{
	public:
		double dx,dy;
		double giro;
		vector< pair<double,double> > vertices;
		Asteroide(double nVertices, double x0, double y0, double dirx, double diry, double tam)
		{
			dx = (dirx * 10) / tam;
			dy = (diry * 10) / tam;
			dx += dirx;
			dy += diry;
			vector<double> angulos;
			for(int i = 0; i < nVertices; i++)
			{
				angulos.push_back( (rand() % 361) *  M_PI/180);
			}
			sort(angulos.begin(),angulos.end());
			for(int i = 0; i < nVertices; i++)
			{
				double x = x0 + tam*cos(angulos[i]); //cambiar r
				double y = y0 + tam*sin(angulos[i]);
				vertices.push_back(make_pair(x,y));
			}
		}
		~Asteroide()
		{
			return;
		}

		void rotarVertice(double id,double grados)
		{
				double radianes = grados * M_PI / 180.0;
				double x = vertices[id].first, y = vertices[id].second;
				vertices[id].first = x * cos(radianes) - y * sin(radianes);
				vertices[id].second = x * sin(radianes) + y * cos(radianes);
		}

		pair<double,double> buscarCentroide()
		{
			double area = 0,aux = 0;
			double x = 0, y = 0;
			for(int i = 0; i < vertices.size() - 1; i++)
			{
				aux = vertices[i].first * vertices[i+1].second;
				aux-= vertices[i+1].first * vertices[i].second;
				area+=aux;
				x += (vertices[i].first + vertices[i+1].first) * aux;
				y += (vertices[i].second + vertices[i+1].second) * aux;
			}

			aux = vertices[vertices.size()-1].first * vertices[0].second;
			aux-= vertices[0].first * vertices[vertices.size()-1].second;
			area+=aux;
			x += (vertices[vertices.size()-1].first + vertices[0].first) * aux;
			y += (vertices[vertices.size()-1].second + vertices[0].second) * aux;
			area /=2;
			x /= (6 * area);
			y /= (6 * area);
			return make_pair(x,y);
		}


		void actualizarAsteroide()
		{
			pair<double,double> centroide = buscarCentroide();
			for(int i = 0; i < vertices.size(); i++)
			{
				vertices[i].first += dx;
				vertices[i].second += dy;

				//Rotacion en torno a centroide
				vertices[i].first -= centroide.first;
				vertices[i].second -= centroide.second;
				rotarVertice(i,10);
				vertices[i].first += centroide.first;
				vertices[i].second += centroide.second;
			}

		}
		void actualizarVertice(double id)
		{
			vertices[id].first += dx;
			vertices[id].second += dy;
		}	
};

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