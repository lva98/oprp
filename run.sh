echo "---Compilando"
mpic++ main.cpp -fopenmp -lcrypt -o slugger

echo "---Copiando arquivos"
scp * client:~/Code/OPRP/
#scp * client1:~/Code/OPRP/

echo "---Execução"
mpirun -machinefile machines slugger