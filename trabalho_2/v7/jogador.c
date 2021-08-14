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
#define PROFUNDIDADE 10

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

// Retorna inteiro com quantidade de saltos em direção 'direcao', indicando em 'salto_pos' o destino de cada salto, 
// em 'fil_por_salto' a quantidade de filosofos removidos por salto.
int criaChutes(char *campo, int *saltos_pos, unsigned char *fil_por_salto, int pos_bola, char direcao, int tam_campo){
  int saltos;
  int pos_atual;

  // Define quantidade de saltos igual a 0.  
  saltos = 0;
  // Define posição atual a frente de bola (considerando a direção indicada por 'direcao').
  pos_atual = pos_bola + direcao;

  // Enquanto houver filósofo apontado por 'pos_atual'.
  while((pos_atual > -1) && (pos_atual < tam_campo) && (campo[pos_atual] == 'f')){

    // Redefine bola como espaço vago.
    campo[pos_bola] = '.';
    
    // Zera contador de filosofos.
    fil_por_salto[saltos] = 0;
    
    // Buscando próximo espaço vago ('.') ou GOL ('-') e substituindo filosofos ('f') por espaço vago, contabilizando em 'fil_por_salto[saltos]'.
    while(campo[pos_atual] == 'f'){
      campo[pos_atual] = '.';
      fil_por_salto[saltos] += 1;
      pos_atual += direcao;
    }

    // Armazenando destino de salto encontrado e atualizando contador de saltos.
    saltos_pos[saltos] = pos_atual;
    saltos ++;

    // Redefinindo novo espaço encontrado como bola e armazenando nova posição de bola.
    campo[pos_atual] = 'o';
    pos_bola = pos_atual;

    // Redefinindo posição atual a frente de bola (de acordo com direcao) para nova análise.
    pos_atual += direcao;
  }

  return saltos;
}

// Retorna posição de bola.
int posicaoBola(char *campo, Jogada *jogada){
  int i;

  // Busca em vetor posição de bola.
  for(i=0; i < (jogada->tam_campo + 2); i++){
    // Encontrando posição de bola, retorna posição - meio_campo.
    if (campo[i] == 'o')
      break;
  }

  return i;
}

int minimax(int profundidade, int nivel, char jogadorMax, char *campo, Jogada *jogada, char *respostaFinal, int alpha, int beta){

  // CONDIÇÂO DE PARADA:
  // Caso nível de nó seja alcançado.
  // Caso bola tenha alcançado algum dos gols.
  if(profundidade == nivel || fimJogo(campo, jogada)){
    //Retornar posição/valor de bola.
    return valorBola(campo, jogada);
  }

  // Variáevis de controle Min-Max
  int i, valor, result;
  
  // Variáveis de controle na geração de saltos.
  int pos_bola, aux_pos_bola, saltos, saltos_pos[jogada->tam_campo], pont_escrita, direcao_inicial, pos_atual;
  char campo_aux[jogada->tam_campo + 3], direcao;
  unsigned char fil_por_salto[jogada->tam_campo];
  
  
  // Variáveis de controle na geração de fiósofos.
  int pos_ini, 
      sinal, 
      pos_fis;

  //#######################################
  //  JOGADOR  - MAXIMIZAÇÃO.
  //#######################################
  if(jogadorMax){

    // Variável de controle de poda em maximizador.
    int podaAlpha;

    // valor iniciado como menos infinito.
    valor =  INT_MIN;
    // zerando flag de poda.
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
    //      GERANDO SALTOS DE BOLA
    // ===============================
    // Armazenando posição de bola.
    pos_bola = posicaoBola(campo, jogada);

    // Definindo direção inicial de saltos (em relação a posição de jogador).
    direcao = direcao_inicial;

    // Loop entre direções (primeiramente saltando para frente e posteriormente saltando para tras).
    do{

      // Copiando campo original para campo auxiliar.
      memcpy(campo_aux, campo, jogada->tam_campo + 3);

      // Efetuando todos os possíveis saltos em direção 'direcao', armazenando a quantidade de saltos em 'saltos', 
      // o destino de cada salto em 'saltos_pos' e a quantidade de filósofos removidos por salto em 'fil_por_salto'.
      saltos = criaChutes(campo_aux, saltos_pos, fil_por_salto, pos_bola, direcao, jogada->tam_campo + 2);

      // Armazenando nova posição de bola em campo modificado.
      aux_pos_bola = posicaoBola(campo_aux, jogada);

      // Enquanto não verificou todos os saltos ou não podando.
      while(saltos > 0 && !podaAlpha){
        //#######################################
        // TRABALHAR COM NOVO CAMPO OBTIDO (CHAMADA RECURSIVA).

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
        
        // Definindo campo de salto anterior.
        // Removendo bola.
        campo_aux[aux_pos_bola] = '.';
        
        // Reposicionando filósofos.
        pos_atual = aux_pos_bola - direcao;

        for (i = 0; i < fil_por_salto[saltos - 1]; i++){
          campo_aux[pos_atual] = 'f';
          pos_atual -= direcao;
        }

        // Redefinindo bola
        aux_pos_bola = pos_atual;
        campo_aux[aux_pos_bola] = 'o';

        saltos --;
      }
      // invertendo direção dos saltos.
      direcao *= -1;
    }
    while(direcao != direcao_inicial && !podaAlpha);

    // ===============================
    //      GERANDO INSERE FILÓSOFO
    // ===============================
    // Percorrendo todas as posições entre os gols.
    for(i=0; (i < (jogada->tam_campo)) && !podaAlpha; i++){

      // Cacula posição de filósofo de acordo com lado de jogador (indo do gol oposto ao gol de jogador).
      pos_fis = pos_ini + (i * sinal);

      // Caso espaço esteja disponível, inserir filósofo.
      if (campo[pos_fis] == '.'){
        // Insere filósofo em posição livre.
        campo[pos_fis] = 'f';

        //#######################################
        // TRABALHAR COM NOVO CAMPO OBTIDO (CHAMADA RECURSIVA).
        
        // Chamando Filho Recursivamente e armazenando valor em 'result'.
        result = minimax(profundidade, nivel + 1 , 0, campo, jogada, respostaFinal, alpha, beta);

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
  }

  //#######################################
  //  JOGADOR ADVERSÁRIO - MINIMIZAÇÃO.
  //#######################################
  else{

    // Variável de controle de poda em maximizador.
    int podaBeta;

    // valor iniciado como mais infinito.
    valor =  INT_MAX;
    // zerando flag de poda.
    podaBeta = 0;

    // Verifica o lado de jogador adversário.
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
    //      GERANDO SALTOS DE BOLA
    // ===============================

    // Armazenando posição de bola.
    pos_bola = posicaoBola(campo, jogada);

    // Definindo direção inicial de saltos (em relação a posição de jogador adversário).
    direcao = direcao_inicial;

    // Loop entre direções (primeiramente saltando para frente e posteriormente saltando para tras).
    do{

      // Copiando campo para campo auxiliar.
      memcpy(campo_aux, campo, jogada->tam_campo + 3);
      
      // Efetuando todos os possíveis saltos em direção 'direcao', armazenando a quantidade de saltos em 'saltos', 
      // o destino de cada salto em 'saltos_pos' e a quantidade de filósofos removidos por salto em 'fil_por_salto'.
      saltos = criaChutes(campo_aux, saltos_pos, fil_por_salto, pos_bola, direcao, jogada->tam_campo + 2);
      
      // Armazenando nova posição de bola em campo modificado.
      aux_pos_bola = posicaoBola(campo_aux, jogada);

      // Enquanto não verificou todos os saltos ou não podando.
      while(saltos > 0 && !podaBeta){
        //#######################################
        // TRABALHAR COM NOVO CAMPO OBTIDO (CHAMADA RECURSIVA).
        
        // Chamando Filho Recursivamente e armazenando valor em 'result'.
        result = minimax(profundidade, nivel + 1 , 1, campo_aux, jogada, respostaFinal, alpha, beta);

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

        // Definindo campo de salto anterior.
        // Removendo bola.
        campo_aux[aux_pos_bola] = '.';
        
        // Redefinindo filósofos.
        pos_atual = aux_pos_bola - direcao;

        for (i = 0; i < fil_por_salto[saltos - 1]; i++){
          campo_aux[pos_atual] = 'f';
          pos_atual -= direcao;
        }

        // Redefinindo bola
        aux_pos_bola = pos_atual;
        campo_aux[aux_pos_bola] = 'o';

        saltos --;
      }
      // invertendo direção dos saltos.
      direcao *= -1;
    }
    while(direcao != direcao_inicial && !podaBeta);

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
  }


  return valor;
}

// Recebe jogada em 'jogadaAdv' e retorna em 'resposta' string para próxima jogada.
void elaboraJogada(char *resposta, Jogada *jogadaAdv){
  char *campo_trabalho;

  // Alocando campo de trabalho com 2 espaços para gol.
  campo_trabalho = (char*) malloc(jogadaAdv->tam_campo + 3);
  
  // Copiando campo para campo_trabalho.
  campo_trabalho[0] = GOL;
  memcpy(&campo_trabalho[1], jogadaAdv->campo, jogadaAdv->tam_campo);
  campo_trabalho[jogadaAdv->tam_campo + 1] = GOL; 
  campo_trabalho[jogadaAdv->tam_campo + 2] = '\0'; 

  // Chamando função minimax, esperanto receber em 'resposta' a póxima jogada.
  minimax(PROFUNDIDADE, 0, 1,  campo_trabalho, jogadaAdv, resposta, INT_MIN, INT_MAX);
  
  free(campo_trabalho);
  return;
}


int main(int argc, char **argv) {
  char buf[MAXSTR];
  char *resposta;
  int i;
  Jogada jogadaAdv;

  resposta = (char *) malloc(MAXSTR);
  
  campo_conecta(argc, argv);

  while(1) {
    // Recebendo e imprimindo campo.
    campo_recebe(buf);
    printf("%s", buf);


    // Recebendo jogada de adversário e armazenando em estrutura Jogada (jogada).
    recuperaJogada(buf, &jogadaAdv);

    // Caso fim de jogo (campo não contendo bola), sair de loop e terminar programa.
    for(i = 0; i < jogadaAdv.tam_campo; i++){
      if (jogadaAdv.campo[i] =='o')
        break;
    }
    if(i == jogadaAdv.tam_campo)
      break;

    // Elaborando jogada a partid de informações recebidas (contidas em jogadaAdv).
    elaboraJogada(resposta, &jogadaAdv);

    // Bufferizando resposta.
    sprintf(buf, "%s\n", resposta);

    // Envia jogada a campo.
    campo_envia(buf);

  }
  
  free(resposta);
}
