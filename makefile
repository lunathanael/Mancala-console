SRCS=$(wildcard *.cpp)
NAME=Mancala

all :
	g++ -std=c++17 -Og $(SRCS) -o $(NAME)
release :
	g++ -std=c++17 -Ofast $(SRCS) -static -o $(NAME)

