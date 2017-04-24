/*****************************************************************************
* red_toroide.c                                                              *
* Autor: José Ángel Martín Baos.                                             *
* Diseño de Infraestructura de Red - Práctica 1                              *
* 2017                                                                       *
*****************************************************************************/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define L 4
#define DATOS_FILE "datos.dat"

void read_and_distribute_token(float* token, int rank, int numtasks){
  int rc;
  MPI_Status status;
  MPI_Request request;
  FILE* file = fopen(DATOS_FILE , "r");
  int i = 0;
  float element = 0;
  while (!feof (file) && i < numtasks){
    fscanf (file, "%g,", &element);
    if(i == rank){
      *token = element;
    }else{
      rc = MPI_Isend(&element, 1, MPI_FLOAT, i, 1, MPI_COMM_WORLD, &request);
      if (rc != MPI_SUCCESS) {
         printf("Send error in task %d\n", rank);
         MPI_Abort(MPI_COMM_WORLD, rc);
         exit(1);
      }
      MPI_Wait(&request, &status);
    }
    i++;
  }
  fclose (file);
}

void obtain_token(float* token, int rank){
  int rc;
  MPI_Status status;
  rc = MPI_Recv(token, 1, MPI_FLOAT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  if (rc != MPI_SUCCESS) {
     printf("Receive error in task %d\n", rank);
     MPI_Abort(MPI_COMM_WORLD, rc);
     exit(1);
  }
}

void obtain_neighbors(int* north, int* south, int* east, int* west, int rank,
  int numtasks){
  //North neighbor
  *north = (rank + L) % numtasks;

  //South neighbor
  *south = (rank - L) % numtasks;
  if(*south < 0){
    *south += numtasks;
  }

  //East neighbor
  *east = rank + 1;
  if(*east % L == 0){
    *east -= L;
  }

  //West neighbor
  *west = (rank - 1) % numtasks;
  if(*west % L == L-1){
    *west += L;
  }else if(*west == -1){
    *west = L-1;
  }
}

int main (int argc, char *argv[]){
  int numtasks,       // Number of MPI tasks
      rank,           // Task number of the current process
      north,          // Rank of the north neighbor
      south,          // Rank of the south neighbor
      east,           // Rank of the east neighbor
      west;           // Rank of the west neighbor
  float token;        // The number from datos.dat for this process

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  if (numtasks != L*L && rank == 0){
    printf("Error: %d task are needed.\n", L*L);
    fflush(stdout);
    MPI_Abort(MPI_COMM_WORLD, 1);
    exit(1);
  }

  obtain_neighbors(&north, &south, &east, &west, rank, numtasks);

  if(rank == 0){
    read_and_distribute_token(&token, rank, numtasks);
  }else{
    obtain_token(&token, rank);
  }

  // Algorithm
  int i, rc;
  float in_token;
  MPI_Status status;
  for (i = 0; i < L-1; i++){
    rc = MPI_Send(&token, 1, MPI_FLOAT, south, i, MPI_COMM_WORLD);
    if (rc != MPI_SUCCESS) {
       printf("Send error in task %d\n", rank);
       MPI_Abort(MPI_COMM_WORLD, rc);
       exit(1);
    }
    rc = MPI_Recv(&in_token, 1, MPI_FLOAT, north, MPI_ANY_TAG, MPI_COMM_WORLD,
                  &status);
    if (rc != MPI_SUCCESS) {
       printf("Receive error in task %d\n", rank);
       MPI_Abort(MPI_COMM_WORLD, rc);
       exit(1);
    }
    if(in_token < token){
      token = in_token;
    }
  }

  for (i = 0; i < L-1; i++){
    rc = MPI_Send(&token, 1, MPI_FLOAT, east, i, MPI_COMM_WORLD);
    if (rc != MPI_SUCCESS) {
       printf("Send error in task %d\n", rank);
       MPI_Abort(MPI_COMM_WORLD, rc);
       exit(1);
    }
    rc = MPI_Recv(&in_token, 1, MPI_FLOAT, west, MPI_ANY_TAG, MPI_COMM_WORLD,
                  &status);
    if (rc != MPI_SUCCESS) {
       printf("Receive error in task %d\n", rank);
       MPI_Abort(MPI_COMM_WORLD, rc);
       exit(1);
    }
    if(in_token < token){
      token = in_token;
    }
  }

  if(rank == 0){
    printf("El menor elemento de toda la red es %g.\n", token);
  }

  MPI_Finalize();
  exit(0);
}
