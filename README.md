
# AMon - Application Monitor

## Resumo

AMon é um programa que monitora, coleta e armazena a quantidade de recursos utilizados por processos específicos no sistema operacional. Através do PID de cada processo, AMon consegue obter as informaões de consumo de CPU, memória e disco durante a execução. Todas as informações são coletadas em tempo real e entre um intervalo de tempo que pode ser definido pelo usuário.

Inicialmente, foi desenvolvido para coletar a utilização de recursos dos benchmarks paralelos do NAS e PARSEC.


## Dependências

- [UPL](https://github.com/dalvangriebler/upl). *Utility Performance Library*.
- [`iostat`](https://linux.die.net/man/1/iostat). 

## Utilizando o AMon

AMon monitora os processos a partir de um arquivo que deve ser informado pela linha de comando. Dentro deste arquivo, deve haver a chamada do processo que deve ser monitorado e a sintaxe para retornar o PID do processo. 

Para demonstração, o processo de compressão paralelo com *pbzip2* será monitorado pelo AMon. Para isso, primeiramente é necessário criar um shell script com a sintaxe do pbzip2 e o comando `echo $!` que retorna o PID do processo do pbzip2. O número de *threads* que *pbzip2* será executado é informado pelo AMon. Portanto, é muito importante que programa a ser monitorado utilize argumento para receber o número de *threads*. Veja o exemplo no *script*:

```bash
#!/bin/bash
#File: pbzip.sh
pbzip2 -p$1 -r -5 myfile.tar files*.txt
echo $!
```
Na linha de comando do terminal a sintaxe fica da seguinte forma:

```bash
user@machine:~$ apmon 4 2 pbzip.sh
```

> apmon: binário de execução

> 4: Número de threads

> 2: intervalo de coleta

> pbzip.sh: arquivo script comando para ser monitorado.

## Resultado do AMon

As métricas coletadas são armazenadas em arquivos dentro do diretório onde AMon foi executado.
Os arquivos são:

- core_load.txt: contêm os valores da utilização (em porcentagem) dos *cores* do processador dentro do intervalo de tempo informado pelo usuário.

- MemPPid.txt: contêm os valores do uso de memória de cada processo dentro do intervalo de tempo informado pelo usuário.

- IO.txt: contêm os valores do uso de disco do sistema.

- GeneralMetrics.txt: contêm métricas gerais, tais como: utilização da memória do sistema antes da execução, *cache missing* e tempo de execução (estimado).
