#include <bits/stdc++.h>
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