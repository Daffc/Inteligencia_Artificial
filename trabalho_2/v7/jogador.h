#include <stdio.h>
#include <stdlib.h>

#ifndef JOGADOR
#define JOGADOR

    
    #define MAXSTR 512
    #define MAXINT 16
    #define GOL '-'

	// Estrutura utilizada para armazenar informações reecebidas de servidor.
	typedef struct Jogada{
        char campo[MAXSTR]; 
        char lado_meu;
        char lado_adv;
        char mov_adv;
        int tam_campo;
        int pos_filo;
        int pos_bola[MAXINT];
        int num_saltos;
	}Jogada;

#endif