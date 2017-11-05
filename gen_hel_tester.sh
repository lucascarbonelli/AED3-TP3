#!/bin/bash

now=$(date +"%T")
echo "comenzando test en: $now"

for i in {20..100..20}
	do
		for j in {1..5..1}
			do
				echo empiezo a subir news
				echo "población = $i; porcentajes: 10 20"
				./geneticOptimizer helix 7 6 4 60 1 5 $i 100 3 10 20 2 1000 2 $j 3 1000 ./gen_hel_res ./gen_hel_err ./gen_hel_test > /dev/null
				echo "población = $i; porcentajes: 20 20"
				./geneticOptimizer helix 7 6 4 60 1 5 $i 100 3 20 20 2 1000 2 $j 3 1000 ./gen_hel_res ./gen_hel_err ./gen_hel_test > /dev/null
				echo "población = $i; porcentajes: 30 20"
				./geneticOptimizer helix 7 6 4 60 1 5 $i 100 3 30 20 2 1000 2 $j 3 1000 ./gen_hel_res ./gen_hel_err ./gen_hel_test > /dev/null
				echo "población = $i; porcentajes: 50 20"
				./geneticOptimizer helix 7 6 4 60 1 5 $i 100 3 50 20 2 1000 2 $j 3 1000 ./gen_hel_res ./gen_hel_err ./gen_hel_test > /dev/null
				echo "población = $i; porcentajes: 70 20"
				./geneticOptimizer helix 7 6 4 60 1 5 $i 100 3 70 20 2 1000 2 $j 3 1000 ./gen_hel_res ./gen_hel_err ./gen_hel_test > /dev/null
		
				echo empiezo a subir breeds
				echo "población = $i; porcentajes: 30 30"
				./geneticOptimizer helix 7 6 4 60 1 5 $i 100 3 30 30 2 1000 2 $j 3 1000 ./gen_hel_res ./gen_hel_err ./gen_hel_test > /dev/null
				echo "población = $i; porcentajes: 30 40"
				./geneticOptimizer helix 7 6 4 60 1 5 $i 100 3 30 40 2 1000 2 $j 3 1000 ./gen_hel_res ./gen_hel_err ./gen_hel_test > /dev/null
				echo "población = $i; porcentajes: 30 50"
				./geneticOptimizer helix 7 6 4 60 1 5 $i 100 3 30 50 2 1000 2 $j 3 1000 ./gen_hel_res ./gen_hel_err ./gen_hel_test > /dev/null
				echo población = $i; porcentajes: 30 60
				./geneticOptimizer helix 7 6 4 60 1 5 $i 100 3 30 60 2 1000 2 $j 3 1000 ./gen_hel_res ./gen_hel_err ./gen_hel_test > /dev/null
		done
done

now=$(date +"%T")
echo "terminando test en: $now"