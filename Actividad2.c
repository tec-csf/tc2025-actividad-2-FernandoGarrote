#include <stdlib.h>
#include <stdio.h>

#define N 10

typedef struct {
  char * titulo;
  int paginas;
} Libro;

typedef void(*imp)(void*);
typedef void(*rec)(void*, size_t, size_t, imp);

void recorre(rec, void *, size_t, size_t, imp);

void forward(void *, size_t, size_t, imp);
void reverse(void *, size_t, size_t, imp);
void bidirectional(void *, size_t, size_t, imp);

void *begin(void *);
void *end(void *, size_t, size_t);
void *next(void *, void *, size_t, size_t);
void *prev(void *, void *, size_t, size_t);

void imprimeInt(void *);
void imprimeLibro(void *);
void freeL(void *);

void recorre(rec re, void *arr, size_t count, size_t size, imp imp) {
  (*re)(arr, count, size, imp);
}

void forward(void *arr, size_t count, size_t size, imp imp){
  printf("\n Forward:");
  void *last = end(arr, count, size);

  for (void *current = begin(arr); current <= last; current = next(current, arr, count, size)) {
      (*imp)(current);
  }
}

void reverse(void *arr, size_t count, size_t size, imp imp) {
  printf("\n Reverse:");
  void *first = begin(arr);

  for (void *current = end(arr, count, size); current >= first; current = prev(current, arr, count, size)) {
      (*imp)(current);
  }
}

void bidirectional(void *arr, size_t count, size_t size, imp imp) {
  printf("\n Bidirectional:");
  forward(arr, count, size, imp);
  reverse(arr, count, size, imp);
}

void *begin(void *arr) {
  return arr;
}

void *end(void *arr, size_t count, size_t size) {
  return arr + ((count * size) - size);
}

void *next(void *current, void *arr, size_t count, size_t size) {
  void *next = current + size;
  void *final = end(arr, count, size);
  if (current <= next) {
      return next;
  }
  else{
    return NULL;
  }
}

void *prev(void *current, void *arr, size_t count, size_t size) {
  void *prev = current - size;
  void *first = begin(arr);
  if (current >= prev) {
      return prev;
  }
  else{
    return NULL;
  }
}

void imprimeInt(void *current) {
  printf("\n %d", *(int *)current);
}

void imprimeLibro(void *current) {
  printf("\n %s tiene %d paginas", ((Libro *)current)->titulo, ((Libro *)current)->paginas);
}

void freeL(void *arr) {
  void *last = end(arr, N, sizeof(Libro));
  for (void *current = begin(arr); current <= last; current = next(current, arr, N, sizeof(Libro))) {
      free(((Libro *)current)->titulo);
  }
}

int main() {

  printf("Numeros int:");

  int * arrInt = (int *) malloc(N * sizeof(int));
  int * intL = arrInt + N;
  int * intA = arrInt;

  for (; intA < intL; ++intA) {
      *intA = rand() % 50;
  }

  recorre(&forward, arrInt, N, sizeof(int), &imprimeInt);
  recorre(&reverse, arrInt, N, sizeof(int), &imprimeInt);
  recorre(&bidirectional, arrInt, N, sizeof(int), &imprimeInt);


  printf("\n Libros:");

  Libro * arrLibro = (Libro *) malloc(N * sizeof(Libro));
  Libro * libL = arrLibro + N;
  Libro * libA = arrLibro;

  for (; libA < libL; ++libA) {
      libA->titulo = (char *) malloc(sizeof(char) * 50);
      sprintf(libA->titulo, "Libro numero %d", rand() % 50);
      libA->paginas = rand() % 100;
  }

  recorre(&forward, arrLibro, N, sizeof(Libro), &imprimeLibro);
  recorre(&reverse, arrLibro, N, sizeof(Libro), &imprimeLibro);
  recorre(&bidirectional, arrLibro, N, sizeof(Libro), &imprimeLibro);

  free(arrInt);
  freeL(arrLibro);
  free(arrLibro);

  return 0;
}
