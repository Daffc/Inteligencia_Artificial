#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "campo.h"
#include "jogador.h"

#define MAXSTR 512
#define MAXINT 16
#define GOL '-'
 
// Lê string 'entrada' e popula variável 'jogada' de estrutura Jogada de acordo com o conteúdo de 'entrada'.
void recuperaJogada(char *entrada, Jogada *jogada){
  int i;

  // separa os elementos do string recebido
  sscanf(strtok(entrada, " \n"), "%c", &(jogada->lado_meu));
  sscanf(strtok(NULL, " \n"), "%d", &(jogada->tam_campo));
  sscanf(strtok(NULL, " \n"), "%s", jogada->campo);
  sscanf(strtok(NULL, " \n"), "%c", &(jogada->lado_adv));
  sscanf(strtok(NULL, " \n"), "%c", &(jogada->mov_adv));
  if(jogada->mov_adv != 'n') {
    if(jogada->mov_adv == 'f')
      sscanf(strtok(NULL, " \n"), "%d", &(jogada->pos_filo));
    else {
      sscanf(strtok(NULL, " \n"), "%d", &(jogada->num_saltos));
      for(i = 0; i < jogada->num_saltos; i++)
    sscanf(strtok(NULL, " \n"), "%d", &(jogada->pos_bola[i]));
    }
  }
}

// Imprime Estrutura Jogada.
void imprimeJogada(Jogada *jogada){
  int i;

  printf("---------------------------\n");
  // mostra o que recebeu
  printf("%c %d %s\n", jogada->lado_meu, jogada->tam_campo, jogada->campo);
  printf("%c %c", jogada->lado_adv, jogada->mov_adv);
  if(jogada->mov_adv != 'n') {
    if(jogada->mov_adv == 'f')
      printf(" %d", jogada->pos_filo);
    else {
      printf(" %d", jogada->num_saltos);
      for(i = 0; i < jogada->num_saltos; i++)
      printf(" %d", jogada->pos_bola[i]);
    }
  }
  printf("\n");
}


// Retorna valor de posição da bola tendo como origem o meio do campo (valor 0).
int valorBola(char *campo, Jogada *jogada){
  int i, meio_campo;
  
  // Armazena Posição de Meio de Campo.
  meio_campo = (jogada->tam_campo + 2) / 2;

  // Caso jogador esteja do lado esquerdo.
  if(jogada->lado_meu == 'e')
    // Busca em vetor posição de bola.
    for(i=0; i < (jogada->tam_campo + 2); i++){
      // Encontrando posição de bola, retorna posição - meio_campo.
      if (campo[i] == 'o')
        return (i - meio_campo);
    }
  // Caso jogador esteja do lado direito.
  else{
    // Busca em vetor posição de bola.
    for(i=0; i < (jogada->tam_campo + 2); i++){
      // Encontrando posição de bola, retorna posição - meio_campo.
      if (campo[i] == 'o')
        return -(i - meio_campo);
    }
  }
  return 0;
}

// Retorna 1 se bola ocupa posição de gols (bordas), caso contrário retorna 0.
int fimJogo(char *campo, Jogada *jogada){
  if(campo[0] == 'o' || campo[jogada->tam_campo + 1] == 'o'){
    return 1;
  }
  return 0;
}

void insereFilosofo(char *campo, Jogada *jogada){
  int i;

  // Percorrendo todas as posições entre os gols.
  for(i=1; i < (jogada->tam_campo + 1); i++){
    // Caso espaço esteja disponível , inserir filósofo.
    if (campo[i] == '.'){
      // Insere filósofo em posiçã livre.
      campo[i] = 'f';

      // Trabalha com novo campo obtido (CHAMADA RECURSIVA).
      printf("\t INSERE 'f' (%c f %d): %s\n", jogada->lado_meu, i, campo);

      // Retorna Campo para estado anterior.
      campo[i] = '.';
    }
  }
}

// void geraChutes(char *campo, Jogada *jogada){
//   int i;

//   // Busca em vetor posição de bola.
//   for(i=0; i < (jogada->tam_campo + 2); i++){
//     // Encontrando posição de bola, retorna posição - meio_campo.
//     if (campo[i] == 'o')
//       return (i - meio_campo);
//   }
//   // Percorrendo todas as posições entre os gols.
//   for(i=1; i < (jogada->tam_campo + 1); i++){
//     // Caso espaço esteja disponível , inserir filósofo.
//     if (campo[i] == '.'){
//       // Insere filósofo em posiçã livre.
//       campo[i] = 'f';

//       // Trabalha com novo campo obtido (CHAMADA RECURSIVA).
//       printf("\t INSERE 'f' [%d]: %s\n", i, campo);

//       // Retorna Campo para estado anterior.
//       campo[i] = '.';
//     }
//   }
// }
// int minimax(int profundidade, char jogadorMax, char *campo, char ladoJogador, Jogada *jogada){

//   // Se limite de profundidade alcançado
//   if(profundidade = 0 || fimJogo(campo)){
//     //Retornar posição/valor de bola.
//     return valorBola(campo, jogada);
//   }

//   int valor;
//   // Se jogador (Maximização).
//   if(jogadorMax){

//   }
//   // Jogador adversário (Minimização).
//   else{ 

//   }
// }

// Recebe campo e retorna ponteiro de string para próxima jogada elaborada.
void elaboraJogada(char * resposta, char *strJogadaAdv){
  Jogada jogadaAdv;
  char *campo_trabalho;

  // Recebendo jogada de adversário e armazenando em estrutura Jogada (jogada).
  recuperaJogada(strJogadaAdv, &jogadaAdv);

  // Imprimindo Jogada.
  // imprimeJogada(&jogadaAdv);

  // Copiando campo recebido para campo_trabalho (com bordas para representar gols).
  campo_trabalho = (char*) malloc(jogadaAdv.tam_campo + 3);
  
  campo_trabalho[0] = GOL;
  memcpy(&campo_trabalho[1], jogadaAdv.campo, jogadaAdv.tam_campo);
  campo_trabalho[jogadaAdv.tam_campo + 1] = GOL; 
  campo_trabalho[jogadaAdv.tam_campo + 2] = '\0'; 

  // TODO: Utilizar em função recursiva mini/max.
  insereFilosofo(campo_trabalho, &jogadaAdv);

  // TODO: Utilizar em função recursiva mini/max.
  // geraChutes(campo_trabalho, &jogadaAdv);

  printf("VALOR: %d\n", valorBola(campo_trabalho, &jogadaAdv));

  // Lendo entrada padrão.
  fgets(resposta, MAXSTR, stdin);
  resposta[strlen(resposta) -1] = '\0';

  free(campo_trabalho);
  return;
}


int main(int argc, char **argv) {
  char buf[MAXSTR];
  char *resposta;

  resposta = (char *) malloc(MAXSTR);
  
  campo_conecta(argc, argv);

  while(1) {
    // Recebendo e imprimindo campo.
    campo_recebe(buf);
    printf("%s", buf);


    elaboraJogada(resposta, buf);

    // caso '0', sair.
    if(resposta[0] == '0')
      break;

    sprintf(buf, "%s\n", resposta);


    // Envia jogada a campo.
    campo_envia(buf);
  }
  
  free(resposta);
}
