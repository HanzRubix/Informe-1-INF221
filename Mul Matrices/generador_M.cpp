#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef vector<vector<int>> Matrix;

Matrix genCuadrado(int size, int maxValue) {
    Matrix matrix(size, vector<int>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = rand() % maxValue;
        }
    }
    return matrix;
}

void saveF(const vector<pair<Matrix, Matrix>>& matrices, const string& filename) {
    ofstream file(filename);
    for (size_t i = 0; i < matrices.size(); ++i) {
        const auto& matrixA = matrices[i].first;
        const auto& matrixB = matrices[i].second;
        int fil = matrixA.size();
        int col = matrixA[0].size();
        
        file << fil << " " << col << endl;
        for (const auto& row : matrixA) {
            for (int num : row) {
                file << num << " ";
            }
            file << endl;
        }
        
        file << endl; 

        file << fil << " " << col << endl;
        for (const auto& row : matrixB) {
            for (int num : row) {
                file << num << " ";
            }
            file << endl;
        }
        
        if (i != matrices.size() - 1) {
            file << endl; 
        }
    }
    file.close();
}

void gen100_1000() {
    vector<pair<Matrix, Matrix>> matrices;
    for (int size = 100; size <= 1000; size += 100) {
        matrices.push_back({genCuadrado(size, 1001), genCuadrado(size, 1001)}); 
    }
    saveF(matrices, "matrices_100_1000.txt");
}

// Fgenera 10 pares de matrices con dimensiones en potencias de 2 entre 2^2 y 2^11
void genPoten2() {
    vector<pair<Matrix, Matrix>> matrices;
    for (int exp = 2; exp <= 11; ++exp) {
        int size = 1 << exp; // Calcula 2^exp
        matrices.push_back({genCuadrado(size, 1001), genCuadrado(size, 1001)}); 
    }
    saveF(matrices, "matrices_potencias2.txt");
}

// genera 10 pares de matrices de dimension 512x512
void generarMatrices512() {
    vector<pair<Matrix, Matrix>> matrices;
    for (int i = 0; i < 10; ++i) {
        matrices.push_back({genCuadrado(512, 1001), genCuadrado(512, 1001)}); 
    }
    saveF(matrices, "matrices_512.txt");
}

int main() {
    srand(static_cast<unsigned>(time(0))); 


    gen100_1000();
    genPoten2();
    generarMatrices512();

    return 0;
}

