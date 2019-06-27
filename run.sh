echo "---Compilando"
pkill a.out
mpic++ main.cpp -fopenmp -lcrypt

echo "---Copiando arquivos"
#scp * client:~/Code/OPRP/

echo "---Execuçao"
mpirun -machinefile machines -np 1 a.out 