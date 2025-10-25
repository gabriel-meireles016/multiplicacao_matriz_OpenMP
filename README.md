# Multiplicação de Matrizes utilizando OpenMP

Programa paralelo usando OpenMP para resolver a multiplicação de duas matrizes quadradas de ordem n da forma C=AxB usando o particionamento 2D.

## Funcionalidades
- Realiza a multiplicação de forma Sequencial.
- Realiza a multiplicação de forma Paralela com Particionamento 1D.
- Realiza a multiplicação de forma Paralela com Particionamento 2D.
- Realiza a multiplicação de forma Paralela com Particionamento 2D com Tiling.
- Mede o tempo de execução de cada um dos algoritmos.

## Requisitos

- **Ubuntu/WSL2** (ou Linux equivalente)
- **g++ (C++17)**

## Compilação

    make

# Casos de Teste

Para iniciar o processo, execute a geração de exemplos de entrada:
```
./obj/generate_input
```

Após a geração dos exemplos de entrada, cada código deve ser executado individualmente na linha de comando:
```
./obj/sequencial
./obj/parallel_1d
./obj/parallel_2d
./obj/parallel_2d_tiling
```
