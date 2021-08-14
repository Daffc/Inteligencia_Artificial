// TODO:  Remover Linhas identificada com '//DEBUG'.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "campo.h"
#include "jogador.h"
#include <limits.h>
#include <time.h> //DEBUG

#define MAXSTR 512
#define MAXINT 16
#define GOL '-'
#define PROFUNDIDADE 10

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
      if (campo[i] == 'o'){
          return (i - meio_campo);
      }
    }
  // Caso jogador esteja do lado direito.
  else{
    // Busca em vetor posição de bola.
    for(i=0; i < (jogada->tam_campo + 2); i++){
      // Encontrando posição de bola, retorna posição - meio_campo.
      if (campo[i] == 'o'){
          return -(i - meio_campo);
      }
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

int geraSaltoCompleto(int *saltos_pos, char *campo, int pos_bola,  char direcao, char poda){

    int saltos;
    int pos_atual;

    // Zerando contador de saltos. definindo posição atual a frente de bola (de acordo com direcao). E armazenando posição de bola em variável auxiliar.
    saltos = 0;
    pos_atual = pos_bola + direcao;

    // Redefine bola como espaço vago.
    campo[pos_bola] = '.';

    // Enquanto houver filósofo ao lado 'direção' da bola.
    while(campo[pos_atual] == 'f' && !poda){

      // Buscando próximo espaço vago ('.') ou GOL ('-') e substituindo filosofos ('f') por espaço vago.
      while(campo[pos_atual] == 'f'){
        campo[pos_atual] = '.';
        pos_atual += direcao;
      }

      // Armazenando destino de salto encontrado e atualizando contador de saltos.
      saltos_pos[saltos] = pos_atual;
      saltos ++;

      pos_bola = pos_atual;

      // Redefinindo posição atual a frente de bola (de acordo com direcao) para nova análise.
      pos_atual += direcao;
    }

    // Redefinindo novo espaço encontrado como bola e armazenando nova posição de bola.
    campo[pos_bola] = 'o';

    return saltos;
}

int minimax(int profundidade, int nivel, char jogadorMax, char *campo, Jogada *jogada, char *respostaFinal, int alpha, int beta){

  // for(int j = 0; j < nivel; j++) //DEBUG
    // printf("  "); //DEBUG

  // printf("ALPHA: %d, BETA: %d\n", alpha, beta); //DEBUG

  NOS_ARVORE++; //DEBUG

  // Se limite de profundidade alcançado
  if(profundidade == nivel || fimJogo(campo, jogada)){
    //Retornar posição/valor de bola.
    // printf("CAMPO: %s, VALOR: %d\n", campo, valorBola(campo, jogada));
    return valorBola(campo, jogada);
  }

  // VARIAVESI MINMAX
  int i, valor, result;
  
  // VARIÁVEIS SALTO
  int pos_bola, saltos, saltos_pos[jogada->tam_campo], pont_escrita, direcao_inicial;
  char *campo_aux, direcao;
  
  // VARIÀVEIS FILOSOFOS.  
  int pos_ini, 
      sinal, 
      pos_fis;


  // Definindo campo auxiliar, que receberá as modificações de salto.
  campo_aux = (char*) malloc(jogada->tam_campo + 3);


  //#######################################
  //  JOGADOR  - MAXIMIZAÇÃO.
  //#######################################
  if(jogadorMax){
    int podaAlpha;

    valor =  INT_MIN;
    podaAlpha = 0;


    // Verifica o lado de jogador.
    if(jogada->lado_meu == 'd'){
      direcao_inicial = -1;         // Indica que saltos de bola devem iniciar para esquerda.
      pos_ini = 1;                  // Posição incial de inserção de filósofos (campo[1]).
      sinal = 1;                    // Direção na qual filosofos devem ser adicionados (ordem crescentes -> para direita).
    }
    else{
      direcao_inicial = 1;          // Indica que saltos de bola devem iniciar para direita.
      pos_ini = jogada->tam_campo;  // Posição incial de inserção de filósofos (campo[jogada->tam_campo]).
      sinal = -1;                   // Direção na qual filosofos devem ser adicionados (ordem decrescente -> para esquerda).
    }

    // ===============================
    //      GERANDO INSERE FILÓSOFO
    // ===============================
    // Percorrendo todas as posições entre os gols.
    for(i=0; (i < (jogada->tam_campo)) && !podaAlpha; i++){

      // Cacula posição de filósofo de acordo com lado de jogador.
      pos_fis = pos_ini + (i * sinal);

      // Caso espaço esteja disponível , inserir filósofo.
      if (campo[pos_fis] == '.'){
        // Insere filósofo em posição livre.
        campo[pos_fis] = 'f';

        //#######################################
        // TRABALHAR COM NOVO CAMPO OBTIDO (CHAMADA RECURSIVA).
        
        // Chamando Filho Recursivamente e armazenando valor em 'result'.
        result = minimax(profundidade, nivel + 1 , 0, campo, jogada, respostaFinal, alpha, beta);

        // if(!nivel) //DEBUG
          // printf("FIL CAMPO: %s, VALOR: %d\n", campo, result); //DEBUG
        // Caso result maximize 'valor'.
        if (result > valor){
          // Atualizando 'valor'.
          valor = result;

          // Caso seja nó de nível raiz (nivel = 0), escrever jogada resposta e buffer 'respostaFinal'.
          if(!nivel){
            snprintf(respostaFinal, MAXSTR, "%c f %d\n", jogada->lado_meu, pos_fis);
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
        campo[pos_fis] = '.';
      }
    }

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

    // Definindo direção inicial de saltos ( para esquerda ).
    direcao = direcao_inicial;


    // SALTANDO PARA FRENTE.

    // Loop entre direções (primeiramente esquerda com direção =-1 e )
    // Copiando campo para campo auxiliar.
    memcpy(campo_aux, campo, jogada->tam_campo + 3);

    // Itera sobre campo, executando o máximo de saltos possíveis em direção 'direcao', 
    // retornando 'campo_aux', 'saltos' e 'saltos_pos' atualizados.
    saltos = geraSaltoCompleto(saltos_pos, campo_aux, pos_bola, direcao, podaAlpha);

    //#######################################
    // Chamando Filho Recursivamente e armazenando valor em 'result'.
    result = minimax(profundidade, nivel + 1 , 0, campo_aux, jogada, respostaFinal, alpha, beta);
    // Caso 'result' maximize 'valor'.
    if (result > valor){
      // Atualizando 'valor'.
      valor = result;

      // Caso seja nó de nível raiz (nivel = 0), escrever jogada resposta e buffer 'respostaFinal'.
      if(!nivel){
        pont_escrita = snprintf(respostaFinal, MAXSTR, "%c o %d ", jogada->lado_meu, saltos);
        for(i=0; i < saltos; i++){
          pont_escrita += snprintf(&respostaFinal[pont_escrita], MAXSTR - pont_escrita,"%d ", saltos_pos[i]);
        }
        snprintf(&respostaFinal[pont_escrita], MAXSTR - pont_escrita,"\n");
        // printf("SALTO CAMPO: %s, VALOR: %d\n", campo_aux, valor); //DEBUG
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

    // invertendo direção dos saltos.
    direcao *= -1;

    // SALTANDO PARA TRAS.

    // Loop entre direções (primeiramente esquerda com direção =-1 e )
    // Copiando campo para campo auxiliar.
    memcpy(campo_aux, campo, jogada->tam_campo + 3);

    // Itera sobre campo, executando o máximo de saltos possíveis em direção 'direcao', 
    // retornando 'campo_aux', 'saltos' e 'saltos_pos' atualizados.
    saltos = geraSaltoCompleto(saltos_pos, campo_aux, pos_bola, direcao, podaAlpha);

    //#######################################
    // Chamando Filho Recursivamente e armazenando valor em 'result'.
    result = minimax(profundidade, nivel + 1 , 0, campo_aux, jogada, respostaFinal, alpha, beta);
    // Caso 'result' maximize 'valor'.
    if (result > valor){
      // Atualizando 'valor'.
      valor = result;

      // Caso seja nó de nível raiz (nivel = 0), escrever jogada resposta e buffer 'respostaFinal'.
      if(!nivel){
        pont_escrita = snprintf(respostaFinal, MAXSTR, "%c o %d ", jogada->lado_meu, saltos);
        for(i=0; i < saltos; i++){
          pont_escrita += snprintf(&respostaFinal[pont_escrita], MAXSTR - pont_escrita,"%d ", saltos_pos[i]);
        }
        snprintf(&respostaFinal[pont_escrita], MAXSTR - pont_escrita,"\n");
        // printf("SALTO CAMPO: %s, VALOR: %d\n", campo_aux, valor); //DEBUG
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

  }

  //#######################################
  //  JOGADOR ADVERSÁRIO - MINIMIZAÇÃO.
  //#######################################
  else{
    int podaBeta;

    valor =  INT_MAX;
    podaBeta = 0;

    // if(nivel == 1) //DEBUG
      // printf("CAMPO ENVIADO: %s\n", campo);

    // Verifica o lado de jogador.
    if(jogada->lado_adv == 'd'){
      direcao_inicial = -1;         // Indica que saltos de bola devem iniciar para esquerda.
      pos_ini = 1;                  // Posição incial de inserção de filósofos (campo[1]).
      sinal = 1;                    // Direção na qual filosofos devem ser adicionados (ordem crescentes -> para direita).
    }
    else{
      direcao_inicial = 1;          // Indica que saltos de bola devem iniciar para direita.
      pos_ini = jogada->tam_campo;  // Posição incial de inserção de filósofos (campo[jogada->tam_campo]).
      sinal = -1;                   // Direção na qual filosofos devem ser adicionados (ordem decrescente -> para esquerda).
    }

    // ===============================
    //      GERANDO INSERE FILÓSOFO
    // ===============================

    // Percorrendo todas as posições entre os gols.
    for(i=0; (i < (jogada->tam_campo)) && !podaBeta; i++){

      // Cacula posição de filósofo de acordo com lado de adversário.
      pos_fis = pos_ini + (i * sinal);
      
      // Caso espaço esteja disponível , inserir filósofo.
      if (campo[pos_fis] == '.'){
        // Insere filósofo em posição livre.
        campo[pos_fis] = 'f';
        
        //#######################################
        // TRABALHAR COM NOVO CAMPO OBTIDO (CHAMADA RECURSIVA).
        
        // Chamando Filho Recursivamente e armazenando valor em 'result'.
        result = minimax(profundidade, nivel + 1 , 1, campo, jogada, respostaFinal, alpha, beta);

        // if(nivel == 1) //DEBUG
          // printf("*MIN* FIL CAMPO: %s, VALOR: %d\n", campo, result); //DEBUG


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
        campo[pos_fis] = '.';
      }
    }

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
    
    
    // Definindo direção inicial de saltos ( para esquerda ).
    direcao = direcao_inicial;

    // SALTANDO FRENTE.

    // Copiando campo para campo auxiliar.
    memcpy(campo_aux, campo, jogada->tam_campo + 3);


    // Itera sobre campo, executando o máximo de saltos possíveis em direção 'direcao', 
    // retornando 'campo_aux', 'saltos' e 'saltos_pos' atualizados.
    saltos = geraSaltoCompleto(saltos_pos, campo_aux, pos_bola, direcao, podaBeta);


    //#######################################
    
    // Chamando Filho Recursivamente e armazenando valor em 'result'.
    result = minimax(profundidade, nivel + 1 , 1, campo_aux, jogada, respostaFinal, alpha, beta);
    // Caso 'result' minimize 'valor'.
    if (result < valor){
      // Atualizando 'valor'.
      valor = result;
    }

    // if(nivel == 1) //DEBUG
      // printf("*MIN* SAL CAMPO: %s, VALOR: %d\n", campo_aux, result); //DEBUG

    // Atualizando beta.
    if (beta > result){
      beta = result;
    }
    
    // Verifica se possível poda
    if(beta <= alpha){
      podaBeta = 1;
    }
    //#######################################

    // invertendo direção dos saltos.
    direcao *= -1;

    // SALTANDO TRAS

    // Copiando campo para campo auxiliar.
    memcpy(campo_aux, campo, jogada->tam_campo + 3);


    // Itera sobre campo, executando o máximo de saltos possíveis em direção 'direcao', 
    // retornando 'campo_aux', 'saltos' e 'saltos_pos' atualizados.
    saltos = geraSaltoCompleto(saltos_pos, campo_aux, pos_bola, direcao, podaBeta);


    //#######################################
    
    // Chamando Filho Recursivamente e armazenando valor em 'result'.
    result = minimax(profundidade, nivel + 1 , 1, campo_aux, jogada, respostaFinal, alpha, beta);
    // Caso 'result' minimize 'valor'.
    if (result < valor){
      // Atualizando 'valor'.
      valor = result;
    }

    // if(nivel == 1) //DEBUG
      // printf("*MIN* SAL CAMPO: %s, VALOR: %d\n", campo_aux, result); //DEBUG

    // Atualizando beta.
    if (beta > result){
      beta = result;
    }
    
    // Verifica se possível poda
    if(beta <= alpha){
      podaBeta = 1;
    }
    //#######################################

  }

  free(campo_aux);
  return valor;
}

// Recebe campo e retorna ponteiro de string para próxima jogada elaborada.
void elaboraJogada(char *resposta, Jogada *jogadaAdv){
  char *campo_trabalho;
  char *tmp_resposta; //TODO: substituir variáveis 'tmp_resposta' por 'resposta' após testes. //DEBUG

  clock_t start = clock(); //DEBUG

  tmp_resposta = (char *) malloc(MAXSTR); //DEBUG

  // Imprimindo Jogada. //DEBUG
  // imprimeJogada(&jogadaAdv); //DEBUG

  // Copiando campo recebido para campo_trabalho (com bordas para representar gols).
  campo_trabalho = (char*) malloc(jogadaAdv->tam_campo + 3);
  
  campo_trabalho[0] = GOL;
  memcpy(&campo_trabalho[1], jogadaAdv->campo, jogadaAdv->tam_campo);
  campo_trabalho[jogadaAdv->tam_campo + 1] = GOL; 
  campo_trabalho[jogadaAdv->tam_campo + 2] = '\0'; 

  NOS_ARVORE = 0; //DEBUG

  // CAMADA FUNÇÂO MINIMAX.
  int valor_final = minimax(PROFUNDIDADE, 0, 1,  campo_trabalho, jogadaAdv, tmp_resposta, INT_MIN, INT_MAX); //DEBUG
  
  printf("JOGADA ESCOLHIDA: %s \nVALOR: %d \nNÓS: %d\n", tmp_resposta, valor_final, NOS_ARVORE); //DEBUG

  clock_t end = clock(); //DEBUG
  printf("TEMPO: %f\n", (float)(end - start) / CLOCKS_PER_SEC);

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
  Jogada jogadaAdv;
  int i;

  resposta = (char *) malloc(MAXSTR);
  
  campo_conecta(argc, argv);

  while(1) {
    // Recebendo e imprimindo campo.
    campo_recebe(buf);
    printf("%s", buf);

    // Recebendo jogada de adversário e armazenando em estrutura Jogada (jogada).
    recuperaJogada(buf, &jogadaAdv);

    for(i = 0; i < jogadaAdv.tam_campo; i++){
      if (jogadaAdv.campo[i] =='o')
        break;
    }

    if(i == jogadaAdv.tam_campo)
      break;

    elaboraJogada(resposta, &jogadaAdv);

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
