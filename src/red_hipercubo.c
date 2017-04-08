/*****************************************************************************
* red_hipecubo.c                                                             *
* Autor: José Ángel Martín Baos.                                             *
* Diseño de Infraestructura de Red - Práctica 1                              *
* 2017                                                                       *
*****************************************************************************/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define D 4 // Number of dimensions
#define DATOS_FILE "datos.dat"

void read_and_distribute_token(int* token, int rank, int numtasks){
  int rc;
  MPI_Status status;
  MPI_Request request;
  FILE* file = fopen(DATOS_FILE , "r");
  int i = 0;
  int element = 0;
  while (!feof (file) && i < numtasks){
    fscanf (file, "%d", &element);
    if(i == rank){
      *token = element;
    }else{
      rc = MPI_Isend(&element, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &request);
      if (rc != MPI_SUCCESS) {
         printf("Send error in task %d\n", rank);
         MPI_Abort(MPI_COMM_WORLD, rc);
         exit(1);
      }
    }
    i++;
  }
  fclose (file);
}

void obtain_token(int* token, int rank){
  int rc;
  MPI_Status status;
  rc = MPI_Recv(token, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  if (rc != MPI_SUCCESS) {
     printf("Receive error in task %d\n", rank);
     MPI_Abort(MPI_COMM_WORLD, rc);
     exit(1);
  }
}

int obtain_neighbor(int rank, int dimension){
  int neighbor, node;
  for(node = 0; node < (int)pow(2,D); node++){
    if((rank ^ node) == (int)pow(2,dimension - 1)){
      neighbor = node;
      break;
    }
  }
  return neighbor;
}

int main (int argc, char *argv[]){
  int numtasks,       // Number of MPI tasks
      rank,           // Task number of the current process
      token;          // The number from datos.dat for this process

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  if (numtasks != (int)pow(2,D) && rank == 0){
    printf("Error: %d task are needed.\n", (int)pow(2,D));
    fflush(stdout);
    MPI_Abort(MPI_COMM_WORLD, 1);
    exit(1);
  }

  if(rank == 0){
    read_and_distribute_token(&token, rank, numtasks);
  }else{
    obtain_token(&token, rank);
  }

  // Algorithm
  int i, in_token, rc;
  MPI_Status status;
  for (i = 1; i <= D; i++){
    rc = MPI_Send(&token, 1, MPI_INT, obtain_neighbor(rank, i), i,
                  MPI_COMM_WORLD);
    if (rc != MPI_SUCCESS) {
       printf("Send error in task %d\n", rank);
       MPI_Abort(MPI_COMM_WORLD, rc);
       exit(1);
    }

    rc = MPI_Recv(&in_token, 1, MPI_INT, obtain_neighbor(rank, i), MPI_ANY_TAG,
                  MPI_COMM_WORLD, &status);
    if (rc != MPI_SUCCESS) {
       printf("Receive error in task %d\n", rank);
       MPI_Abort(MPI_COMM_WORLD, rc);
       exit(1);
    }

    if(in_token > token){
      token = in_token;
    }
  }

  if(rank == 0){
    printf("El mayor elemento de toda la red es %d.\n", token);
  }

  MPI_Finalize();
  exit(0);
}
