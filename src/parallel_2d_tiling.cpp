#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <omp.h>

int TILE_SIZE = 128;

class Matrix {
private:
    std::vector<std::vector<double>> data;
    int n;

public:
    // Construtor
    Matrix(int size) : n(size), data(size, std::vector<double>(size, 0.0)) {}

    // Construtor de cópia
    Matrix(const Matrix& other) : n(other.n), data(other.data) {}

    // Carrega matriz de arquivo
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        file.read(reinterpret_cast<char*>(&n), sizeof(n));
        data.resize(n, std::vector<double>(n, 0.0));
        for (int i = 0; i < n; i++) {
            file.read(reinterpret_cast<char*>(data[i].data()), n * sizeof(double));
        }
        file.close();
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

// Matrix parallel_2d_tiling(const Matrix& A, const Matrix& B) {
//     int n = A.size();
//     Matrix C(n);

//     #pragma omp parallel for collapse(2)
//     for (int ii = 0; ii < n; ii += TILE_SIZE) {
//         for (int jj = 0; jj < n; jj += TILE_SIZE) {
//             for (int kk = 0; kk < n; kk += TILE_SIZE) {
//                 for (int i = ii; i < ii + TILE_SIZE; i++) {
//                     for (int j = jj; j < jj + TILE_SIZE; j++) {
//                         for (int k = kk; k < kk + TILE_SIZE; k++) {
//                             C(i, j) += A(i, k) * B(k, j);
//                         }
//                     }
//                 }
//             }
//         }
//     }

//     return C;
// }

Matrix parallel_2d_tiling(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n);

    #pragma omp parallel for collapse(2) schedule(static)
    for (int ii = 0; ii < n; ii += TILE_SIZE) {
        for (int jj = 0; jj < n; jj += TILE_SIZE) {
            // Limite superior I e J
            int i_lim = std::min(ii + TILE_SIZE, n);
            int j_lim = std::min(jj + TILE_SIZE, n);
            for (int kk = 0; kk < n; kk += TILE_SIZE) {
                // Limite superior K
                int k_lim = std::min(kk + TILE_SIZE, n);
                for (int i = ii; i < i_lim; ++i) {
                    for (int k = kk; k < k_lim; ++k) {
                        double aik = A(i, k);
                        for (int j = jj; j < j_lim; ++j) {
                            C(i, j) += aik * B(k, j);
                        }
                    }
                }
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

    std::cout << "============ VERSÃO PARALELA 2D C/ TILING ============" << std::endl;
    std::cout << "Número de threads disponíveis: 8" << std::endl;
    std::cout << "======================================================" << std::endl;

    // Configurando número de threads
    omp_set_num_threads(8);

    for (int n : sizes) {
        std::cout << "\n--- Matriz " << n << "x" << n << " ---" << std::endl;

        // Cria matrizes A e B
        Matrix A(n);
        Matrix B(n);

        // Carrega os arquivos de entrada
        std::string fileA = "matrix_A_" + std::to_string(n) + ".bin";
        std::string fileB = "matrix_B_" + std::to_string(n) + ".bin";

        std::ifstream testA(fileA);
        std::ifstream testB(fileB);

        if (testA.good() && testB.good()) {
            testA.close();
            testB.close();
            A.loadFromFile(fileA);
            B.loadFromFile(fileB);
            std::cout << "Matrizes carregadas do arquivo." << std::endl;
        } else {
            std::cout << "Erro ao carregar as matrizes." << std::endl;
            return 1;
        }

        // Mede tempo da implementação
        double time_par_2d = measureTime(parallel_2d_tiling, A, B);

        // Exibe resultados
        std::cout << std::fixed << std::setprecision(4);
        std::cout << "Tempo de Execução: " << time_par_2d << " s" << std::endl;

    }

    return 0;
}
