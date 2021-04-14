#include <mpi.h>
#include <stdio.h>
#include <assert.h>

int rank;
int size;


/*
  Paralléliser avec MPI le calcul de la somme suivante
  Chaque processus MPI devra traiter max_iter_exo1/size éléments
  Le processus 0 doit obtenir le résultat dans la variable somme
*/
#define max_iter_exo1 1000

void exo1(){
  int i; 
  int somme = 0;


  // Variable to help for MPI
  int ibeg = 0;
  int iend = 0;
  int nb_ite = max_iter_exo1 / size;
  int remain = max_iter_exo1 % size;
  int res = 0;

  // We share the work the way to have just one more iteration if there are
  // remain value to compute.
  if (rank < remain)
    {
      // Case we need to do nb_ite + 1 opertions
      nb_ite++;
      ibeg = nb_ite * rank;
      iend = nb_ite * (rank + 1);
    }
  else
    {
      // Case we need to do nb_ite operations
      // It is not very hard, we just need to add the number of times we make
      // nb_ite + 1 operation (this is on the left of plus), then we add the
      // number of times we make nb_ite until our rank.
      ibeg = (nb_ite + 1) * remain + nb_ite * (rank - remain);
      iend = (nb_ite + 1) * remain + nb_ite * (rank + 1 - remain);
    }

  // Reduce in local
  for(i = ibeg; i < iend; i++){
    somme += i; 
  }

  // Exchange mpi
  // Here I use reduce function because we want to do a reduction on the original code
  MPI_Reduce(&somme, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0)
    {
      /*Vérification du résultat*/
      fprintf(stderr, "Somme %d attendu %d\n", res, ((max_iter_exo1 - 1) * max_iter_exo1) / 2);
      /*Vérification du résultat de la somme*/
      assert(res == ((max_iter_exo1 - 1) * max_iter_exo1) / 2);
      /*Vérification du nombre d'iération réalisées par chaque processus MPI*/
      assert(i == max_iter_exo1/size);
    }
}


int main(int argc, char** argv){
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  if(rank == 0)
    fprintf(stderr,"MPI %d\n",size);

  /* Vérification du nombre de processus MPI*/
  assert(max_iter_exo1 % size == 0);

  exo1();
  
  
  MPI_Finalize();
  return 0; 
}
