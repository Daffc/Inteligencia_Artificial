#include <stdio.h>
#include "pilha.h"

// Imprimeindo pilha de filosofos.
void imprimePilhaFil(PilhaFil *pilha){

  printf("PILHA:\n");
  for(int i = 0; i <= pilha->topo; i++)
    printf("\tENTRADA: %d SALTO:%d POSICAO: %d\n", i, pilha->pilha[i].salto, pilha->pilha[i].posicao);
}

// Inicializando pilha de filosofos vazia.
void inicializaPilha(PilhaFil *pilha){
  pilha->topo = -1;
}

// Empilhando nova entrada para filosofo em 'pilha', removido em 'salto' e em posição 'posicao'. 
void empilhaFil(PilhaFil *pilha, int salto, int posicao){
  pilha->topo ++;

  pilha->pilha[pilha->topo].salto = salto;
  pilha->pilha[pilha->topo].posicao = posicao;
}

// Desempilhando informações de filosofo e retornonado ponteiro para entrada.
EntPilhaFil* desempilhaFil(PilhaFil *pilha){

  EntPilhaFil *entrada;
  if(pilha->topo == -1)
    return NULL;

  entrada = &(pilha->pilha[pilha->topo]);

  pilha->topo --;

  return entrada;
}