/*
  Arquivo: paralelo_ex5.cpp
  Objetivo: Exercício 5 - Escalonamento (Scheduling)
  
  Este exercício compara diferentes estratégias de escalonamento:
  a) Execute com schedule(static) e schedule(dynamic, 1000)
  b) Compare os tempos em diferentes quantidades de threads (2, 4, 8)
  c) Explique em quais situações static e dynamic são mais adequados
  
  Compilação: g++ -O2 -fopenmp -std=c++17 paralelo_ex5.cpp -o paralelo_ex5
  Execução: ./paralelo_ex5
*/

#include <iostream>     // std::cout, std::endl
#include <vector>       // std::vector
#include <iomanip>      // std::fixed, std::setprecision
#include <string>       // std::string
#include <omp.h>        // API OpenMP
#include <algorithm> 

// Função para executar o cálculo vetorial com escalonamento específico
double executar_calculo(const std::vector<double>& x, const std::vector<double>& y, 
                       const std::vector<double>& z, std::vector<double>& a, 
                       const std::string& tipo_schedule, int num_threads) {
    
    const int N = x.size();
    omp_set_num_threads(num_threads);
    
    double tempo_inicio = omp_get_wtime();
    
    if (tipo_schedule == "static") {
        /*
          schedule(static): 
          - Divide as iterações em blocos contíguos de tamanho aproximadamente igual
          - Cada thread recebe um bloco no início da execução
          - Baixa sobrecarga de escalonamento
          - Ideal para cargas de trabalho uniformes
          
          Distribuição para N=1.000.000 com 4 threads:
          - Thread 0: iterações 0 a 249.999
          - Thread 1: iterações 250.000 a 499.999  
          - Thread 2: iterações 500.000 a 749.999
          - Thread 3: iterações 750.000 a 999.999
        */
#pragma omp parallel for schedule(static)
        for (int i = 0; i < N; ++i) {
            a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
        }
    } 
    else if (tipo_schedule == "dynamic") {
        /*
          schedule(dynamic, 1000):
          - Distribui blocos de tamanho 1000 iterações dinamicamente
          - Threads pegam novos blocos conforme terminam os anteriores
          - Maior sobrecarga de escalonamento due ao gerenciamento dinâmico
          - Ideal para cargas de trabalho irregulares ou desbalanceadas
          
          Como funciona:
          - Thread pega bloco de 1000 iterações (ex: 0-999)
          - Ao terminar, pega próximo bloco disponível (ex: 1000-1999)
          - Continua até não haver mais trabalho
          - Balanceamento automático entre threads
        */
#pragma omp parallel for schedule(dynamic, 1000)
        for (int i = 0; i < N; ++i) {
            a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
        }
    }
    
    double tempo_fim = omp_get_wtime();
    return tempo_fim - tempo_inicio;
}

int main() {
    std::cout << "=== Exercicio 5: Escalonamento (Scheduling) ===\n";
    std::cout << "Comparando schedule(static) vs schedule(dynamic, 1000)\n";
    std::cout << "com diferentes quantidades de threads (2, 4, 8)\n\n";

    // =========================================================
    // Configuração do problema
    // =========================================================
    const int N = 1'000'000;  // Tamanho dos vetores
    
    // Vetores de entrada e saída
    std::vector<double> x(N), y(N), z(N);
    std::vector<double> a_static(N), a_dynamic(N);
    
    // Inicialização dos vetores
    std::cout << "Inicializando vetores de entrada (N = " << N << ")...\n";
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; ++i) {
        x[i] = static_cast<double>(i) * 1e-3;
        y[i] = static_cast<double>(i % 100) * 1e-3;
        z[i] = 0.5 + static_cast<double>(i % 7) * 1e-2;
    }

    // Array com diferentes quantidades de threads para testar
    std::vector<int> threads_para_testar = {2, 4, 8};
    
    // Estrutura para armazenar resultados
    struct ResultadoTeste {
        int num_threads;
        double tempo_static;
        double tempo_dynamic;
        double speedup_static;
        double speedup_dynamic;
        double razao_static_dynamic;
    };
    
    std::vector<ResultadoTeste> resultados;
    
    // Tempo sequencial de referência (1 thread, static)
    std::cout << "\nCalculando tempo sequencial de referencia...\n";
    double tempo_sequencial = executar_calculo(x, y, z, a_static, "static", 1);
    std::cout << "Tempo sequencial: " << std::fixed << std::setprecision(6) 
              << tempo_sequencial << " segundos\n\n";

    // =========================================================
    // b) Testes com diferentes quantidades de threads
    // =========================================================
    std::cout << "=== TESTES DE PERFORMANCE ===\n";
    std::cout << "Threads | Static (s) | Dynamic (s) | Speedup Static | Speedup Dynamic | Razao S/D\n";
    std::cout << "--------|------------|-------------|----------------|-----------------|----------\n";
    
    for (int num_threads : threads_para_testar) {
        std::cout << "Testando com " << num_threads << " threads...\n";
        
        // a) Teste com schedule(static)
        double tempo_static = executar_calculo(x, y, z, a_static, "static", num_threads);
        
        // a) Teste com schedule(dynamic, 1000)
        double tempo_dynamic = executar_calculo(x, y, z, a_dynamic, "dynamic", num_threads);
        
        // Cálculo de speedup (aceleração): tempo_sequencial / tempo_paralelo
        double speedup_static = tempo_sequencial / tempo_static;
        double speedup_dynamic = tempo_sequencial / tempo_dynamic;
        
        // Razão entre os tempos (static/dynamic)
        double razao = tempo_static / tempo_dynamic;
        
        // Armazenar resultados
        resultados.push_back({num_threads, tempo_static, tempo_dynamic, 
                             speedup_static, speedup_dynamic, razao});
        
        // Exibir resultados formatados
        std::cout << std::setw(7) << num_threads << " | "
                  << std::setw(10) << tempo_static << " | "
                  << std::setw(11) << tempo_dynamic << " | "
                  << std::setw(14) << speedup_static << " | "
                  << std::setw(15) << speedup_dynamic << " | "
                  << std::setw(8) << razao << "\n";
    }

    // =========================================================
    // Verificação de corretude
    // =========================================================
    std::cout << "\n=== VERIFICACAO DE CORRETUDE ===\n";
    double max_erro = 0.0;
    for (int i = 0; i < N; ++i) {
        double erro = std::abs(a_static[i] - a_dynamic[i]);
        if (erro > max_erro) max_erro = erro;
    }
    std::cout << "Erro maximo entre static e dynamic: " << max_erro << "\n";
    if (max_erro < 1e-10) {
        std::cout << "PASSOU: Resultados identicos entre ambos os escalonamentos\n";
    } else {
        std::cout << "FALHOU: Resultados diferentes entre escalonamentos\n";
    }

    // =========================================================
    // c) Análise e explicação detalhada
    // =========================================================
    std::cout << "\n=== ANALISE DOS RESULTADOS ===\n";
    
    // Encontrar melhor performance
    auto melhor_static = *std::max_element(resultados.begin(), resultados.end(),
        [](const ResultadoTeste& a, const ResultadoTeste& b) {
            return a.speedup_static < b.speedup_static;
        });
    
    auto melhor_dynamic = *std::max_element(resultados.begin(), resultados.end(),
        [](const ResultadoTeste& a, const ResultadoTeste& b) {
            return a.speedup_dynamic < b.speedup_dynamic;
        });
    
    std::cout << "Melhor speedup static:  " << melhor_static.speedup_static 
              << "x com " << melhor_static.num_threads << " threads\n";
    std::cout << "Melhor speedup dynamic: " << melhor_dynamic.speedup_dynamic 
              << "x com " << melhor_dynamic.num_threads << " threads\n\n";

/*
 =======================================================
c) Explicação detalhada: quando usar static vs dynamic
=========================================================

EXPLICACAO: QUANDO USAR STATIC VS DYNAMIC
    
1) SCHEDULE(STATIC) - Escalonamento Estatico:

   VANTAGENS:
   - Baixa sobrecarga: threads recebem trabalho uma unica vez
   - Previsivel: mesma distribuicao a cada execucao
   - Cache-friendly: acesso sequencial aos dados
   - Otimo para cargas uniformes onde cada iteracao tem custo similar

   DESVANTAGENS:
   - Desbalanceamento se cargas forem irregulares
   - Thread que termina cedo fica ociosa esperando outras
   - Nao se adapta a variacoes de performance do sistema

   USAR QUANDO:
   - Cada iteracao tem custo computacional similar
   - Operacoes vetoriais simples (como neste exemplo)
   - Processamento de imagens com operacoes uniformes
   - Calculos matematicos em arrays regulares

2) SCHEDULE(DYNAMIC, chunk_size) - Escalonamento Dinamico:
   VANTAGENS:
   - Balanceamento automatico: threads pegam trabalho conforme disponibilidade
   - Adapta-se a cargas irregulares
   - Resiliente a variacoes de performance entre threads
   - Nenhuma thread fica ociosa enquanto houver trabalho

   DESVANTAGENS:
   - Maior sobrecarga: threads precisam sincronizar para pegar trabalho
   - Menos cache-friendly: acesso nao-sequencial aos dados
   - Performance pode variar entre execucoes

   USAR QUANDO:
   - Custo computacional varia significativamente por iteracao
   - Processamento de listas com elementos de tamanhos diferentes
   - Algoritmos adaptativos (ex: refinamento de malha)
   - Simulacoes com regioes de densidade variavel
   - Processamento de grafos irregulares

3) CHUNK SIZE (tamanho do bloco):
   - Chunk pequeno (ex: 1): maximo balanceamento, maxima sobrecarga
   - Chunk grande (ex: N/num_threads): minima sobrecarga, possivel desbalanceamento
   - Chunk medio (ex: 1000): compromisso entre balanceamento e sobrecarga
   - Regra pratica: chunk = max(1, N / (num_threads * 10))

4) RESULTADOS ESPERADOS NESTE EXEMPLO:
   - Para esta expressao vetorial uniforme, static deve ser ligeiramente mais rapido
   - A diferenca sera pequena pois a carga e uniforme
   - Dynamic pode ter pequena sobrecarga adicional
   - Com mais threads, a diferenca pode se tornar mais visivel

 */
    // Análise específica dos resultados obtidos
    std::cout << "5) ANALISE DOS RESULTADOS OBTIDOS:\n";
    for (const auto& resultado : resultados) {
        std::cout << "   Com " << resultado.num_threads << " threads:\n";
        if (resultado.razao_static_dynamic < 1.0) {
            std::cout << "     - Static foi " << (1.0/resultado.razao_static_dynamic - 1.0)*100.0 
                     << "% mais rapido que dynamic\n";
        } else {
            std::cout << "     - Dynamic foi " << (resultado.razao_static_dynamic - 1.0)*100.0 
                     << "% mais rapido que static\n";
        }
    }

    return 0;
}
