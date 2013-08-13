
CFLAGS=-g -Wall -std=c++11

all: banker.o banker_util.o driver


driver:banker.o banker_util.o banker_algo.o driver_banker_static.cpp
	g++ $(CFLAGS) -o driver driver_banker_static.cpp banker_algo.o banker_util.o banker.o


banker.o:banker.hpp banker.cpp
	g++ $(CFLAGS) -c banker.cpp
	
banker_util.o:banker.hpp banker_util.cpp
	g++ $(CFLAGS) -c banker_util.cpp

banker_algo.o:banker_algo.cpp banker.hpp
	g++ $(CFLAGS) -c banker_algo.cpp
	

clean:
	rm -rf banker.o banker_util.o banker_algo.o driver
