icpc -mkl -std=c++11 -o percolation percolation.cpp
parallel --jobs 10 ./percolation {} ::: 0.50 0.52 0.54 0.56 0.59 0.62 0.64 0.66 0.68 0.70
icpc -mkl -std=c++11 -o ising2D ising2D.cpp
parallel --jobs 10 ./ising2D {} ::: 2.19 2.21 2.23 2.25 2.27 2.29 2.31 2.33 2.35 2.37
icpc -mkl -std=c++11 -o ising3D ising3D.cpp
parallel --jobs 10 ./ising3D {} ::: 4.39 4.42 4.45 4.48 4.51 4.54 4.57 4.60 4.63 4.66
icpc -mkl -std=c++11 -o RFIM2D RFIM2D.cpp
parallel --jobs 10 ./RFIM2D {} ::: 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0
icpc -mkl -std=c++11 -o RFIM3D RFIM3D.cpp
parallel --jobs 10 ./RFIM3D {} ::: 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0
