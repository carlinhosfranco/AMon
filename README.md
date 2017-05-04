
# AMon - Application Monitor

RESUMO: 

AMon é um programa desenvolvido em C++ utilizando a biblioteca UPL que monitora a utilização de recursos de processos específicos no linux.

Inicialmente, foi desenvolvida para coletar métricas de processos executando os benchmarks NAS e PARSEC.
Autor: Carlos Alberto Franco Maron
Contato: francocaam at gmail dot com

UTILIZANDO:

AMon monitora os processos a partir de um arquivo que é informado pela linha de comando. Dentro desse arquivo informado, deve haver a chamada do processo que você gostaria de monitorar e a sintaxe para retornar o PID do processo. Por exemplo, você quer monitorar um processo de compactação utilizando o pbzip2. Para isso, você deve criar um script bash com a chamada do pbzip2 e o número de threads e o comando echo $! que retorna o PID do pbzip2. Vale lembrar que  o número de threads definido pelo usuário é informado pelo AMon ao script bash criado. Portanto, o script bash deve receber o número de threads como argumento.

Explicando o cenário anterior, o usário terá os seguintes arquivos: pbzip.sh e apmon (binário).

------------------------------------
#!/bin/bash
#File: pbzip.sh
pbzip2 -p$1 -r -5 myfile.tar files*.txt
echo $!
------------------------------------

Para executar o APmon:
------------------------------------
user@machine:~$ apmon 4 2 pbzip.sh
------------------------------------
apmon: binário de execução
4: Número de threads
2: intervalo de coleta
pbzip.sh: arquivo script comando para ser monitorado.


Para utilizar o AMon, você precisa ter instalado a biblioteca UPL e a ferramenta iostat para as métricas de disco.


RESULTADOS:

As métricas coletadas são armazenadas em arquivos dentro do diretório onde APmon foi executado.
Os arquivos são:

core_load.txt: Informa o uso dos cores (porcentagem) em cada intervalo informado.

MemPPid.txt: Informa o uso de memória baseando-se pelo PID do processo. A frequência de coleta é baseada no intervalo informado.

IO.txt: As métricas do uso de disco do sistema são informadas.

GeneralMetrics.txt: Métricas gerais são informadas, tais como: Memória do sistema antes da execução, cache missing e tempo de execução (estimado).
