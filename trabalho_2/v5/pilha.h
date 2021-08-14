#include "jogador.h"

#ifndef __PILHA__
#define __PILHA__

    typedef struct EntPilhaFil{
        int salto;
        int posicao;
	}EntPilhaFil;

	typedef struct PilhaFil{
        EntPilhaFil pilha[MAXSTR];
        int topo;
	}PilhaFil;

    // Imprimeindo pilha de filosofos.
    void imprimePilhaFil(PilhaFil *pilha);

    // Inicializando pilha de filosofos vazia.
    void inicializaPilha(PilhaFil *pilha);

    // Empilhando nova entrada para filosofo em 'pilha', removido em 'salto' e em posição 'posicao'. 
    void empilhaFil(PilhaFil *pilha, int salto, int posicao);

    // Desempilhando informações de filosofo e retornonado ponteiro para entrada.
    EntPilhaFil* desempilhaFil(PilhaFil *pilha);
    
#endif
