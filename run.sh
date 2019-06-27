echo "---Compilando"
mpic++ main.cpp -fopenmp -lcrypt -o slugger

echo "---Copiando arquivos"
for i in `cat machines`; 
    do scp slugger $i:~/Code/OPRP/; 
done

echo "---Execução"
mpirun -machinefile machines slugger