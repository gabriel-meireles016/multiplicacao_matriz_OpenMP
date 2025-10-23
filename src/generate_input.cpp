// generate_matrices.cpp
#include <iostream>
#include <vector>
#include <random>
#include <fstream>

class Matrix {
private:
    std::vector<std::vector<double>> data;
    int n;

public:
    // Construtor
    Matrix(int size) : n(size), data(size, std::vector<double>(size, 0.0)) {}
    
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
    
    // Salvar matriz em arquivo
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        file.write(reinterpret_cast<const char*>(&n), sizeof(n));
        for (int i = 0; i < n; i++) {
            file.write(reinterpret_cast<const char*>(data[i].data()), n * sizeof(double));
        }
        file.close();
    }
    
    // Getter para o tamanho
    int size() const {
        return n;
    }
};

int main() {
    std::vector<int> sizes = {512, 1024, 2048, 4096, 8192};

    for (int n : sizes) {
        
        // Verifica se as matrizes já existem
        std::string fileA = "matrix_A_" + std::to_string(n) + ".bin";
        std::string fileB = "matrix_B_" + std::to_string(n) + ".bin";
        
        std::ifstream testA(fileA);
        std::ifstream testB(fileB);
        
        if (testA.good() && testB.good()) {
            testA.close();
            testB.close();
            std::cout << "Matrizes " << n << "x" << n << " já existem." << std::endl;
            continue;
        }
        testA.close();
        testB.close();
        
        // Cria e preenche as matrizes
        Matrix A(n);
        A.fillRandom();
        
        Matrix B(n);
        B.fillRandom();
        
        // Salva as matrizes
        A.saveToFile(fileA);
        B.saveToFile(fileB);
        
        std::cout << "Arquivos: " << fileA << " e " << fileB << std::endl;

    }
    
    return 0;
}