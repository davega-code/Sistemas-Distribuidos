	g++ Server.cpp -std=c++11 -pthread -c
	g++ gfx.o Server.o -o Server -lX11 -pthread