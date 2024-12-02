# 🧵 Problemas Clássicos de Concorrência  

Este repositório contém implementações de dois problemas clássicos de concorrência em sistemas operacionais: **O Jantar dos Filósofos** e **Leitores e Escritores**. Esses exemplos demonstram o uso de mecanismos de sincronização e exclusão mútua para resolver conflitos em cenários de compartilhamento de recursos.

## 🛠️ Problemas Abordados  

### 🍽️ O Jantar dos Filósofos  
Um problema que ilustra o uso de recursos compartilhados (os garfos) por várias threads (os filósofos). O desafio é evitar condições de corrida, deadlocks e starvation enquanto os filósofos alternam entre pensar e comer.

### 📚 Leitores e Escritores  
Este problema foca na sincronização de acesso a um recurso compartilhado, garantindo que múltiplos leitores possam acessar o recurso simultaneamente, mas impedindo que escritores o modifiquem enquanto leitores ou outros escritores estão ativos.  

## 🚀 Tecnologias Utilizadas  

- **Linguagem:** C  
- **Biblioteca:** pthreads (POSIX Threads)  

