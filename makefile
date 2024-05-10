SRCS=$(wildcard *.cpp)
NAME=Mancala

all :
	g++ -std=c++17 -Og $(SRCS) -o $(NAME)
release :
	g++ -std=c++17 -march=native -flto -Ofast $(SRCS) -static -o $(NAME)
fprofile :
	g++ -std=c++17 -fprofile-generate=profile -march=native -flto -Ofast $(SRCS) -static -o $(NAME)
use-fprofile :
	g++ -std=c++17 -fprofile-use=profile/$(if $(PROFDATA),$(PROFDATA).profdata,default.profdata) -march=native -flto -Ofast $(SRCS) -static -o $(NAME)
