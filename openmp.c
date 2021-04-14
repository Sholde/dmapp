#include <omp.h>
#include <stdio.h>
#include <assert.h>

/*Paralléliser avec OpenMP le calcul de la somme suivante*/
#define max_iter_exo1 1000
void exo1(){
  int i;
  int somme = 0;
  for(i = 0; i < max_iter_exo1; i++){
    somme += i; 
  }

  /*Vérification du résultat*/
  fprintf(stderr,"Somme %d attendu %d\n",somme, ((max_iter_exo1-1)*max_iter_exo1)/2);
  assert(somme == ((max_iter_exo1-1)*max_iter_exo1)/2);
}


/*
  Expliquer les valeurs des adresses de chacune des variables suivantes: a,b,c,d. Expliquer pourquoi elle sont différentes ou identiques.
*/
int a;
int b;
#pragma threadprivate(b)
void exo2(){
  static int c;
  int d;
  int e;
  int f; 
#pragma omp parallel private(e) shared(f)
  {
    fprintf(stderr,"\tThread %d a %p\n",omp_get_thread_num(),&a);
    fprintf(stderr,"\tThread %d b %p\n",omp_get_thread_num(),&b);
    fprintf(stderr,"\tThread %d c %p\n",omp_get_thread_num(),&c);
    fprintf(stderr,"\tThread %d d %p\n",omp_get_thread_num(),&d);
    fprintf(stderr,"\tThread %d e %p\n",omp_get_thread_num(),&e);
    fprintf(stderr,"\tThread %d f %p\n",omp_get_thread_num(),&f);
  }
    fprintf(stderr,"Thread %d a %p\n",omp_get_thread_num(),&a);
    fprintf(stderr,"Thread %d b %p\n",omp_get_thread_num(),&b);
    fprintf(stderr,"Thread %d c %p\n",omp_get_thread_num(),&c);
    fprintf(stderr,"Thread %d d %p\n",omp_get_thread_num(),&d);
    fprintf(stderr,"Thread %d e %p\n",omp_get_thread_num(),&e);
    fprintf(stderr,"Thread %d f %p\n",omp_get_thread_num(),&f);
}

int main(int argc, char** argv){
  fprintf(stderr,"OpenMP\n");
  
  exo1();
  exo2();
  
  return 0; 
}
