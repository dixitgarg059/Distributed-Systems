#include <fstream>
#include <iostream>
#include <mpi.h>
#include <stdio.h>

using namespace std;
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
    double a[20004], b[20004], c[20004], temp[20004];
    double d;
    fstream fin;
    fin.open(argv[1], ios::in);
    fstream fout;
    fout.open(argv[2], ios::out);
    int n;
    if (rank == 0) {
        fin >> n;
        for (int i = 0; i < n * n; i++)
            fin >> a[i];
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(a, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int order = n;
    for (int k = 0; k < 2 * order * order; k++) {
        if ((k % (2 * order) / order) == 0) {
            c[k] = a[(k % (2 * order)) + (order * (k / (2 * order)))];
        } else {
            if (k % ((2 * order) + 1) == order) {
                c[k] = 1;
            } else {
                c[k] = 0;
            }
        }
    }

    for (int i = (order - 1); i > 0; i--) {
        if (c[(i - 1) * order * 2] < c[i * order * 2]) {
            for (int j = 0; j < 2 * order; j++) {
                d = c[((i - 1) * order * 2) + j];
                c[((i - 1) * order * 2) + j] = c[(i * order * 2) + j];
                c[(i * order * 2) + j] = d;
            }
        }
    }

    MPI_Scatter(c, 2 * order, MPI_DOUBLE, temp, 2 * order, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < order; i++) {
        if (rank == i) {
            MPI_Bcast(temp, 2 * order, MPI_DOUBLE, i, MPI_COMM_WORLD);
        } else {
            double value_received[2 * 100 + 1];
            MPI_Bcast(value_received, 2 * order, MPI_DOUBLE, i, MPI_COMM_WORLD);

            double c;
            double d;
            double sum2;
            c = temp[i];
            d = value_received[i];
            sum2 = c / d;

            for (int j = 0; j < 2 * order; j++) {
                temp[j] = temp[j] - (sum2 * value_received[j]);
            }
        }
    }
    double div = temp[rank];
    for (int j = 0; j < 2 * order; j++) {
        temp[j] = temp[j] / div;
    }

    double temp2[20004];
    for (int j = 0; j < order; j++) {
        temp2[j] = temp[j + order];
    }

    MPI_Gather(temp2, order, MPI_DOUBLE, b, order, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int j = 0; j < order * order; j++) {
            fout << b[j] << " ";
            if ((j + 1) % order == 0)
                fout << endl;
        }
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
