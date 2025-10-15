/*
  Exercício 2: Paralelizando um for simples

  a) Vetor v de tamanho 100 inicializado com 1
  b) Loop sequencial para somar
  c) Loop paralelo com reduction
  d) Comparar e explicar reduction
  
*/

#include <iostream>
#include <vector>
#include <cassert>
#include <omp.h>

int main() {
    std::cout << "=== Exercicio 2: Paralelizando um for simples ===\n";
    
    // a) Criar vetor v de tamanho 100 com todos elementos = 1
    const int N = 100;
    std::vector<int> v(N, 1);
    
    // b) Loop sequencial
    long long soma_seq = 0;
    for (int i = 0; i < N; ++i) {
        soma_seq += v[i];
    }
    
    // c) Loop paralelo com reduction
    omp_set_num_threads(4);
    long long soma_par = 0;
    
#pragma omp parallel for reduction(+:soma_par)
    for (int i = 0; i < N; ++i) {
        soma_par += v[i];
    }
    
    // d) Comparando os resultados e Verificando se são iguais
    std::cout << "Soma sequencial: " << soma_seq << "\n";
    std::cout << "Soma paralela: " << soma_par << "\n";

    assert(soma_seq == soma_par && "Somas devem ser iguais");
    std::cout << "\nVerificacao: PASSOU - resultados identicos\n";
    
    // Explicando Porque a diretiva reduction é necessaria
    std::cout << "\nPor que reduction(+:soma_par) e necessario?\n";
    std::cout << "- Sem reduction, multiplas threads atualizariam 'soma_par' simultaneamente\n";
    std::cout << "- Isso criaria uma condicao de corrida (race condition)\n";
    std::cout << "- reduction cria uma copia privada para cada thread\n";
    std::cout << "- Ao final, combina todas as copias com a operacao '+'\n";
    std::cout << "- E Garante um resultado correto\n";
    std::cout << "\n";

    return 0;
}