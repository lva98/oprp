echo "---Compilando"
mpic++ main.cpp -fopenmp -lcrypt

echo "---Copiando arquivos"
#scp * client:~/Code/OPRP/

echo "---Execuçao"
mpirun -machinefile machines a.out