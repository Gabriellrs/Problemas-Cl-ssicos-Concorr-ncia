#include<stdio.h> // Inclui a biblioteca padrão de entrada e saída
#include<stdlib.h> // Inclui a biblioteca padrão de funções gerais
#include<semaphore.h> // Inclui a biblioteca de semáforos para sincronização de threads
#include<pthread.h> // Inclui a biblioteca de threads POSIX

#define N 5  // Define a quantidade de filósofos
#define COMENDO 2 // Define o estado do filósofo como comendo
#define FOME 1 // Define o estado do filósofo como com fome
#define PENSANDO 0 // Define o estado do filósofo como pensando
#define ESQUERDA (filosofoID + 4) % N  // Define o índice do filósofo à esquerda
#define DIREITA (filosofoID + 1) % N   // Define o índice do filósofo à direita

sem_t semaforo1; // Semáforo para controlar o acesso aos recursos compartilhados
sem_t semaforo2[N]; // Array de semáforos, um para cada filósofo
int estado[N]; // Array para armazenar o estado atual de cada filósofo (comendo, com fome, pensando)
int filosofoID[N] = {0,1,2,3,4}; // Array para armazenar a identificação de cada filósofo

void *filosofo(void *num); // Função que representa as ações de um filósofo
void pegaGarfo(int);  // Função para pegar os garfos
void devolveGarfo(int);  // Função para devolver os garfos à mesa
void verificacao(int);  // Função para verificar se um filósofo pode comer

int main() {  // Função principal
  int i;
  pthread_t thread_id[N]; // Array para armazenar os identificadores das threads dos filósofos

  sem_init(&semaforo1, 0, 1); // Inicializa o semáforo principal com valor 1
  for (i=0; i<N; i++)
    sem_init(&semaforo2[i], 0, 0); // Inicializa os semáforos dos filósofos com valor 0
  for (i=0; i<N; i++) {
    pthread_create(&thread_id[i], NULL, filosofo, &filosofoID[i]); // Cria uma thread para cada filósofo
    printf("O %dº Filósofo esta pensando...\n", i + 1); // Imprime que o filósofo está pensando
  } 
  for (i=0; i<N; i++)
    pthread_join(thread_id[i], NULL); // Aguarda todas as threads terminarem
  return 0;
}

void *filosofo(void *num) { // Função que representa as ações de um filósofo
  while(1) {  
    int *i = num; 
    sleep(1);
    pegaGarfo(*i);
    sleep(1);
    devolveGarfo(*i);
  }
}

void devolveGarfo(int filosofoID) { 
  sem_wait(&semaforo1); //Decrementa no valor do semáforo p/ que só uma thread entre na região crítica
  estado[filosofoID] = PENSANDO; 
  printf("O %dº Filósofo comeu e devolveu os garfos que usou para comer...\n", filosofoID + 1);
  printf("O %dº Filósofo está pensando...\n", filosofoID + 1);
  verificacao(ESQUERDA); //Verifica se esses filósofos podem começar a comer agora que o filósofo atual devolveu os garfos.
  verificacao(DIREITA); 
  sem_post(&semaforo1); //Incrementa no valor do semáforo p/ que outra thread entre na região crítica
}

void pegaGarfo(int filosofoID) { 
  sem_wait(&semaforo1); 
  estado[filosofoID] = FOME; 
  printf("O %dº Filósofo está com fome...\n", filosofoID + 1);
  verificacao(filosofoID); //Se os filósofos à esquerda e à direita não estiverem comendo, o filósofo atual pode começar a comer.
  sem_post(&semaforo1); 
  sem_wait(&semaforo2[filosofoID]); //Se o semáforo for maior que 0, ele é decrementado. Isso faz com que a thread espere até que seja permitido comer.
}

void verificacao(int filosofoID) {  
if (estado[filosofoID] == FOME && estado[ESQUERDA] != COMENDO && estado[DIREITA] != COMENDO) { 
    estado[filosofoID] = COMENDO; 
    printf("O %dº Filósofo pegou os garfos %d e %d \n", filosofoID + 1, ESQUERDA + 1, filosofoID + 1);
    printf("O %dº Filósofo estava com fome e agora está comendo...\n", filosofoID + 1); 
    sem_post(&semaforo2[filosofoID]); 
  }
}

/*

Este código é uma implementação do problema clássico de sincronização conhecido como o problema dos filósofos jantando.

- Existem `N` filósofos sentados ao redor de uma mesa circular. Cada filósofo tem um prato de espaguete. Os garfos são colocados entre cada par de filósofos adjacentes.

- Cada filósofo pode alternar entre duas ações: pensar ou comer. No entanto, um filósofo só pode comer se tiver ambos os garfos (o da esquerda e o da direita).

- O estado de cada filósofo é rastreado usando o array `estado[]`, que pode ser `PENSANDO`, `FOME` ou `COMENDO`.

- Semáforos são usados para garantir que os filósofos não peguem os garfos ao mesmo tempo, o que poderia levar a condições de corrida. O semáforo `semaforo1` é usado para acesso exclusivo ao array `estado[]`. Os semáforos em `semaforo2[]` são usados para controlar se um filósofo pode começar a comer.

- A função `filosofo()` é executada por cada thread. Ela simula o ciclo de vida de um filósofo: pensar, ficar com fome, pegar garfos, comer e devolver garfos.

- As funções `pegaGarfo()` e `devolveGarfo()` são usadas para pegar e devolver garfos, respectivamente. Elas atualizam o estado do filósofo e imprimem mensagens apropriadas.

- A função `verificacao()` é chamada após um filósofo pegar ou devolver garfos. Ela verifica se o filósofo atual pode começar a comer (ou seja, se ambos os vizinhos não estão comendo) e, em caso afirmativo, atualiza seu estado e libera seu semáforo em `semaforo2[]`.

*/