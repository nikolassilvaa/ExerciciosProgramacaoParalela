/*
  Exercício 1: Hello World Paralelo
  a) Criar uma região paralela com #pragma omp parallel
  b) Cada thread imprime seu número e o total de threads
  c) Configurado para 4 threads
*/

#include <iostream>
#include <sstream>
#include <omp.h>

int main() {

    // Configurar 4 threads
    omp_set_num_threads(4);

    std::cout << "=== Exercicio 1: Hello World Paralelo ===\n";

    #pragma omp parallel
    {
        int tid = omp_get_thread_num(); // ID da thread (0, 1, 2, 3)
        int nthreads = omp_get_num_threads(); // Total de threads (4)

        std::ostringstream oss;
        oss << "Thread " << tid << " de " << nthreads << " dizendo: Ola, Mundo!\n";
        
        #pragma omp critical
        {
            std::cout << oss.str();
        }
    }

    return 0;
}
