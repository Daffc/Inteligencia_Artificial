// TODO:  Remover Linhas identificada com '//DEBUG'.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "campo.h"
#include "jogador.h"
#include <limits.h>

#define MAXSTR 512
#define MAXINT 16
#define GOL '-'
#define PROFUNDIDADE 7

// Variável Global para contágem de nós. //DEBUG.
unsigned int NOS_ARVORE; //DEBUG

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

int minimax(int profundidade, int nivel, char jogadorMax, char *campo, Jogada *jogada, char *respostaFinal, int alpha, int beta){

  // for(int j = 0; j < nivel; j++) //DEBUG
    // printf("  "); //DEBUG

  // printf("ALPHA: %d, BETA: %d\n", alpha, beta); //DEBUG

  NOS_ARVORE++; //DEBUG

  // Se limite de profundidade alcançado
  if(profundidade == nivel || fimJogo(campo, jogada)){
    //Retornar posição/valor de bola.
    return valorBola(campo, jogada);
  }

  int i, valor, pos_bola, aux_pos_bola, pos_atual, saltos, saltos_pos[jogada->tam_campo], result, pont, direcao_inicial;
  char *campo_aux, direcao;

  // Definindo campo auxiliar, que receberá as modificações.
  campo_aux = (char*) malloc(jogada->tam_campo + 3);


  //#######################################
  //  JOGADOR  - MAXIMIZAÇÃO.
  //#######################################
  if(jogadorMax){
    int podaAlpha;

    valor =  INT_MIN;
    podaAlpha = 0;
    // ===============================
    //      GERANDO SALTOS DE BOLA
    // ===============================
    // Busca em vetor posição de bola.
    for(i=0; i < (jogada->tam_campo + 2); i++){
      // Encontrando posição de bola, retorna posição - meio_campo.
      if (campo[i] == 'o')
        break;
    }

    // Armazenando posição de bola.
    pos_bola = i;

    // Verifica o lado de jogador, indicando para iniciar chutes por lado oposto.
    if(jogada->lado_meu == 'd'){
      direcao_inicial = -1;
    }
    else
      direcao_inicial = 1;

    // Definindo direção inicial de saltos ( para esquerda ).
    direcao = direcao_inicial;

    // Loop entre direções (primeiramente esquerda com direção =-1 e )
    do{
      // Copiando campo para campo auxiliar.
      memcpy(campo_aux, campo, jogada->tam_campo + 3);

      // Zerando contador de saltos. definindo posição atual a frente de bola (de acordo com direcao). E armazenando posição de bola em variável auxiliar.
      saltos = 0;
      pos_atual = pos_bola + direcao;
      aux_pos_bola = pos_bola;

      // Enquanto houver filósofo ao lado 'direção' da bola.
      while(campo_aux[pos_atual] == 'f' && !podaAlpha){

        // Redefine bola como espaço vago.
        campo_aux[aux_pos_bola] = '.';

        // Buscando próximo espaço vago ('.') ou GOL ('-') e substituindo filosofos ('f') por espaço vago.
        while(campo_aux[pos_atual] == 'f'){
          campo_aux[pos_atual] = '.';
          pos_atual += direcao;
        }

        // Armazenando destino de salto encontrado e atualizando contador de saltos.
        saltos_pos[saltos] = pos_atual;
        saltos ++;

        // Redefinindo novo espaço encontrado como bola e armazenando nova posição de bola.
        campo_aux[pos_atual] = 'o';
        aux_pos_bola = pos_atual;

        //#######################################
        // Chamando Filho Recursivamente e armazenando valor em 'result'.
        result = minimax(profundidade, nivel + 1 , 0, campo_aux, jogada, respostaFinal, alpha, beta);
        // Caso 'result' maximize 'valor'.
        if(!nivel) //DEBUG
          printf("CAMPO VERIFICADO: %s\tVALOR: %d\n", campo_aux, result);  //DEBUG
        if (result >= valor){
          // Atualizando 'valor'.
          valor = result;

          // Caso seja nó de nível raiz (nivel = 0), escrever jogada resposta e buffer 'respostaFinal'.
          if(!nivel){
            pont = snprintf(respostaFinal, MAXSTR, "%c o %d ", jogada->lado_meu, saltos);
            for(i=0; i < saltos; i++){
              pont += snprintf(&respostaFinal[pont], MAXSTR - pont,"%d ", saltos_pos[i]);
            }
            snprintf(&respostaFinal[pont], MAXSTR - pont,"\n");
            printf("CAMPO ESCOLHIDO: %s\tVALOR: %d\n", respostaFinal, valor);  //DEBUG
          }
        }
        // Atualizando alpha.
        if (alpha < result){
          alpha = result;
        }

        // Verifica se possível poda
        if(beta <= alpha){
          podaAlpha = 1;
        }

        
        //#######################################

        // Redefinindo posição atual a frente de bola (de acordo com direcao) para nova análise.
        pos_atual += direcao;
      }

      // invertendo direção dos saltos.
      direcao *= -1;
    }
    while(direcao != direcao_inicial && !podaAlpha);

    // ===============================
    //      GERANDO INSERE FILÓSOFO
    // ===============================

    // Caso jogador esteja dolado direito, inserção a partir do lado esquerdo.
    if(jogada->lado_meu == 'd'){
      // Percorrendo todas as posições entre os gols da esquerda para direita.
      for(i=1; (i < (jogada->tam_campo + 1)) && !podaAlpha; i++){
        // Caso espaço esteja disponível , inserir filósofo.
        if (campo[i] == '.'){
          // Insere filósofo em posição livre.
          campo[i] = 'f';

          //#######################################
          // TRABALHAR COM NOVO CAMPO OBTIDO (CHAMADA RECURSIVA).
          
          // printf("JG D CAMPO FIL: %s\n", campo); //DEBUG
          // Chamando Filho Recursivamente e armazenando valor em 'result'.
          result = minimax(profundidade, nivel + 1 , 0, campo, jogada, respostaFinal, alpha, beta);
          // Caso result maximize 'valor'.
          if (result > valor){
            // Atualizando 'valor'.
            valor = result;

            // Caso seja nó de nível raiz (nivel = 0), escrever jogada resposta e buffer 'respostaFinal'.
            if(!nivel){
              snprintf(respostaFinal, MAXSTR, "%c f %d\n", jogada->lado_meu, i);
            }
          }

          // Atualizando alpha.
          if (alpha < result){
            alpha = result;
          }
          
          // Verifica se possível poda
          if(beta <= alpha){
            podaAlpha = 1;
          }

          //#######################################
          
          // Retorna Campo para estado anterior.
          campo[i] = '.';
        }
      }
    }
    // Caso jogador esteja do lado esquerdo ('e'), inserção a partir do lado direito.
    else{
      // Percorrendo todas as posições entre os gols da direita para esquerda.
      for(i=jogada->tam_campo; (i > 0) && !podaAlpha; i--){
        // Caso espaço esteja disponível , inserir filósofo.
        if (campo[i] == '.'){
          // Insere filósofo em posição livre.
          campo[i] = 'f';

          // printf("JG E CAMPO FIL: %s\n", campo); //DEBUG
          // Chamando Filho Recursivamente e armazenando valor em 'result'.
          result = minimax(profundidade, nivel + 1 , 0, campo, jogada, respostaFinal, alpha, beta);
          // Caso 'result' maximize 'valor'.
          if (result > valor){
            // Atualizando 'valor'.
            valor = result;

            // Caso seja nó de nível raiz (nivel = 0), escrever jogada resposta e buffer 'respostaFinal'.
            if(!nivel){
              snprintf(respostaFinal, MAXSTR, "%c f %d\n", jogada->lado_meu, i);
            }
          }

          // Atualizando alpha.
          if (alpha < result){
            alpha = result;
          }
          
          // Verifica se possível poda
          if(beta <= alpha){
            podaAlpha = 1;
          }

          // Retorna Campo para estado anterior.
          campo[i] = '.';
        }
      }
    }
  }

  //#######################################
  //  JOGADOR ADVERSÁRIO - MINIMIZAÇÃO.
  //#######################################
  else{
    int podaBeta;

    valor =  INT_MAX;
    podaBeta = 0;
    // ===============================
    //      GERANDO SALTOS DE BOLA
    // ===============================
    // printf("\tGERA CHUTES:\n");  //DEBUG
    // Busca em vetor posição de bola.
    for(i=0; i < (jogada->tam_campo + 2); i++){
      // Encontrando posição de bola, retorna posição - meio_campo.
      if (campo[i] == 'o')
        break;
    }

    // Armazenando posição de bola.
    pos_bola = i;

    // Verifica o lado de jogador adversário, indicando para iniciar chutes por lado oposto.
    if(jogada->lado_adv == 'd'){
      direcao_inicial = -1;
    }
    else
      direcao_inicial = 1;
    
    
    // Definindo direção inicial de saltos ( para esquerda ).
    direcao = direcao_inicial;

    // Loop entre direções (primeiramente esquerda com direção =-1 e )
    do{
      // Copiando campo para campo auxiliar.
      memcpy(campo_aux, campo, jogada->tam_campo + 3);

      // Zerando contador de saltos. definindo posição atual a frente de bola (de acordo com direcao). E armazenando posição de bola em variável auxiliar.
      saltos = 0;
      pos_atual = pos_bola + direcao;
      aux_pos_bola = pos_bola;

      // Enquanto houver filósofo a esquerda da bola.
      while(campo_aux[pos_atual] == 'f' && !podaBeta){

        // Redefine bola como espaço vago.
        campo_aux[aux_pos_bola] = '.';

        // Buscando próximo espaço vago ('.') ou GOL ('-') e substituindo filosofos ('f') por espaço vago.
        while(campo_aux[pos_atual] == 'f'){
          campo_aux[pos_atual] = '.';
          pos_atual += direcao;
        }

        // Armazenando destino de salto encontrado e atualizando contador de saltos.
        saltos_pos[saltos] = pos_atual;
        saltos ++;

        // Redefinindo novo espaço encontrado como bola e armazenando nova posição de bola.
        campo_aux[pos_atual] = 'o';
        aux_pos_bola = pos_atual;

        //#######################################
        
        // Chamando Filho Recursivamente e armazenando valor em 'result'.
        result = minimax(profundidade, nivel + 1 , 1, campo_aux, jogada, respostaFinal, alpha, beta);
        // Caso 'result' minimize 'valor'.
        if (result <= valor){
          // Atualizando 'valor'.
          valor = result;
        }

        // Atualizando beta.
        if (beta > result){
          beta = result;
        }
        
        // Verifica se possível poda
        if(beta <= alpha){
          podaBeta = 1;
        }
        //#######################################

        // Redefinindo posição atual a frente de bola (de acordo com direcao) para nova análise.
        pos_atual += direcao;
      }

      // invertendo direção dos saltos.
      direcao *= -1;
    }
    while(direcao != direcao_inicial && !podaBeta);

    // ===============================
    //      GERANDO INSERE FILÓSOFO
    // ===============================

    // Caso jogador adversário esteja dolado esquerdo, inimigo efetua inserção a partir do lado esquerdo.
    if(jogada->lado_adv == 'd'){
      // Percorrendo todas as posições entre os gols.
      for(i=1; (i < (jogada->tam_campo + 1)) && !podaBeta; i++){
        // Caso espaço esteja disponível , inserir filósofo.
        if (campo[i] == '.'){
          // Insere filósofo em posição livre.
          campo[i] = 'f';

          //#######################################
          // TRABALHAR COM NOVO CAMPO OBTIDO (CHAMADA RECURSIVA).
          
          // Chamando Filho Recursivamente e armazenando valor em 'result'.
          result = minimax(profundidade, nivel + 1 , 1, campo, jogada, respostaFinal, alpha, beta);
          // Caso 'result' minimize 'valor'.
          if (result < valor){
            // Atualizando 'valor'.
            valor = result;
          }

          // Atualizando beta.
          if (beta > result){
            beta = result;
          }
          
          // Verifica se possível poda
          if(beta <= alpha){
            podaBeta = 1;
          }

          //#######################################
          
          // Retorna Campo para estado anterior.
          campo[i] = '.';
        }
      }  
    }
    // Caso jogador adversário esteja do lado direito, inimigo efetua inserção a partir do lado direito.
    else{
      // Percorrendo todas as posições entre os gols.
      for(i = jogada->tam_campo; (i > 0) && !podaBeta; i--){
        // Caso espaço esteja disponível , inserir filósofo.
        if (campo[i] == '.'){
          // Insere filósofo em posição livre.
          campo[i] = 'f';

          //#######################################
          // TRABALHAR COM NOVO CAMPO OBTIDO (CHAMADA RECURSIVA).
          
          // Chamando Filho Recursivamente e armazenando valor em 'result'.
          result = minimax(profundidade, nivel + 1 , 1, campo, jogada, respostaFinal, alpha, beta);
          // Caso 'result' minimize 'valor'.
          if (result < valor){
            // Atualizando 'valor'.
            valor = result;
          }

          // Atualizando beta.
          if (beta > result){
            beta = result;
          }
          
          // Verifica se possível poda
          if(beta <= alpha){
            podaBeta = 1;
          }

          //#######################################
          // Retorna Campo para estado anterior.
          campo[i] = '.';
        }
      }  
    }
  }

  free(campo_aux);
  return valor;
}

// Recebe campo e retorna ponteiro de string para próxima jogada elaborada.
void elaboraJogada(char *resposta, char *strJogadaAdv){
  Jogada jogadaAdv;
  char *campo_trabalho;
  char *tmp_resposta; //TODO: substituir variáveis 'tmp_resposta' por 'resposta' após testes. //DEBUG

  tmp_resposta = (char *) malloc(MAXSTR); //DEBUG

  // Recebendo jogada de adversário e armazenando em estrutura Jogada (jogada).
  recuperaJogada(strJogadaAdv, &jogadaAdv);

  // Imprimindo Jogada. //DEBUG
  // imprimeJogada(&jogadaAdv); //DEBUG

  // Copiando campo recebido para campo_trabalho (com bordas para representar gols).
  campo_trabalho = (char*) malloc(jogadaAdv.tam_campo + 3);
  
  campo_trabalho[0] = GOL;
  memcpy(&campo_trabalho[1], jogadaAdv.campo, jogadaAdv.tam_campo);
  campo_trabalho[jogadaAdv.tam_campo + 1] = GOL; 
  campo_trabalho[jogadaAdv.tam_campo + 2] = '\0'; 

  // CAMADA FUNÇÂO MINIMAX.
  NOS_ARVORE = 0;
  int valor_final = minimax(PROFUNDIDADE, 0, 1,  campo_trabalho, &jogadaAdv, tmp_resposta, INT_MIN, INT_MAX); //DEBUG
  
  printf("JOGADA ESCOLHIDA: %s \nVALOR: %d \nNÓS: %d\n", tmp_resposta, valor_final, NOS_ARVORE); //DEBUG

  // Lendo entrada padrão.
  fgets(resposta, MAXSTR, stdin); //DEBUG
  resposta[strlen(resposta) -1] = '\0';  //DEBUG
    
  free(campo_trabalho);
  free(tmp_resposta); //DEBUG
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

    printf("#######################################\n"); //DEBUG
  }
  
  free(resposta);
}
