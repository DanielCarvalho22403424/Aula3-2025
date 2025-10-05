# Trabalho 1 SO DANIEL CARVALHO a22403424



## FIFO
### Executa os processos pela ordem de chegada, cada um até terminar. Simples, mas tarefas longas podem atrasar as curtas.

## SJF
### Escolhe sempre o processo que vai terminar mais rápido. Reduz tempo de espera médio, mas processos longos podem sofrer starvation.

## RR
### Cada processo recebe um time slice fixo. Quando o tempo acaba, vai para o fim da fila e o próximo ocupa o CPU. Equitativo e bom para interatividade.

## MLFQ
### Usa várias filas de prioridade. Processos que usam muito CPU descem de nível; processos que esperam sobem. Adapta-se dinamicamente entre eficiência e justiça.



```

1 run_apps.sh
2 run_apps2.sh
3 run_appsio.sh
4 run_appsio2.sh

 
Aplicação Métrica (segs)    FIFO    SJF     RR      MLFQ
1       Tempo Execução      29,06   26,67   38,33   38,00  
1       Tempo Resposta      13,26   11,67   11,67   11,67
2       Tempo Execução      20,50   15,50   23,75   23,91   
2       Tempo Resposta      14,00   9,00    9,00    9,00
3       Tempo Execução      N.D.    N.D.    N.D.    7,60
3       Tempo Resposta      N.D.    N.D.    N.D.    3,27
4       Tempo Execução      N.D.    N.D.    N.D.    16,81
4       Tempo Resposta      N.D.    N.D.    N.D.    12,14

```


#### Nos tempos médios de execução e de resposta de quatro aplicações, SJF se destaca em processos CPU-bound, apresentando os menores tempos de execução, enquanto RR e MLFQ têm valores maiores devido à preempção, mas garantem respostas mais rápidas. FIFO, apesar da simplicidade, apresenta tempos de resposta mais elevados quando processos longos chegam primeiro. Em aplicações I/O-bound, o MLFQ reduz tanto o tempo de execução quanto o de resposta ao alternar eficientemente entre processos prontos e bloqueados, equilibrando desempenho e interatividade.