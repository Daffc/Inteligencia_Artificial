// A C Program to demonstrate adjacency list
// representation of graphs
#include <stdio.h>
#include <stdlib.h>

#ifndef JOGADOR
#define JOGADOR

    
    #define MAXSTR 512
    #define MAXINT 16

	// Represents a node in te node list.
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