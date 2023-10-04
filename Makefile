CC=g++
FLAGS=-Wall -g

all: marco polo

marco: marco.cpp
	$(CC) $(FLAGS) marco.cpp -o marco

polo: polo.cpp
	$(CC) $(FLAGS) polo.cpp -o polo

clean:
	rm marco polo
