SORTREDUCE_INCLUDE=../../include/
SORTREDUCE_LIB=../../obj/

obj/single: single.cpp $(SORTREDUCE_LIB)/libsortreduce.a
	mkdir -p obj
	g++ -o ./obj/single single.cpp -Wall -I$(SORTREDUCE_INCLUDE) -L$(SORTREDUCE_LIB) -std=c++11 -lsortreduce -pthread -laio -march=native -lrt -g
