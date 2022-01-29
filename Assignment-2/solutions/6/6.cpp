#include "mpi.h"
#include <bits/stdc++.h>
#include <fstream>
#include <stdio.h>
#include <string.h>
using namespace std;
typedef long long int ll;

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

    int range_low = 0, range_high = 0;
    fstream fin;
    fin.open(argv[1], ios::in);

    fstream fout;
    fout.open(argv[2], ios::out);
    if (rank == 0) {
        int m, n;
        fin >> m >> n;
        vector<int> input_graph[n + 1];
        int graph[m][m];
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                graph[i][j] = 0;
            }
        }
        for (int i = 0; i < n; ++i) {
            int x, y;
            fin >> x >> y;
            x--;
            y--;
            graph[x][y] = 1;
            graph[y][x] = 1;
        }

        MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(graph, m * m, MPI_INT, 0, MPI_COMM_WORLD);

        int batchsize[numprocs];
        for (int i = 0; i < numprocs; ++i) {
            if (i < m % numprocs) {
                batchsize[i] = m / numprocs + 1;
            } else {
                batchsize[i] = m / numprocs;
            }
        }
        range_low = batchsize[0], range_high = batchsize[0];
        for (int i = 1; i < numprocs; ++i) {
            range_high += batchsize[i];
            MPI_Send(&range_low, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&range_high, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            range_low = range_high;
        }
        range_low = 0, range_high = batchsize[0];

        int color[m];
        for (int i = 0; i < m; ++i) {
            color[i] = 0;
        }

        int weight[m];
        for (int i = 0; i < m; ++i) {
            weight[i] = i;
        }

        unsigned seed = 0;
        shuffle(weight, weight + m, default_random_engine(seed));
        int stop_signal = 0;

        while (1) {
            MPI_Bcast(color, m, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(weight, m, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&stop_signal, 1, MPI_INT, 0, MPI_COMM_WORLD);
            if (stop_signal == 1) {
                break;
            }
            for (int i = range_low; i < range_high; ++i) {
                int cancolor = 1;
                set<int> available_colors;
                for (int j = 0; j < m; ++j) {
                    available_colors.insert(j + 1);
                }
                for (int j = 0; j < m; ++j) {
                    if (graph[i][j] == 1) {
                        if (weight[j] > weight[i] && color[j] == 0) {
                            cancolor = 0;
                        }
                        if (color[j] != 0)
                            available_colors.erase(color[j]);
                    }
                }
                if (cancolor) {
                    color[i] = *available_colors.begin();
                }
            }
            stop_signal = 1;
            for (int i = 1; i < numprocs; ++i) {
                int temp_color[m];
                MPI_Recv(temp_color, m, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for (int j = 0; j < m; ++j) {
                    color[j] = max(color[j], temp_color[j]);
                }
            }
            for (int i = 0; i < m; i++) {
                if (color[i] == 0) {
                    stop_signal = 0;
                }
            }
        }
        int max_color = 0;
        for (int i = 0; i < m; ++i) {
            max_color = max(max_color, color[i]);
        }
        fout << max_color << "\n";
        for (int i = 0; i < m; ++i) {
            fout << color[i] << " ";
        }
        fout << "\n";

    } else {
        int m = 0;
        MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

        int graph[m][m];
        MPI_Bcast(graph, m * m, MPI_INT, 0, MPI_COMM_WORLD);

        range_low = 0, range_high = 0;
        MPI_Recv(&range_low, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&range_high, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // fout << rank <<" "<< range_low << " " << range_high<<"\n";

        int colored_cnt = 0, stop_signal = 0;

        while (stop_signal == 0) {

            int color[m];
            MPI_Bcast(color, m, MPI_INT, 0, MPI_COMM_WORLD);

            int weight[m];
            MPI_Bcast(weight, m, MPI_INT, 0, MPI_COMM_WORLD);

            MPI_Bcast(&stop_signal, 1, MPI_INT, 0, MPI_COMM_WORLD);

            for (int i = range_low; i < range_high; ++i) {
                if (color[i] != 0)
                    continue;
                int cancolor = 1;
                set<int> available_colors;
                for (int j = 0; j < m; ++j) {
                    available_colors.insert(j + 1);
                }
                for (int j = 0; j < m; ++j) {
                    if (graph[i][j] == 1) {
                        if (weight[j] > weight[i] && color[j] == 0) {
                            cancolor = 0;
                        }
                        if (color[j] != 0)
                            available_colors.erase(color[j]);
                    }
                }
                if (cancolor) {
                    color[i] = *available_colors.begin();
                    colored_cnt++;
                }
            }

            MPI_Send(color, m, MPI_INT, 0, 0, MPI_COMM_WORLD);
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
