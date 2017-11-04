CPP=g++
FLAGS= -std=c++11 -g -Wall

all: minimax_player random_player minimax_alpha_beta_player parametric_player optimizer geneticOptimizer

optimizer:
	g++ -std=c++11 optimizer.cpp -o optimizer

geneticOptimizer:
	g++ -std=c++11 geneticOptimizer.cpp -o geneticOptimizer

parametric_player: parametricPlayer.cpp board.o
	$(CPP) $(FLAGS) -o $@ $< board.o

minimax_player: minimax_player.cpp board.o
	$(CPP) $(FLAGS) -o $@ $< board.o

minimax_alpha_beta_player: minimax_alpha_beta_player.cpp board.o
	$(CPP) $(FLAGS) -o $@ $< board.o

random_player:
	g++ -std=c++11 random_player.cpp -o random_player

%.o: %.cpp
	$(CPP) $(FLAGS) -c -o $@ $<

clean:
	rm -f minimax_player minimax_alpha_beta_player random_player optimizer geneticOptimizer *.o *~
