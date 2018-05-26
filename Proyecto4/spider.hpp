#include <bits/stdc++.h>
using namespace std;


class spider
{
	public:
		pair<double,double> pos;
		pair<int,int> seguir;
		double velocidad;
		int id;
	spider(double, double, double, int);
	void setSeguir(pair<int,int>);
	double updatePos(void);
};



