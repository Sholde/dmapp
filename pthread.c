#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

// Thread structure to compute partial sum
typedef struct sum_s
{
  // sum
  int *res;
  int ibeg;
  int iend;

  // pthread
  pthread_mutex_t *mut;
} sum_t;

// It is the thread function for exo1
// It take in parameter the structure above
void *worker(void *arg)
{
  // Recup structure
  sum_t *sum = (sum_t *)arg;

  // Partial sum variale
  int i = 0;
  int somme = 0;
  
  // Partial sum
  for(i = sum->ibeg; i < sum->iend; i++)
    {
      somme += i; 
    }

  // Updating the final sum with our partial sum
  pthread_mutex_lock(sum->mut);
  {
    *(sum->res) += somme;
  }
  pthread_mutex_unlock(sum->mut);
}

/*
Convertir le code du fichier openmp.c exo1 en code utilisant uniquement les fonctions pthread
*/
#define max_iter_exo1 1000
void exo1(const int nthreads)
{
  // Define the number of thread
  int NB_THREADS = nthreads;

  // Dafault variable for sum
  int i = 0;
  int somme = 0;

  // Variable to help for pthread
  pthread_t tid[NB_THREADS];
  sum_t sum[NB_THREADS];
  pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

  int res = 0;
  int nb_ite = max_iter_exo1 / NB_THREADS;
  int remain = max_iter_exo1 % NB_THREADS;

  // Create all threads
  for(i = 0; i < NB_THREADS; i++)
    {
      // Init struct
      {
        sum[i].res = &res;

        // Note that is the same explanation that mpi (because we do the same thing)
        // We share the work the way to have just one more iteration if there are
        // remain value to compute.
        if (i < remain)
          {
            // Case we need to do nb_ite + 1 opertions
            nb_ite++;
            sum[i].ibeg = nb_ite * i;
            sum[i].iend = nb_ite * (i + 1);
          }
        else
          {
            // Case we need to do nb_ite operations
            // It is not very hard, we just need to add the number of times we make
            // nb_ite + 1 operation (this is on the left of plus), then we add the
            // number of times we make nb_ite until our rank.
            sum[i].ibeg = (nb_ite + 1) * remain + nb_ite * (i - remain);
            sum[i].iend = (nb_ite + 1) * remain + nb_ite * (i + 1 - remain);
          }

      sum[i].mut = &mut;
    }

    // Throw thread
    pthread_create(&(tid[i]), NULL, worker, sum + i);
  }

  // Waiting all threads
  for(i = 0; i < NB_THREADS; i++)
    {
      pthread_join(tid[i], NULL);
    }

  // Destroy the mutex
  pthread_mutex_destroy(&mut);

  // Affect to somme the result
  somme = *(sum[0].res);
  
  /*Vérification du résultat*/
  fprintf(stderr, "Somme %d attendu %d\n", somme, ((max_iter_exo1 - 1) * max_iter_exo1) / 2);
  assert(somme == ((max_iter_exo1 - 1) * max_iter_exo1) / 2);
}

/*
  expliquer les valeur des adresses de chacune des variables suivantes: a,b,c,d. Expliquer pourquoi elle sont différentes ou identiques
*/

int a;
__thread int b;

void *func(void *arg)
{
  static int c;
  int d;
  static __thread int e;

  //
  fprintf(stderr, "\tThread %p a %p\n", (void *)pthread_self(), &a);
  fprintf(stderr, "\tThread %p b %p\n", (void *)pthread_self(), &b);
  fprintf(stderr, "\tThread %p c %p\n", (void *)pthread_self(), &c);
  fprintf(stderr, "\tThread %p d %p\n", (void *)pthread_self(), &d);
  fprintf(stderr, "\tThread %p e %p\n", (void *)pthread_self(), &e);
}

void exo2(const int nthreads)
{
  // Get the number of threads
  int NB_THREADS = nthreads;

  // Allocate pthread structure
  pthread_t *tid = malloc(sizeof(pthread_t) * NB_THREADS);

  //
  int i;

  // Create thread
  for(i = 0; i < NB_THREADS; i++)
    {
      pthread_create(&(tid[i]), NULL, func, NULL);
    }

  // Waiting thread
  for(i = 0; i < NB_THREADS; i++)
    {
      pthread_join(tid[i], NULL);
    }

  // Print global value
  fprintf(stderr, "Thread %p a %p\n", (void *)pthread_self(), &a);
  fprintf(stderr, "Thread %p b %p\n", (void *)pthread_self(), &b);

  // Release pthread structure
  free(tid);
}

int main(int argc, char** argv)
{
  // Check argument
  if (argc != 2)
    return fprintf(stderr, "Usage: %s [NB_THREADS]\n", argv[0]), 1;

  //
  fprintf(stderr, "Bench name: PThread\n");

  // Get the number of threads
  int nthreads = atoi(argv[1]);

  // Run exo
  exo1(nthreads);
  exo2(nthreads);

  //
  return 0; 
}
