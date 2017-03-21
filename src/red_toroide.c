/*****************************************************************************
* red_toroide.c                                                              *
* Autor: José Ángel Martín Baos.                                             *
* Diseño de Infraestructura de Red - Práctica 1                              *
*****************************************************************************/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define L 4
#define DATOS_FILE "datos.dat"

void read_and_distribute_token(int* token, int rank){
  FILE* file = fopen(DATOS_FILE , "r");
  int i = 0;
  int element = 0;
  while (!feof (file)){
    fscanf (file, "%d", &element);
    if(i == rank){
      *token = element;
      break;
    }
    i++;
  }
  fclose (file);
}

int main (int argc, char *argv[]){
  int numtasks,      // Number of MPI tasks
      rank,          // Task number of the current process
      north,          // Rank of the north neighbor
      south,         // Rank of the south neighbor
      east,          // Rank of the east neighbor
      west,          // Rank of the west neighbor
      token;         // The number from datos.dat for this process

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  if (numtasks != L*L){
    printf("Error: %d task are needed.", numtasks);
    exit(1);
  }

  north = (rank + L) % numtasks;
  south = (rank - L) % numtasks;
  east = (rank - 1) % numtasks;
  west = (rank + 1) % numtasks;

  read_and_distribute_token(&token, rank);

  // Algorithm
  int i;
  for (i = 0; i < L-1; i++){
    printf("Nodo %d, valor %d\n", rank,token);
  }

  MPI_Finalize();
  exit(0);
}
