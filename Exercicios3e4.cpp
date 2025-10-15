/*
  Exercícios 3 e 4: Expressão Vetorial com Medição Detalhada de Tempo
  
  Exercício 3:
  - a[i] = x[i]^2 + y[i]^2 + z[i]^2 para N=1.000.000
  - Comparar versão sequencial vs paralela
  - Medir tempo total
  
  Exercício 4:
  - Medir tempo por thread individual
  - Mostrar quantas threads foram utilizadas
  - Análise de balanceamento
*/

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <omp.h>

int main() {
    std::cout << "=== Exercicios 3 e 4: Expressao Vetorial com Medicao Detalhada ===\n";
    
    const int N = 1'000'000;
    const int NUM_THREADS = 4;
    omp_set_num_threads(NUM_THREADS);
    
    // Vetores
    std::vector<double> x(N), y(N), z(N), a_seq(N), a_par(N);
    
    // Inicialização
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; ++i) {
        x[i] = static_cast<double>(i) * 1e-3;
        y[i] = static_cast<double>(i % 100) * 1e-3;  
        z[i] = 0.5 + static_cast<double>(i % 7) * 1e-2;
    }
    
    // EXERCÍCIO 3: Versão sequencial

    std::cout << "\n=== Exercicio 3: Comparacao Sequencial vs Paralela ===\n";
    double t0 = omp_get_wtime();
    for (int i = 0; i < N; ++i) {
        a_seq[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
    }
    double t1 = omp_get_wtime();
    double tempo_seq = t1 - t0;
    
    // Versão paralela básica
    double t2 = omp_get_wtime();
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; ++i) {
        a_par[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
    }
    double t3 = omp_get_wtime();
    double tempo_par = t3 - t2;
    
    std::cout << "Tempo sequencial: " << std::fixed << std::setprecision(6) 
              << tempo_seq << " s\n";
    std::cout << "Tempo paralelo: " << tempo_par << " s\n";
    std::cout << "Speedup: " << tempo_seq/tempo_par << "x\n";
    
    // EXERCÍCIO 4: Medição detalhada por thread
    
    std::cout << "\n=== Exercicio 4: Medicao por Thread ===\n";
    
    const int MAX_THREADS = omp_get_max_threads();
    std::vector<double> tempo_inicio(MAX_THREADS, 0.0);
    std::vector<double> tempo_fim(MAX_THREADS, 0.0);
    std::vector<int> iteracoes_processadas(MAX_THREADS, 0);
    int num_threads_usadas = 0;
    
    double tempo_total_inicio = omp_get_wtime();
    
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        
#pragma omp single nowait
        {
            num_threads_usadas = nthreads;
        }
        
        tempo_inicio[tid] = omp_get_wtime();
        int contador_local = 0;
        
#pragma omp for schedule(static) nowait
        for (int i = 0; i < N; ++i) {
            a_par[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
            contador_local++;
        }
        
        tempo_fim[tid] = omp_get_wtime();
        iteracoes_processadas[tid] = contador_local;
    }
    
    double tempo_total_fim = omp_get_wtime();
    double tempo_total = tempo_total_fim - tempo_total_inicio;
    
    // Resultados detalhados
    std::cout << "Tempo total: " << tempo_total << " s\n";
    std::cout << "Threads utilizadas: " << num_threads_usadas << "\n\n";
    
    std::cout << "Thread | Tempo (s) | Iteracoes\n";
    std::cout << "-------|-----------|----------\n";
    
    std::vector<double> tempos_individuais(num_threads_usadas);
    for (int tid = 0; tid < num_threads_usadas; ++tid) {
        double tempo_thread = tempo_fim[tid] - tempo_inicio[tid];
        tempos_individuais[tid] = tempo_thread;
        
        std::cout << std::setw(6) << tid << " | "
                  << std::setw(9) << tempo_thread << " | "
                  << std::setw(9) << iteracoes_processadas[tid] << "\n";
    }
    
    // Análise de balanceamento
    auto min_tempo = *std::min_element(tempos_individuais.begin(), tempos_individuais.end());
    auto max_tempo = *std::max_element(tempos_individuais.begin(), tempos_individuais.end());
    double desvio_percentual = (max_tempo - min_tempo) / ((min_tempo + max_tempo)/2) * 100.0;
    
    std::cout << "\nAnalise de balanceamento:\n";
    std::cout << "Tempo min: " << min_tempo << " s\n";
    std::cout << "Tempo max: " << max_tempo << " s\n";
    std::cout << "Desvio: " << desvio_percentual << "%\n";
    
    if (desvio_percentual < 5.0) {
        std::cout << "Balanceamento: EXCELENTE\n";
    } else if (desvio_percentual < 15.0) {
        std::cout << "Balanceamento: BOM\n";
    } else {
        std::cout << "Balanceamento: RUIM - considere schedule dinamico\n";
    }
    
    return 0;
}