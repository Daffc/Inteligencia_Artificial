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

  printf("\tINSERE FILOSOFOS:\n");  //DEBUG
  // Percorrendo todas as posições entre os gols.
  for(i=1; i < (jogada->tam_campo + 1); i++){
    // Caso espaço esteja disponível , inserir filósofo.
    if (campo[i] == '.'){
      // Insere filósofo em posiçã livre.
      campo[i] = 'f';

      //#######################################
      // TRABALHAR COM NOVO CAMPO OBTIDO (CHAMADA RECURSIVA).
      printf("\t\t INSERE 'f' (%c f %d): %s\n", jogada->lado_meu, i, campo);  //DEBUG
      //#######################################
      
      // Retorna Campo para estado anterior.
      campo[i] = '.';
    }
  }
}

void geraChutes(char *campo, Jogada *jogada){
  int i;
  int pos_bola, aux_pos_bola, pos_atual;
  int saltos, saltos_pos[jogada->tam_campo];
  char *campo_aux;

  pos_bola = 0;
  printf("\tGERA CHUTES:\n");  //DEBUG
  // Busca em vetor posição de bola.
  for(i=0; i < (jogada->tam_campo + 2); i++){
    // Encontrando posição de bola, retorna posição - meio_campo.
    if (campo[i] == 'o'){
      pos_bola = i;
      break;
    }
  }

  // Definindo campo auxiliar, que receberá as modificações.
  campo_aux = (char*) malloc(jogada->tam_campo + 3);
  
  // Copiando campo para campo auxiliar.
  memcpy(campo_aux, campo, jogada->tam_campo + 3);

  // Zerando contador de saltos. definindo posição atual a esquerda de bola. E armazenando posição de bola em variável auxiliar.
  saltos = 0;
  pos_atual = pos_bola - 1;
  aux_pos_bola = pos_bola;

  printf("\t\tSALTOS A ESQUERDA:\n");  //DEBUG

  // Enquanto houver filósofo a esquerda da bola.
  while(campo_aux[pos_atual] == 'f'){

    // Redefine bola como espaço vago.
    campo_aux[aux_pos_bola] = '.';

    // Buscando próximo espaço vago ('.') ou GOL ('-') e substituindo filosofos ('f') por espaço vago.
    while(campo_aux[pos_atual] == 'f'){
      campo_aux[pos_atual] = '.';
      pos_atual --;
    }

    // Armazenando destino de salto encontrado e atualizando contador de saltos.
    saltos_pos[saltos] = pos_atual;
    saltos ++;

    // Redefinindo novo espaço encontrado como bola e armazenando nova posição de bola.
    campo_aux[pos_atual] = 'o';
    aux_pos_bola = pos_atual;

    //#######################################
    // TRABALHAR COM NOVO CAMPO OBTIDO (CHAMADA RECURSIVA).
    printf("\t\t\tSALTO ESQUERDA (%c o %d ", jogada->lado_meu, saltos);  //DEBUG
    for(i=0; i < saltos; i++){
      printf("%d ", saltos_pos[i]);  //DEBUG
    }
    printf("): %s\n", campo_aux); //DEBUG
    //#######################################

    // Redefinindo ponteiro para esquerda de nova posição de bola(aux_pos_bola - 1).
    pos_atual --;
  }

  // Copiando campo para campo auxiliar.
  memcpy(campo_aux, campo, jogada->tam_campo + 3);

  // Zerando contador de saltos. definindo posição atual a direita de bola. E armazenando posição de bola em variável auxiliar.
  saltos = 0;
  pos_atual = pos_bola + 1;
  aux_pos_bola = pos_bola;

  printf("\t\tSALTOS A DIREITA:\n");  //DEBUG

  // Enquanto houver filósofo a direita da bola.
  while(campo_aux[pos_atual] == 'f'){

    // Redefine bola como espaço vago.
    campo_aux[aux_pos_bola] = '.';

    // Buscando próximo espaço vago ('.') ou GOL ('-') e substituindo filosofos ('f') por espaço vago.
    while(campo_aux[pos_atual] == 'f'){
      campo_aux[pos_atual] = '.';
      pos_atual ++;
    }

    // Armazenando destino de salto encontrado e atualizando contador de saltos.
    saltos_pos[saltos] = pos_atual;
    saltos ++;

    // Redefinindo novo espaço encontrado como bola e armazenando nova posição de bola.
    campo_aux[pos_atual] = 'o';
    aux_pos_bola = pos_atual;

    //#######################################
    // TRABALHAR COM NOVO CAMPO OBTIDO (CHAMADA RECURSIVA).
    printf("\t\t\tSALTO DIREITA (%c o %d ", jogada->lado_meu, saltos);  //DEBUG
    for(i=0; i < saltos; i++){
      printf("%d ", saltos_pos[i]);  //DEBUG
    }
    printf("): %s\n", campo_aux); //DEBUG
    //#######################################

    // Redefinindo ponteiro para direita de nova posição de bola(aux_pos_bola + 1).
    pos_atual ++;
  }

  free(campo_aux);
}


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
  geraChutes(campo_trabalho, &jogadaAdv);

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

    printf("#######################################-------------------\n");
  }
  
  free(resposta);
}
