# Exercícios de Programação Paralela

Este repositório reúne exercícios em C++ que exemplificam conceitos fundamentais de programação paralela utilizando OpenMP. Cada arquivo aborda um tema específico e com código comentado.

---

## Sumário

- [Apresentação](#apresentação)
- [Requisitos](#requisitos)
- [Como compilar e executar](#como-compilar-e-executar)
- [Descrição dos Exercícios](#descrição-dos-exercícios)

---

## Apresentação

Este projeto serve como material  para aprender e exercitar conceitos de paralelismo em C++ com OpenMP, incluindo:

- Criação de regiões paralelas
- Paralelização de loops
- Redução (reduction)
- Estratégias de escalonamento (scheduling)

---

## Requisitos

- Compilador C++ com suporte a OpenMP (ex: `g++`, `clang++`, ou Visual Studio)
- Sistema operacional Linux, macOS ou Windows (com MinGW/MSYS ou Visual Studio)

---

## Como compilar e executar

**Compilação (exemplo com g++):**
```sh
g++ -std=c++11 -fopenmp Exercicio1.cpp -o Exercicio1
```

**Execução:**
```sh
./Exercicio1      # Linux/macOS/Terminal do Windows
```

## Descrição dos Exercícios

### Exercicio1.cpp
**Objetivo:**  
Demonstração básica de região paralela com OpenMP ("Hello World" paralelo).

**Descrição:**  
Cria uma região `#pragma omp parallel` onde cada thread imprime seu identificador (`omp_get_thread_num()`) e o total de threads (`omp_get_num_threads()`). Utiliza sincronização para evitar embaralhamento da saída.

---

### Exercicio2.cpp
**Objetivo:**  
Paralelização de um loop de soma usando `reduction`.

**Descrição:**  
Soma os elementos de um vetor sequencialmente e em paralelo, utilizando `#pragma omp parallel for reduction(+:soma)`. Compara os resultados e demonstra a importância do uso de `reduction` para evitar condições de corrida.

---

### Exercicios3e4.cpp
**Objetivo:**  
Explorar diferentes políticas de escalonamento (`schedule(static)`, `schedule(dynamic, chunk)`) e medir o impacto no desempenho.

**Descrição:**  
Implementa e compara estratégias de distribuição de trabalho entre threads, analisando vantagens e desvantagens de cada abordagem em termos de desempenho e balanceamento de carga.

---

### Exercicio5.cpp
**Objetivo:**  
Aprofundar conceitos de sincronização e paralelização.

**Descrição:**  
Aborda tópicos como seções críticas, uso de `atomic`; paralelização de algoritmos mais complexos, reforçando conceitos apresentados nos exercícios anteriores.

---

Autor: Nikolas Silva