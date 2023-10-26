#include <mpi.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TERMINATE -1
#define READY 0

void hello_world(int message) {
    int procID;

    MPI_Comm_rank(MPI_COMM_WORLD, &procID);
    // Sleep for message % 5 seconds
    sleep(message % 5);

    // Print message
    printf("Hello world from task %d with process %d\n", message, procID);
}

int main(int argc, char** argv) {
    int numTasks = atoi(argv[1]);

    MPI_Init(&argc, &argv);
    int procID, totalProcs;
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &procID);
    if (procID == 0) {
        // Manager node
        int nextTask = 0;
        MPI_Status status;
        int32_t message;

        while (nextTask < numTasks) {
            MPI_Recv(&message, 1, MPI_INT32_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            int workerID = status.MPI_SOURCE;
            MPI_Send(&nextTask, 1, MPI_INT32_T, workerID, 0, MPI_COMM_WORLD);
            nextTask++;
        }

        // Wait for all processes to finish
        for (int i = 1; i < totalProcs; i++) {
            MPI_Recv(&message, 1, MPI_INT32_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            int workerID = status.MPI_SOURCE;
            int terminate = TERMINATE;
            MPI_Send(&terminate, 1, MPI_INT32_T, workerID, 0, MPI_COMM_WORLD);
        }
    } else {
        // Worker node
        int32_t message;

        while (true) {
            message = READY;
            MPI_Send(&message, 1, MPI_INT32_T, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(&message, 1, MPI_INT32_T, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (message == TERMINATE)
                break;
            hello_world(message);
        }
    }
    MPI_Finalize();
}
