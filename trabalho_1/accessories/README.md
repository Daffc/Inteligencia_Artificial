- exemplo_mapa_30_30_10.txt: exemplo de instância do problema;

- exemplo_solucao_30_30_10.txt: exemplo de uma solução possível
  para a instância exemplo;

- ***geramapa***: programa para gerar instâncias aleatórias do problema;

- ***verifica***: programa para verificar se um determinado mapa é 
  resolvido por uma determinada sequencia de cores, retornando 0 
  se sim e 1 caso contrário, exemplo de uso:
    $ cat exemplo_mapa_30_30_10.txt exemplo_solucao_30_30_10.txt | ./verifica
  
  para pegar o resultado no bash:
    ````
    if [ "$?" -eq 0 ]
        then
            echo ok
        else
            echo fail
    fi
    ````

- **anima*** : programa para gerar uma animação da solução de um mapa no terminal 
  exemplo de uso: 
    $ cat exemplo_mapa_30_30_10.txt exemplo_solucao_30_30_10.txt | ./anima

- ***floodit_h1*** resolvedor exemplo que usa um algoritmo guloso que escolhe a 
  cor que mais aparece na borda da região do canto superior esquerdo;

- ***floodit_h2***: resolvedor exemplo que usa um algoritmo guloso que escolhe a 
  cor que mais aparece nas regiões adjacentes à região do canto superior 
  esquerdo.