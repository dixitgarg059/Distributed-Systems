#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <mpi.h>
using namespace std;

const int kNumTerms = 100;
int main(int argc, char **argv) {
    int rank, numprocs;

    // initiate MPI
    MPI_Init(&argc, &argv);

    // get size of the current communicator
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    // get current process rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    /*synchronize all processes*/

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    // enter your code here

    int range_low = (kNumTerms * rank) / numprocs + 1;
    int range_high = (kNumTerms * (rank + 1)) / numprocs;

    fstream fin;
    fin.open(argv[1], ios::in);

    fstream fout;
    fout.open(argv[2], ios::out);
    int x, p;

    if (rank == 0) {
        fin >> x >> p;
    }

    MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
    long double value = x;
    int val = 2 * range_low - 1;
    for (int i = 2; i <= val; ++i) {
        value *= ((long double)(x) / i);
    }

    long double partial_sum = (range_low & 1 ? value : -value);
    for (int i = range_low + 1; i <= range_high; ++i) {

        value /= (long double)((2 * i - 1) * (2 * i - 2));
        value *= x * x;
        partial_sum += (i & 1 ? value : -value);
    }

    long double result;
    MPI_Reduce(&partial_sum, &result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        cout << result << endl;
        fout << setprecision(p) << fixed << result << endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime() - start_time;
    double maxTime;
    // get max program run time for all processes
    MPI_Reduce(&end_time, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        cout << "Total time (s): " << maxTime << "\n";
    }

    // shut down MPI and close
    MPI_Finalize();
    return 0;
}
