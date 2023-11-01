// Alunos: Pedro Paulo Pereira Souza e Gabriel Rodrigues
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define LEITORES 5
#define ESCRITORES 2
pthread_mutex_t db;    // Mutex para controlar o acesso ao banco de dados
pthread_mutex_t mutex; // Mutex para controlar o acesso à variável leitor_lendo
pthread_cond_t cond;   // Variável de condição para bloquear os leitores quando um escritor está esperando
int leitor_lendo;      // Variável que mantém o número de leitores lendo atualmente
int escritor_esperando; // Variável que mantém o número de escritores esperando para escrever

void ler_bd(void);
void usar_bd(void);
void pensando_bd(void);
void escrevendo_bd(void);

void leitor()
{
    while (1)
    {
        pthread_mutex_lock(&mutex);      // Bloqueia o acesso à variável leitor_lendo
        while (escritor_esperando > 0)   // Se houver algum escritor esperando, espera até que ele termine de escrever
            pthread_cond_wait(&cond, &mutex);
        leitor_lendo = leitor_lendo + 1; // Incrementa o número de leitores lendo
        if (leitor_lendo == 1)
            pthread_mutex_lock(&db);  // Se for o primeiro leitor, bloqueia o acesso ao banco de dados
        pthread_mutex_unlock(&mutex); // Libera o acesso à variável leitor_lendo

        ler_bd(); // Realiza a leitura do banco de dados

        pthread_mutex_lock(&mutex);      // Bloqueia o acesso à variável leitor_lendo
        leitor_lendo = leitor_lendo - 1; // Decrementa o número de leitores lendo

        if (leitor_lendo == 0)
            pthread_mutex_unlock(&db); // Se não houver mais leitores, libera o acesso ao banco de dados
        pthread_mutex_unlock(&mutex);  // Libera o acesso à variável leitor_lendo

        usar_bd(); // Utiliza o conhecimento adquirido no banco de dados
    }
}

void escritor()
{
    while (1)
    {
        pensando_bd();           // O escritor pensa no que irá escrever
        pthread_mutex_lock(&mutex);      // Bloqueia o acesso à variável escritor_esperando
        escritor_esperando = escritor_esperando + 1; // Incrementa o número de escritores esperando
        pthread_mutex_unlock(&mutex);    // Libera o acesso à variável escritor_esperando

        pthread_mutex_lock(&db);         // Bloqueia o acesso ao banco de dados

        pthread_mutex_lock(&mutex);      // Bloqueia o acesso à variável escritor_esperando
        escritor_esperando = escritor_esperando - 1; // Decrementa o número de escritores esperando
        if (escritor_esperando == 0)
            pthread_cond_broadcast(&cond);   // Se não houver mais escritores esperando, acorda todos os leitores que estavam esperando
        pthread_mutex_unlock(&mutex);    // Libera o acesso à variável escritor_esperando

        escrevendo_bd();                 // Escreve no banco de dados

        pthread_mutex_unlock(&db);       // Libera o acesso ao banco de dados
    }
}

void ler_bd()
{

    int tempo_de_leitura = rand() % 5; // Gera um tempo de leitura aleatório

    printf("Usuario lendo banco de dados. Total de %d leitores lendo agora.\n", leitor_lendo);
    sleep(tempo_de_leitura); // Simula a leitura do banco de dados
}

void usar_bd()
{

    int tempo_de_uso = rand() % 10; // Gera um tempo de uso aleatório

    printf("Utilizando conhecimento adquirido no Banco de dados.\n");
    sleep(tempo_de_uso); // Simula o uso do conhecimento adquirido no banco de dados
}

void pensando_bd()
{

    int tempo_para_pensar = rand() % 10; // Gera um tempo para pensar aleatório

    printf("Escritor pensando no que irá escrever.\n");
    sleep(tempo_para_pensar); // Simula o tempo que o escritor pensa no que irá escrever
}

void escrevendo_bd()
{

    int tempo_de_escrita = rand() % 10; // Gera um tempo de escrita aleatório

    printf("Escritor escrevendo no banco de dados.\n");
    sleep(tempo_de_escrita); // Simula a escrita no banco de dados
}

int main()
{

    pthread_t threadsdosescritores[ESCRITORES], threadsdosleitores[LEITORES];
    int i;

    pthread_mutex_init(&db, NULL);    // Inicializa o mutex do banco de dados
    pthread_mutex_init(&mutex, NULL); // Inicializa o mutex da variável leitor_lendo

    for (i = 0; i < ESCRITORES; i++)
    {

        pthread_create(&threadsdosescritores[i], NULL, (void *)escritor, NULL); // Cria as threads dos escritores
    }

    for (i = 0; i < LEITORES; i++)
    {

        pthread_create(&threadsdosleitores[i], NULL, (void *)leitor, NULL); // Cria as threads dos leitores
    }

    for (i = 0; i < ESCRITORES; i++)
    {

        pthread_join(threadsdosescritores[i], NULL); // Aguarda o término das threads dos escritores
    }

    for (i = 0; i < LEITORES; i++)
    {

        pthread_join(threadsdosleitores[i], NULL); // Aguarda o término das threads dos leitores
    }

    return 0;
}
