# Trabalho 2

Implementar um programa  em C que jogue o jogo  "Futebol dos Filósofos Unidimensional".


## Especificação:
O  jogo "Futebol  dos Filósofos  Unidimensional" é  um jogo  onde dois jogadores competem  entre si em  um campo finito  unidimensional. Cada extremo do campo é ocupado por um jogador. O jogo termina quando algum jogador marca gol, fazendo  com que a bola saia do  campo pelo lado do
seu adversário.

Os dois jogadores, o da esquerda e o da direita, se revesam em turnos. Em cada  turno o jogador  da vez pode  fazer apenas uma  das seguintes ações:

  - Colocar um novo filósofo em alguma posição não ocupada do campo;

  - Mover a bola.

O movimento da bola acontece em  saltos sobre sequências de um ou mais filósofos  adjacentes.  Para saltar  a  bola  sobre uma  sequência  de
filósofos a  bola precisa estar  em uma  posição do campo  adjacente à sequência.

Considere por  exemplo a  seguinte configuração de  um campo  em algum momento de uma partida:

````
..fff.off.f.fff..f.
````

Este campo  tem 19 posições, as  posições com "." indicam  as posições não ocupadas  do campo, as posições  com "f" indicam os  filósofos que estão em campo e  o "o" é a bola. Neste cenário o  jogador da vez pode mover  a bola,  saltando  sobre os  dois filósofos  que  estão logo  a direita da bola.  O destino do  salto não pode ser uma posição ocupada do campo.  Os  filósofos que são saltados pela  bola são imediatamente retirados do campo. A configuração do campo resultante é dada por:

````
..fff....of.fff..f.
````

O  movimento  da  bola  pode  ser   composto  por  mais  de  um  salto consecutivo,  o  jogador  da  vez  define de  quantos  saltos  será  o movimento da bola. Considerando o campo:

````
..fff.off.f.fff..f.
````

Os seguintes movimentos da bola são possíveis:

````
..fff....of.fff..f.     (um salto sobre dois filósofos)

..fff......offf..f.     (dois saltos, 2+1 filósofos)

..fff..........o.f.     (três saltos, 2+1+3 filósofos)
````

O movimento da bola pode resultar  em gol.  Considere que o jogador da vez é o da direita e que a configuração do campo é dada por:

````
ffffffoff.f.fff..f.
````

Nesta configuração, o jogador da direita pode marcar um gol saltando a bola sobre  a sequência  de 6  filósofos que está  logo à  esquerda da bola, fazendo com que a bola saia do campo  pelo extremo esquerdo. Ao marcar o gol o jogador da direita vence e a partida é encerrada.

O seu programa deve implementar  um jogador automático que se comunica com o controlador do campo. O funcionamento deve ser um processo repetitivo composto dos seguintes passos:

  1) receber a configuração atual do campo e a última ação feito pelo 
     adversário;

  2) escolher, em no máximo 5 segundos, a ação que será feita;

  3) enviar a ação escolhida;

  4) voltar para o passo 1.

O seu  programa se comunicará  com um controlador  do campo que  faz o controle da partida, revezando os dois jogadores e controlando o tempo de resposta de cada um.

A configuração do campo é dada por  uma linha em formato texto, com os seguintes elementos separados por espaços:

  - um caractere  "e" ou  "d", indicando  de que lado  do campo  o seu
    jogador está;

  - um número inteiro impar K, indicando o tamanho do campo;

  - uma  sequência de  K  caracteres (string)  com  a configuração  do campo, onde "." indica uma posição livre, "f" indica um filósofo e "o" indica a bola.

Os índices das posições no campo vão de 1 até K.

As ações são  dadas por uma linha  em formato texto que  inicia com um caractere "e" ou  "d", indicando de qual jogador é  a ação, seguido de um espaço e da descrição da ação:

  - a ação  de inserir um  novo filósofo no campo  é dada por  um "f", seguido de um espaço  e um número inteiro entre 1  e K indicando a
    posição do campo onde o filósofo será inserido;

  - a ação de mover a bola é dada por um "o", seguido de um espaço, um número  inteiro indicando  o  número de  saltos  do movimento,  um espaço e uma sequência de números inteiros entre 0 e K+1 separados por espaços, indicando as posições de destino de cada salto;

  - uma ação especial nula, dada  pelo caractere "n", pode  ocorrer em casos especiais como: início de jogo, fim de jogo, ações inválidas ou violação do tempo de resposta de 5 segundos.

Um salto da bola que resulta em gol  deve ser o último e deve ter como destino uma  posição fora do  campo. Este  destino é uma  das posições especiais: 0 para o lado esquerdo e K+1 para o lado direito.

A partida  inicia com o  campo contendo apenas  a bola na  sua posição central.

O tamanho do campo não muda durante a partida.

A partida termina quando a configuração do campo não contiver a bola e o jogador vitorioso é aquele que marcou o gol.

Por exemplo, considere que o seu  jogador, logo após iniciar, recebe a seguinte entrada:

````
e 7 ...o...
d n
````


A primeira linha indica que a posição do seu jogador é o lado esquerdo do campo,  que o campo tem  7 posições e  que apenas a posição  4 está ocupada  com a  bola. A  segunda  linha indica  que a  última ação  do jogador da direita é a ação especial nula, que indica neste caso que o seu jogador é o primeiro a jogar.

Suponha que o  seu jogador decide inserir um filósofo  na posição 7 do campo. Para isso ele deve gerar a seguinte saída:

````
e f 7
````

Esta ação é  recebida pelo controlador que atualiza o  campo e passa a vez para o jogador da direita. Após  a jogada da direita o seu jogador recebe a entrada:

````
e 7 ...of.f
d f 5
````


A nova configuração do campo tem  o filósofo que o seu jogador inseriu na posição  7 na jogada  anterior e um novo  filósofo na posição  5. A segunda  linha indica  que a  última ação  do jogador  da direita  foi inserir o filósofo da posição 5.

O seu jogador  agora decide marcar um gol saltando  a bola duas vezes, uma para a posição 6 e outra para fora do campo:

````
e o 2 6 8
````

Após o  controlador verificar  que um  gol foi  marcado o  seu jogador recebe:

````
e 7 .......
d n
````


O que  indica que  a partida  terminou pois  a bola  não está  mais em campo.  A  ação "n"  neste caso  indica que o  jogador da  direita não jogou nesta última rodada.

Caso o seu  jogador não gere como  saída uma nova ação no  limite de 5 segundos, o controlador passará a vez para o adversário, assumindo que a ação do seu jogador foi "n".


## Equipe:

O trabalho  pode ser feito  em grupos de até  2 alunos de  graduação e individual para os alunos de pós.


## Entrega:

O trabalho deve ser entregue por email para:
- fabiano@inf.ufpr.br
- com o assunto/subject: trabalho2
- até 23:59 do dia 15/08/2021
- o corpo do email deve conter o nome completo e GRR dos alunos
- o anexo do email deve conter um arquivo .tar.gz com:
    - código fonte
    - makefile
    - leiame.txt
    - este último deve conter a descrição detalhada da estratégia
      implementada e outras informações que considerar relevantes
      para a avaliação do trabalho


## Material de apoio:

***phutball_1d.tgz***: pacote com exemplos e ferramentas de apoio para o desenvolvimento do trabalho,  leia o arquivo leiame.txt do pacote.