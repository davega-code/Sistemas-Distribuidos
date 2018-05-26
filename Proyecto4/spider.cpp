#include "spider.hpp"

spider::spider(double x, double y, double velocidad, int id)
{
	this->pos = make_pair(x,y);
	this->velocidad = velocidad;
	this->id = id;
}

void spider::setSeguir(pair<int,int> seg)
{
	seguir = seg;
}

double spider::updatePos()
{
	double dx = abs(pos.first-seguir.first), dy = abs(pos.second - seguir.second);
	double dist = sqrt(dx*dx + dy*dy);
	if(seguir.first < pos.first) dx*=-1;
	if(seguir.second < pos.second) dy*=-1;
	double xmove = (dx/dist * velocidad); // hay que cuidar el modulo para que no se pase
	double ymove = (dy/dist * velocidad); // si esta cosa es mayor a la distancia, lo seteamos a pos
	pos.first += xmove;
	pos.second += ymove;
	return abs(xmove+ymove); 
}