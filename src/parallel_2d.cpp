#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <omp.h>

class Matrix {
private:
    std::vector<std::vector<double>> data;
    int n;

public:
    // Construtor
    Matrix(int size) : n(size), data(size, std::vector<double>(size, 0.0)) {}
    
    // Construtor de cópia
    Matrix(const Matrix& other) : n(other.n), data(other.data) {}
    
    // Preencher matriz com valores aleatórios
    void fillRandom() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0.0, 100.0);
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                data[i][j] = dis(gen);
            }
        }
    }
    
    // Preencher matriz com valor específico
    void fillValue(double value) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                data[i][j] = value;
            }
        }
    }
    
    // Acesso aos elementos
    double& operator()(int i, int j) {
        return data[i][j];
    }
    
    const double& operator()(int i, int j) const {
        return data[i][j];
    }
    
    // Getter para o tamanho
    int size() const {
        return n;
    }
};

Matrix parallel_2d(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n);
    
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double aij = A(i, j);
            for (int k = 0; k < n; k++) {
                C(i, k) += aij * B(j, k);
            }
        }
    }
    
    return C;
}

// Função para medir tempo de execução
template<typename Func>
double measureTime(Func func, const Matrix& A, const Matrix& B) {
    auto start = std::chrono::high_resolution_clock::now();
    func(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count();
}

int main() {
    std::vector<int> sizes = {512, 1024, 2048, 4096, 8192};
    
    std::cout << "================= VERSÃO PARALELA 2D =================" << std::endl;
    std::cout << "Número de threads disponíveis: 8" << std::endl;
    std::cout << "======================================================" << std::endl;
    
    // Configurando número de threads
    omp_set_num_threads(8);
    
    for (int n : sizes) {
        std::cout << "\n--- Matriz " << n << "x" << n << " ---" << std::endl;
        
        // Cria matrizes A e B
        Matrix A(n);
        Matrix B(n);
        
        // Preenche matrizes com valores aleatórios
        A.fillRandom();
        B.fillRandom();
        
        // Mede tempo da implementação
        double time_par_2d = measureTime(parallel_2d, A, B);
        
        // Exibe resultados
        std::cout << std::fixed << std::setprecision(4);
        std::cout << "Tempo de Execução: " << time_par_2d << " s" << std::endl;
        
    }
    
    return 0;
}
