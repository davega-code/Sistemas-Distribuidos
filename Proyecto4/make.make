Server: Server.cpp
	 g++ Server.cpp -std=c++11 -pthread -c
	 g++ gfx.o Server.o -o Server -lX11 -pthread



Cliente: Cliente.cpp spider.cpp spider.hpp
	 g++ Cliente.cpp spider.cpp spider.hpp -std=c++11

Spider: spider.cpp spider.hpp
	 g++ spider.cpp spider.hpp -std=c++11

clean:
	rm Proyecto3.out lectura.hpp.gch

