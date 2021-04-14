#include <pthread.h>
#include <stdio.h>
#include <assert.h>

/*
Convertir le code du fichier openmp.c exo1 en code utilisant uniquement les fonctions pthread
*/
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
  expliquer les valeur des adresses de chacune des variables suivantes: a,b,c,d. Expliquer pourquoi elle sont différentes ou identiques
*/
#define NB_THREADS 4
int a;
__thread int b;

void* func(void* arg){
  static int c;
  int d;
  static __thread int e;
 
 fprintf(stderr,"\tThread %p a %p\n",(void*)pthread_self(),&a);
 fprintf(stderr,"\tThread %p b %p\n",(void*)pthread_self(),&b);
 fprintf(stderr,"\tThread %p c %p\n",(void*)pthread_self(),&c);
 fprintf(stderr,"\tThread %p d %p\n",(void*)pthread_self(),&d);
 fprintf(stderr,"\tThread %p e %p\n",(void*)pthread_self(),&e);
}

void exo2(){
  pthread_t tid[NB_THREADS];
  int i;

  for(i = 0; i < NB_THREADS; i++){
    pthread_create(&(tid[i]),NULL,func,NULL);
  }
    
  for(i = 0; i < NB_THREADS; i++){
    pthread_join(tid[i], NULL);
  }
  
  fprintf(stderr,"Thread %p a %p\n",(void*)pthread_self(),&a);
  fprintf(stderr,"Thread %p b %p\n",(void*)pthread_self(),&b);
    
}
int main(int argc, char** argv){
  fprintf(stderr,"PThread\n");
  
  exo1();
  exo2();
  
  return 0; 
}
