SRCS=$(wildcard *.cpp)
NAME=Mancala

all :
	g++ -Og $(SRCS) -o $(NAME)
release :
	g++ -Ofast $(SRCS) -static -o $(NAME)

