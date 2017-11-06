CPP=g++
FLAGS= -std=c++11 -g -Wall

all: minimax_player random_player minimax_alpha_beta_player minimax_alpha_beta_fast_player parametric_player optimizer geneticOptimizer finalMatch

geneticJuab:
	g++ -std=c++11 geneticJuab.cpp -o geneticJuab

geneticPate:
	g++ -std=c++11 geneticPate.cpp -o geneticPate

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

minimax_alpha_beta_fast_player: minimax_alpha_beta_fast_player.cpp board.o
	$(CPP) $(FLAGS) -o $@ $< board.o

random_player:
	g++ -std=c++11 random_player.cpp -o random_player

finalMatch:
	g++ -std=c++11 match.cpp -o finalMatch

%.o: %.cpp
	$(CPP) $(FLAGS) -c -o $@ $<

clean:
	rm -f minimax_player minimax_alpha_beta_player minimax_alpha_beta_fast_player random_player optimizer geneticOptimizer geneticPate *.o *~
