#!/bin/bash

now=$(date +"%T")
echo "comenzando test en: $now"

for i in {20..100..20}
	do
		for j in {1..5..1}
			do
				for l in {1..3..1}
					do
						echo ENTRENANDO CON MINIMAX FAST
						echo "con score de tipo $j en iteración l $l"
						echo empiezo a subir news
						echo "población = $i; porcentajes: 10 20"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 10 20 3 2 100 $j 1000 minimax_fast 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 10 20 3 2 100 $j 1000 minimax_fast 1 ./gen_hel_test > /dev/null
						echo "población = $i; porcentajes: 20 20"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 20 20 3 2 100 $j 1000 minimax_fast 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 20 20 3 2 100 $j 1000 minimax_fast 1 ./gen_hel_test > /dev/null
						echo "población = $i; porcentajes: 30 20"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 20 3 2 100 $j 1000 minimax_fast 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 20 3 2 100 $j 1000 minimax_fast 1 ./gen_hel_test > /dev/null
						echo "población = $i; porcentajes: 50 20"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 50 20 3 2 100 $j 1000 minimax_fast 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 50 20 3 2 100 $j 1000 minimax_fast 1 ./gen_hel_test > /dev/null
				
						echo empiezo a subir breeds
						echo "población = $i; porcentajes: 30 10"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 10 3 2 100 $j 1000 minimax_fast 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 10 3 2 100 $j 1000 minimax_fast 1 ./gen_hel_test > /dev/null
						echo "población = $i; porcentajes: 30 30"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 20 3 2 100 $j 1000 minimax_fast 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 20 3 2 100 $j 1000 minimax_fast 1 ./gen_hel_test > /dev/null
						echo "población = $i; porcentajes: 30 40"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 40 3 2 100 $j 1000 minimax_fast 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 40 3 2 100 $j 1000 minimax_fast 1 ./gen_hel_test > /dev/null
						echo "población = $i; porcentajes: 30 50"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 50 3 2 100 $j 1000 minimax_fast 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 50 3 2 100 $j 1000 minimax_fast 1 ./gen_hel_test > /dev/null

						echo ENTRENANDO CON PARAMETRIC
						echo "población = $i; porcentajes: 10 20"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 10 20 3 2 100 $j 1000 parametric_player 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 10 20 3 2 100 $j 1000 parametric_player 1 ./gen_hel_test > /dev/null
						echo "población = $i; porcentajes: 20 20"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 20 20 3 2 100 $j 1000 parametric_player 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 20 20 3 2 100 $j 1000 parametric_player 1 ./gen_hel_test > /dev/null
						echo "población = $i; porcentajes: 30 20"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 20 3 2 100 $j 1000 parametric_player 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 20 3 2 100 $j 1000 parametric_player 1 ./gen_hel_test > /dev/null
						echo "población = $i; porcentajes: 50 20"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 50 20 3 2 100 $j 1000 parametric_player 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 50 20 3 2 100 $j 1000 parametric_player 1 ./gen_hel_test > /dev/null
				
						echo empiezo a subir breeds
						echo "población = $i; porcentajes: 30 10"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 10 3 2 100 $j 1000 parametric_player 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 10 3 2 100 $j 1000 parametric_player 1 ./gen_hel_test > /dev/null
						echo "población = $i; porcentajes: 30 30"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 20 3 2 100 $j 1000 parametric_player 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 20 3 2 100 $j 1000 parametric_player 1 ./gen_hel_test > /dev/null
						echo "población = $i; porcentajes: 30 40"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 40 3 2 100 $j 1000 parametric_player 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 40 3 2 100 $j 1000 parametric_player 1 ./gen_hel_test > /dev/null
						echo "población = $i; porcentajes: 30 50"
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 50 3 2 100 $j 1000 parametric_player 0 ./gen_hel_test > /dev/null
						./geneticOptimizer helix 7 6 4 60 1 $i 150 25 30 50 3 2 100 $j 1000 parametric_player 1 ./gen_hel_test > /dev/null
				done
		done
done

now=$(date +"%T")
echo "terminando test en: $now"