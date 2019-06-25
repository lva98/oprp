#include <mpi.h>
#include <omp.h>
#include <bits/stdc++.h>
#include <sys/sysinfo.h>
#include "source.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    MPI::Init(argc, argv);
    MPI::COMM_WORLD.Set_errhandler(MPI::ERRORS_THROW_EXCEPTIONS);
    try {
        oprp::rank = MPI::COMM_WORLD.Get_rank();
        oprp::size = MPI::COMM_WORLD.Get_size();
        
        oprp::start_up("entry");
        MPI_Barrier(MPI_COMM_WORLD);
        oprp::run_recursive();
        //cout << omp_get_max_threads() << endl; 
    } catch (MPI::Exception e) {
        std::cout << "MPI ERROR: " << e.Get_error_code() \
        << " - " << e.Get_error_string() \
        << std::endl;
    }
    MPI::Finalize();

    return 0;
}