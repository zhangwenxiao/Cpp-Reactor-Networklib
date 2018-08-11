all:
	make test1
	make test2
	make test3

test1: test1.cpp EventLoop.cpp EventLoop.h
	g++ -std=c++11 -o test1 test1.cpp EventLoop.cpp -lmuduo_net -lmuduo_base -lpthread

test2: test2.cpp EventLoop.cpp EventLoop.h
	g++ -std=c++11 -o test2 test2.cpp EventLoop.cpp -lmuduo_net -lmuduo_base -lpthread

test3: test3.cpp EventLoop.cpp EventLoop.h
	g++ -std=c++11 -o test3 test3.cpp EventLoop.cpp -lmuduo_net -lmuduo_base -lpthread

clean: 
	rm test1 test2 test3
