#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

class Matrix {
private:
    std::vector<std::vector<double>> data;
    int n;

public:
    // Construtor
    Matrix(int size) : n(size), data(size, std::vector<double>(size, 0.0)) {}
    
    // Construtor de cópia
    Matrix(const Matrix& other) : n(other.n), data(other.data) {}
    
    // Preenche matriz com valores aleatórios
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

// Multiplicação Sequencial
Matrix sequential(const Matrix& a, const Matrix& b) {
    int n = a.size();
    Matrix C(n);
    
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                C(i, j) += a(i, k) * b(k, j);
            }
        }
    }
    
    return C;
}

// Função para medir o tempo de execução
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
    std::cout << "================= VERSÃO SEQUENCIAL =================" << std::endl;
    
    for (int n : sizes) {
        std::cout << "\n--- Matriz " << n << "x" << n << " ---" << std::endl;
        
        // Cria matrizes A e B
        Matrix A(n);
        Matrix B(n);
        
        // Preenche matrizes com valores aleatórios
        A.fillRandom();
        B.fillRandom();
        
        // Mede tempo da implementação
        double time_seq = measureTime(sequential, A, B);
        
        // Exibe resultados
        std::cout << std::fixed << std::setprecision(4);
        std::cout << "Tempo de Execução: " << time_seq << " s" << std::endl;
        
    }
    
    return 0;
}
