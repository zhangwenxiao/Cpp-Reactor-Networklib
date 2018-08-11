test3: test3.cpp Channel.o EventLoop.o Poller.o
	g++ -std=c++11 -o test3 test3.cpp Channel.o EventLoop.o Poller.o -lmuduo_net -lmuduo_base -lpthread

EventLoop.o: EventLoop.cpp EventLoop.h
	g++ -std=c++11 -c EventLoop.cpp

Channel.o: Channel.cpp Channel.h
	g++ -std=c++11 -c Channel.cpp 

Poller.o: Poller.cpp Poller.h
	g++ -std=c++11 -c Poller.cpp

clean:
	rm *.o
